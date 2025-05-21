#include "GDIPlusManager.h"
#include "Logger.h"
#include <vector>

#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Ole32.lib")

static ULONG_PTR g_gdiplusToken = 0;

using namespace Gdiplus;

void InitGDIPlus() {
    if (g_gdiplusToken != 0)
        return; // Already initialized

    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&g_gdiplusToken, &gdiplusStartupInput, nullptr);
}

void ShutdownGDIPlus() {
    if (g_gdiplusToken != 0) {
        GdiplusShutdown(g_gdiplusToken);
        g_gdiplusToken = 0;
    }
}

ULONG_PTR GetGdiPlusToken() {
    return g_gdiplusToken;
}

// string to wstring
static std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);

    return wstr;
}

// Helper to load image from file
static Gdiplus::Bitmap* LoadPNGImage(const std::wstring& filePath) {
    return Gdiplus::Bitmap::FromFile(filePath.c_str(), FALSE);
}

// Convert Bitmap to base64 data URI string
static std::string BitmapToBase64(Gdiplus::Bitmap* bmp) {
    CLSID pngClsid;
    CLSIDFromString(L"{557CF406-1A04-11D3-9A73-0000F81EF32E}", &pngClsid);

    IStream* stream = nullptr;
    CreateStreamOnHGlobal(nullptr, TRUE, &stream);
    bmp->Save(stream, &pngClsid, nullptr);

    // Read stream
    STATSTG stats;
    stream->Stat(&stats, STATFLAG_NONAME);
    ULONG size = (ULONG)stats.cbSize.QuadPart;
    std::vector<BYTE> buffer(size);

    LARGE_INTEGER li = {};
    stream->Seek(li, STREAM_SEEK_SET, nullptr);
    ULONG read = 0;
    stream->Read(buffer.data(), size, &read);
    stream->Release();

    // Encode to base64
    DWORD base64Len = 0;
    CryptBinaryToStringA(buffer.data(), size, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, nullptr, &base64Len);
    std::string base64(base64Len, '\0');
    CryptBinaryToStringA(buffer.data(), size, CRYPT_STRING_BASE64 | CRYPT_STRING_NOCRLF, &base64[0], &base64Len);

    return "data:image/png;base64," + base64;
}

// Draw text over the loaded PNG and return base64
std::string DrawImage(const std::wstring& imagePath,
                      const std::string& header, const std::string& data, const std::string& data2,
                      int headerOffset, int headerFontSize, int dataOffset, int dataFontSize,
                      int data2Offset, int data2FontSize) {
    Gdiplus::Bitmap* bmp = LoadPNGImage(imagePath);
    if (!bmp || bmp->GetLastStatus() != Ok) {
        LogError("DrawTextOverImage LoadPNGImage error!");
        ShutdownGDIPlus();
        return {};
    }

    Graphics graphics(bmp);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

    if (!header.empty()) {
        // FontFamily fontFamily(L"Digital-7");
        FontFamily fontFamily(L"Segoe UI Semibold");
        Font font(&fontFamily, TO_REAL(headerFontSize), FontStyleRegular, UnitPixel);
        SolidBrush brush(Color(255, 255, 255, 255)); // White

        RectF rect(0, TO_REAL(headerOffset), TO_REAL(bmp->GetWidth()), TO_REAL(headerFontSize + 4));
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        std::wstring wheader = StringToWString(header);
        graphics.DrawString(wheader.c_str(), -1, &font, rect, &format, &brush);

        LogInfo("width " + std::to_string(bmp->GetWidth()));
        LogInfo("height " + std::to_string(bmp->GetHeight()));
    }

    if (!data.empty()) {
        FontFamily fontFamily(L"Digital-7");
        Font font(&fontFamily, TO_REAL(dataFontSize), FontStyleRegular, UnitPixel);
        SolidBrush brush(Color(255, 255, 255, 255)); // White

        // RectF rect(0, 0, static_cast<REAL>(bmp->GetWidth()), static_cast<REAL>(bmp->GetHeight()));
        RectF rect(0, TO_REAL(dataOffset), TO_REAL(bmp->GetWidth()), TO_REAL(dataFontSize + 4));
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        std::wstring wdata = StringToWString(data);
        graphics.DrawString(wdata.c_str(), -1, &font, rect, &format, &brush);
    }

    if (!data2.empty()) {
        FontFamily fontFamily(L"Digital-7");
        Font font(&fontFamily, TO_REAL(data2FontSize), FontStyleRegular, UnitPixel);
        SolidBrush brush(Color(255, 255, 255, 255)); // White

        // RectF rect(0, 0, static_cast<REAL>(bmp->GetWidth()), static_cast<REAL>(bmp->GetHeight()));
        RectF rect(0, TO_REAL(data2Offset), TO_REAL(bmp->GetWidth()), TO_REAL(data2FontSize + 4));
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        std::wstring wdata = StringToWString(data2);
        graphics.DrawString(wdata.c_str(), -1, &font, rect, &format, &brush);
    }

    std::string base64Image = BitmapToBase64(bmp);
    delete bmp;
    return base64Image;
}
