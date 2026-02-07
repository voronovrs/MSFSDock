#include "GDIPlusManager.hpp"
#include "GDIFonts.hpp"
#include "plugin/Logger.hpp"
#include <memory>
#include <vector>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Ole32.lib")

static ULONG_PTR g_gdiplusToken = 0;

static float dialCharWidth      = 0;
static float dialCharHeight     = 0;
static float dialFieldWidth     = 0;
static float dialFieldX         = 0;

static float radioCharHeight    = 0;
static float radioGap           = 0;
static float radioIntWidth      = 0;
static float radioFracWidth     = 0;
static float radioTotalWidth    = 0;
static float radioStartX        = 0;

using namespace Gdiplus;
const Color COLOR_WHITE             (255, 255, 255, 255);
const Color COLOR_OFF_WHITE         (255, 235, 235, 235);
const Color COLOR_ORANGE            (255, 160, 95, 0);
const Color COLOR_BRIGHT_ORANGE     (255, 255, 165, 0);
const Color COLOR_GRAY              (255, 150, 150, 150);
const Color COLOR_YELLOW            (255, 255, 255, 0);
const Color COLOR_RED               (255, 139, 0, 0);
const Color COLOR_BRIGHT_RED        (255, 255, 45, 45);
const Color COLOR_GREEN             (255,   0, 180,   0);
const Color COLOR_DARK_GREEN        (255,   0, 120,   0);
const Color COLOR_CYAN              (255,   0, 200, 200);
const Color COLOR_BLUE              (255,   0, 120, 220);
const Color COLOR_NEAR_BLACK        (255,  20,  20,  20);
const Color COLOR_DARK_BLUE         (255,  20,  40,  80);


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

std::wstring ToFixedWidth(int value, int width) {
    std::wstringstream ss;
    ss << std::setw(width) << std::setfill(L'0') << value;
    return ss.str();
}

// string to wstring
static std::wstring StringToWString(const std::string& str) {
    if (str.empty()) return std::wstring();

    int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), nullptr, 0);
    std::wstring wstr(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);

    return wstr;
}

Gdiplus::Color ColorFromHex(const std::string& hex) {
    if (hex.size() != 7 || hex[0] != '#') {
        return Gdiplus::Color(255, 0, 0, 0); // дефолт черный
    }

    unsigned int r, g, b;
    std::stringstream ss;

    ss << std::hex << hex.substr(1,2);
    ss >> r;
    ss.clear();
    ss.str("");

    ss << std::hex << hex.substr(3,2);
    ss >> g;
    ss.clear();
    ss.str("");

    ss << std::hex << hex.substr(5,2);
    ss >> b;

    return Gdiplus::Color(255, r, g, b); // a=255
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

// Draw red border if sim not connected
void DrawNotConnectedOutline(Gdiplus::Graphics& graphics, int width, int height) {
    int penWidth = 2.0f;
    Pen redPen(Color(255, 255, 0, 0), penWidth);
    graphics.DrawRectangle(&redPen, penWidth/2, penWidth/2, width - penWidth, height - penWidth);
}

// Draw header text
void DrawHeader(Gdiplus::Graphics& graphics, int frameWidth, const std::string& header, const Gdiplus::Color& headerColor,
    int headerFontSize, int headerOffset, int startX = 0, StringAlignment strAlign = StringAlignmentCenter) {
    using namespace Gdiplus;

    if (!header.empty()) {
        FontFamily fontFamily(L"Segoe UI Semibold");
        Font font(&fontFamily, TO_REAL(headerFontSize), FontStyleRegular, UnitPixel);
        SolidBrush brush(headerColor);

        RectF rect(startX, TO_REAL(headerOffset), TO_REAL(frameWidth), TO_REAL(headerFontSize + 4));
        StringFormat format;
        format.SetAlignment(strAlign);
        format.SetLineAlignment(StringAlignmentNear);

        std::wstring wheader = StringToWString(header);
        graphics.DrawString(wheader.c_str(), -1, &font, rect, &format, &brush);
    }
}

// Draw text over the loaded PNG and return base64
std::string DrawButtonImage(const std::wstring& imagePath,
                      const std::string& header, Color headerColor,
                      const std::string& data, Color dataColor,
                      int headerOffset, int headerFontSize,
                      int dataOffset, int dataFontSize, bool simConnected) {
    Gdiplus::Bitmap* bmp = LoadPNGImage(imagePath);
    if (!bmp || bmp->GetLastStatus() != Ok) {
        LogError("DrawTextOverImage LoadPNGImage error!");
        ShutdownGDIPlus();
        return {};
    }

    Graphics graphics(bmp);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

    DrawHeader(graphics, bmp->GetWidth(), header, headerColor, headerFontSize, headerOffset);


    if (simConnected) {
        if (!data.empty()) {
            SolidBrush brush(dataColor);

            RectF rect(0, TO_REAL(dataOffset), TO_REAL(bmp->GetWidth()), TO_REAL(dataFontSize + 4));
            StringFormat format;
            format.SetAlignment(StringAlignmentCenter);
            format.SetLineAlignment(StringAlignmentNear);

            std::wstring wdata = StringToWString(data);

            Font* font = GDIFonts::GetFont(TO_REAL(dataFontSize));
            // Font fallbackFont(L"Arial", 20, FontStyleRegular, UnitPixel);
            if (font) {
                if (font->GetLastStatus() != Ok) {
                    LogError("Font is invalid or not loaded properly.");
                }
                graphics.DrawString(wdata.c_str(), -1, font, rect, &format, &brush);
            } else {
                LogInfo("No font for drawing");
            }
        }
    } else {
        DrawHeader(graphics, bmp->GetWidth(), "NO SIM", COLOR_BRIGHT_RED, headerFontSize, bmp->GetHeight()/2 - headerFontSize/2);
    }

    std::string base64Image = BitmapToBase64(bmp);
    delete bmp;
    return base64Image;
}

// Draw dial data
void DrawDialData(Gdiplus::Graphics& graphics, int frameWidth, int dataFontSize, int dataOffset,
    const Gdiplus::Color& dataColor, const std::string& data) {
    using namespace Gdiplus;

    constexpr int MAX_DIGITS = 6;

    Font* font = GDIFonts::GetFont(TO_REAL(dataFontSize));
    if (!font || font->GetLastStatus() != Ok) {
        LogError("Font is invalid or not loaded properly.");
        return;
    }

    if (dialCharWidth == 0) {
        // measure char witdh and calculate field space once
        CharacterRange range(0, 1);
        StringFormat measureFmt(StringFormat::GenericTypographic());
        measureFmt.SetMeasurableCharacterRanges(1, &range);

        Region region;
        graphics.MeasureCharacterRanges(L"8", 1, font, RectF(0, 0, 1000, 1000), &measureFmt, 1, &region);

        RectF charRect;
        region.GetBounds(&charRect, &graphics);

        dialCharWidth  = charRect.Width;
        dialCharHeight = charRect.Height;
        dialFieldWidth = dialCharWidth * MAX_DIGITS;
        dialFieldX = (frameWidth - dialFieldWidth) * 0.5f + 2;
    }

    RectF fieldRect(dialFieldX, TO_REAL(dataOffset), dialFieldWidth, dialCharHeight);

    // Pen debugPen(Color(180, 255, 0, 0), 1.0f); // debug
    // graphics.DrawRectangle(&debugPen, fieldRect.X, fieldRect.Y, fieldRect.Width, fieldRect.Height);

    StringFormat drawFmt(StringFormat::GenericTypographic());
    drawFmt.SetAlignment(StringAlignmentNear);
    drawFmt.SetLineAlignment(StringAlignmentNear);

    // Draw display background
    {
        Color bgColor(25, dataColor.GetRed(), dataColor.GetGreen(), dataColor.GetBlue());
        SolidBrush bgBrush(bgColor);
        std::wstring bg(MAX_DIGITS, L'B');
        graphics.DrawString(bg.c_str(), -1, font, fieldRect, &drawFmt, &bgBrush);
    }

    // Data
    if (!data.empty()) {
        SolidBrush dataBrush(dataColor);
        std::wstring wdata = StringToWString(data);

        RectF dataBounds;
        graphics.MeasureString(wdata.c_str(), -1, font, RectF(0, 0, 1000, 1000), &drawFmt, &dataBounds);

        float xData = fieldRect.X + fieldRect.Width - dataBounds.Width;

        graphics.DrawString(wdata.c_str(), -1, font, PointF(xData, fieldRect.Y), &drawFmt, &dataBrush);
    }
}

// Draw text over the loaded PNG and return base64
std::string DrawDialImage(const std::wstring& imagePath,
                      const std::string& header, Color headerColor,
                      const std::string& data, Color dataColor,
                      const std::string& data2, Color data2Color,
                      int headerOffset, int headerFontSize,
                      int dataOffset, int dataFontSize,
                      int data2Offset, int data2FontSize, bool simConnected) {
    Gdiplus::Bitmap* bmp = LoadPNGImage(imagePath);
    if (!bmp || bmp->GetLastStatus() != Ok) {
        LogError("DrawTextOverImage LoadPNGImage error!");
        ShutdownGDIPlus();
        return {};
    }

    constexpr int MAX_DIGITS = 6;
    Graphics graphics(bmp);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

    DrawHeader(graphics, bmp->GetWidth(), header, headerColor, headerFontSize, headerOffset);

    DrawDialData(graphics, bmp->GetWidth(), dataFontSize, dataOffset, dataColor, data);
    DrawDialData(graphics, bmp->GetWidth(), data2FontSize, data2Offset, data2Color, data2);

    if (!simConnected) {
        DrawNotConnectedOutline(graphics, bmp->GetWidth(), bmp->GetHeight());
    }

    std::string base64Image = BitmapToBase64(bmp);
    delete bmp;
    return base64Image;
}

// Draw radio data
void DrawRadioData(Gdiplus::Graphics& graphics, int frameWidth, int dataFontSize, int dataOffset,
    const Gdiplus::Color& dataColor, const Gdiplus::Color& data2Color, int int_val, int frac_val) {
    using namespace Gdiplus;

    Font* font = GDIFonts::GetFont(TO_REAL(dataFontSize));
    if (!font || font->GetLastStatus() != Ok) {
        LogError("Font is invalid or not loaded properly.");
        return;
    }

    if (radioStartX == 0) {
        constexpr int INT_DIGITS  = 3;
        constexpr int FRAC_DIGITS = 3;

        CharacterRange range(0, 1);
        StringFormat measureFmt(StringFormat::GenericTypographic());
        measureFmt.SetMeasurableCharacterRanges(1, &range);

        Region region;
        graphics.MeasureCharacterRanges(L"8", 1, font, RectF(0, 0, 1000, 1000), &measureFmt, 1, &region);

        RectF charRect;
        region.GetBounds(&charRect, &graphics);

        float charWidth  = charRect.Width;
        radioCharHeight = charRect.Height;
        radioGap = charWidth * 0.3f;
        radioIntWidth  = charWidth * INT_DIGITS;
        radioFracWidth = charWidth * FRAC_DIGITS;
        radioTotalWidth = radioIntWidth + radioGap + radioFracWidth;
        radioStartX = (frameWidth - radioTotalWidth) * 0.5f + 2;
    }

    RectF fieldRect(radioStartX, TO_REAL(dataOffset), radioTotalWidth, radioCharHeight);

    // Pen debugPen(Color(180, 255, 0, 0), 1.0f); // debug
    // graphics.DrawRectangle(&debugPen, fieldRect.X, fieldRect.Y, fieldRect.Width, fieldRect.Height);

    StringFormat drawFmt(StringFormat::GenericTypographic());
    drawFmt.SetAlignment(StringAlignmentFar);
    drawFmt.SetLineAlignment(StringAlignmentNear);

    SolidBrush dataBrush(dataColor);
    SolidBrush data2Brush(data2Color);
    Color bgColor(25, dataColor.GetRed(), dataColor.GetGreen(), dataColor.GetBlue());
    SolidBrush bgBrush(bgColor);

    RectF intRect(radioStartX, fieldRect.Y, radioIntWidth, radioCharHeight);
    // graphics.DrawRectangle(&debugPen, intRect.X, intRect.Y, intRect.Width, intRect.Height);

    RectF fracRect(radioStartX + radioIntWidth + radioGap, fieldRect.Y, radioFracWidth, radioCharHeight);
    // graphics.DrawRectangle(&debugPen, fracRect.X, fracRect.Y, fracRect.Width, fracRect.Height);

    std::wstring intPart  = ToFixedWidth(int_val, 3);
    std::wstring fracPart = ToFixedWidth(frac_val, 3);

    graphics.DrawString(L"BBB", -1, font, intRect,  &drawFmt, &bgBrush);
    graphics.DrawString(L"BBB", -1, font, fracRect, &drawFmt, &bgBrush);

    graphics.DrawString(L".", -1, font, PointF(radioStartX + radioIntWidth + radioGap, fieldRect.Y + radioCharHeight * 0.05f), &drawFmt, &dataBrush);

    graphics.DrawString(intPart.c_str(), -1, font, intRect, &drawFmt, &dataBrush);

    graphics.DrawString(fracPart.c_str(), -1, font, fracRect, &drawFmt, &data2Brush);
}

// Draw text over the loaded PNG and return base64
std::string DrawRadioImage(const std::wstring& imagePath,
                      const std::string& header, Color headerColor,
                      int int_val, int frac_val, int stdb_val, int stdb_frac_val,
                      Color mainColor, Color stdbColorInt, Color stdbColorFrac,
                      int headerOffset, int headerFontSize,
                      int dataOffset, int dataFontSize,
                      int data2Offset, int data2FontSize, bool simConnected) {
    Gdiplus::Bitmap* bmp = LoadPNGImage(imagePath);
    if (!bmp || bmp->GetLastStatus() != Ok) {
        LogError("DrawTextOverImage LoadPNGImage error!");
        ShutdownGDIPlus();
        return {};
    }

    constexpr int MAX_DIGITS = 6;
    Graphics graphics(bmp);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);

    DrawHeader(graphics, bmp->GetWidth(), header, headerColor, headerFontSize, headerOffset);

    DrawRadioData(graphics, bmp->GetWidth(), dataFontSize, dataOffset, mainColor, mainColor, int_val, frac_val);
    DrawRadioData(graphics, bmp->GetWidth(), data2FontSize, data2Offset, stdbColorInt, stdbColorFrac, stdb_val, stdb_frac_val);

    if (!simConnected) {
        DrawNotConnectedOutline(graphics, bmp->GetWidth(), bmp->GetHeight());
    }

    std::string base64Image = BitmapToBase64(bmp);
    delete bmp;
    return base64Image;
}

// Draw arc with parameters
void DrawGaugeArc(Gdiplus::Graphics& graphics, const Gdiplus::RectF& arcRect, float startAngle, float sweepAngle,
    const Gdiplus::Color& color, float penWidth) {
    using namespace Gdiplus;

    Pen pen(color, penWidth);
    graphics.DrawArc(&pen, arcRect, startAngle, sweepAngle);
}

// Calculate current indicator angle
float computeIndicatorAngle(double val, float minVal, float maxVal, float baseAngle, float arcLength) {
    if (val < minVal) val = minVal;
    if (val > maxVal) val = maxVal;

    float t = (val - minVal) / (maxVal - minVal);
    return baseAngle + t * arcLength;
}

std::string DrawGaugeImage(const std::string& header, Color headerColor,
                           double value, const std::string& data, Color dataColor,
                           int headerOffset, int headerFontSize,
                           int dataOffset, int dataFontSize,
                           int minVal, int maxVal, bool fill,
                           std::string scaleColor, std::string indicatorColor, std::string bgColor, bool simConnected) {
    float baseAngle = 135.0f, arcLength = 180.0f, arcWidth = 12.0f, zeroArcWidth = 2.0f, pointerWidth = 16.0f,
        pointerLength = 9.0f;

    Gdiplus::Bitmap* bmp = new Gdiplus::Bitmap(72, 72, PixelFormat32bppARGB);

    if (!bmp || bmp->GetLastStatus() != Ok) {
        LogError("Bitmap create error!");
        ShutdownGDIPlus();
        return {};
    }

    // Draw background
    Graphics graphics(bmp);
    graphics.Clear(ColorFromHex(bgColor));
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // BG arc parameters
    const int w = bmp->GetWidth();
    const int h = bmp->GetHeight();

    float cx = w * 0.5f - 1.0f;
    float cy = h * 0.5f - 1.0f;
    float radius = 28.0f;

    RectF arcRect(
        cx - radius,
        cy - radius,
        radius * 2.0f,
        radius * 2.0f
    );

    // Draw BG indicator arc
    DrawGaugeArc(graphics, arcRect, baseAngle, arcLength, ColorFromHex(scaleColor), arcWidth);

    // Draw Zero arc if needed
    if (minVal < 0 && maxVal > 0) {
        float zeroAngle = computeIndicatorAngle(0.0, minVal, maxVal, baseAngle, arcLength);
        DrawGaugeArc(graphics, arcRect, zeroAngle, zeroArcWidth, COLOR_NEAR_BLACK, arcWidth);
    }

    float angle = computeIndicatorAngle(value, minVal, maxVal, baseAngle, arcLength);

    // Draw indicator arc
    if (fill) {
        float fillAngle =(angle - baseAngle) ? angle - baseAngle : 4.0f;
        DrawGaugeArc(graphics, arcRect, baseAngle, fillAngle, ColorFromHex(indicatorColor), pointerWidth - 6.0f);
    } else {
        DrawGaugeArc(graphics, arcRect, angle, pointerLength, ColorFromHex(indicatorColor), pointerWidth);
    }

    DrawHeader(graphics, bmp->GetWidth(), header, headerColor, headerFontSize, headerOffset, 18, StringAlignmentNear);

    // Draw data
    if (!data.empty()) {
        SolidBrush brush(dataColor);

        RectF rect(6, TO_REAL(dataOffset), TO_REAL(bmp->GetWidth()), TO_REAL(dataFontSize + 4));
        StringFormat format;
        format.SetAlignment(StringAlignmentCenter);
        format.SetLineAlignment(StringAlignmentNear);

        std::wstring wdata = StringToWString(data);

        FontFamily fontFamily(L"Consolas");
        Font font(&fontFamily, TO_REAL(dataFontSize), FontStyleRegular, UnitPixel);
        graphics.DrawString(wdata.c_str(), -1, &font, rect, &format, &brush);
    }

    if (!simConnected) {
        DrawNotConnectedOutline(graphics, bmp->GetWidth(), bmp->GetHeight());
    }

    std::string base64Image = BitmapToBase64(bmp);
    delete bmp;
    return base64Image;
}

// Draw rounded rectangle for switch background
void AddRoundedRect(Gdiplus::GraphicsPath& path, Gdiplus::RectF rect, float radius) {
    float d = radius * 2;

    path.AddArc(rect.X, rect.Y, d, d, 180, 90);
    path.AddArc(rect.X + rect.Width - d, rect.Y, d, d, 270, 90);
    path.AddArc(rect.X + rect.Width - d, rect.Y + rect.Height - d, d, d, 0, 90);
    path.AddArc(rect.X, rect.Y + rect.Height - d, d, d, 90, 90);
    path.CloseFigure();
}

std::string DrawSwitchImage(const std::vector<std::string>& labels, int currentPosition, const std::string& header,
    bool simConnected) {
    const float pointerDiameter = 20.0f;
    const float outlineRadius   = 2.0f;
    const float bgXoffset       = 6.0f;
    const float bgYoffset       = 2.0f;
    const float bgWidth         = 12.0f;

    const int bmpW = 72;
    const int bmpH = 72;

    int numPositions = static_cast<int>(labels.size());

    auto* bmp = new Gdiplus::Bitmap(bmpW, bmpH, PixelFormat32bppARGB);
    if (!bmp || bmp->GetLastStatus() != Ok) {
        LogError("Bitmap create error!");
        ShutdownGDIPlus();
        return {};
    }

    Graphics graphics(bmp);
    graphics.Clear(COLOR_NEAR_BLACK);
    graphics.SetTextRenderingHint(TextRenderingHintAntiAliasGridFit);
    graphics.SetSmoothingMode(SmoothingModeAntiAlias);

    // Switch background
    RectF BG(
        bgXoffset,
        bgYoffset,
        bgWidth,
        bmpH - bgYoffset * 2 - 16
    );

    SolidBrush bgBrush(COLOR_NEAR_BLACK);
    Pen bgPen(COLOR_OFF_WHITE, outlineRadius);

    GraphicsPath path;
    AddRoundedRect(path, BG, bgWidth * 0.5f);

    graphics.FillPath(&bgBrush, &path);
    graphics.DrawPath(&bgPen, &path);

    // Layout
    const float slotHeight = BG.Height / numPositions;

    // Pointerposition
    const float pointerCX = BG.X + BG.Width * 0.5f;
    const float pointerCY = BG.Y + slotHeight * (currentPosition + 0.5f);

    RectF pointerRect(
        pointerCX - pointerDiameter * 0.5f,
        pointerCY - pointerDiameter * 0.5f,
        pointerDiameter,
        pointerDiameter
    );

    SolidBrush pointerBrush(COLOR_DARK_GREEN);
    Pen pointerPen(COLOR_WHITE, outlineRadius);

    graphics.FillEllipse(&pointerBrush, pointerRect);
    graphics.DrawEllipse(&pointerPen, pointerRect);

    SolidBrush highlight(Color(80, 255, 255, 255));
    graphics.FillEllipse(
        &highlight,
        RectF(
            pointerRect.X + 2,
            pointerRect.Y + 2,
            pointerRect.Width - 4,
            pointerRect.Height - 4
        )
    );

    // Data
    const float dataRectX =
        std::max(pointerRect.GetRight(), BG.GetRight()) + 2.0f;

    float dataRectY = BG.Y + 2.0f;

    FontFamily fontFamily(L"Consolas");
    Font font(&fontFamily, 15.0f, FontStyleRegular, UnitPixel);
    SolidBrush textBrush(COLOR_OFF_WHITE);

    StringFormat format;
    format.SetAlignment(StringAlignmentCenter);
    format.SetLineAlignment(StringAlignmentCenter);

    for (int pos = 0; pos < numPositions; pos++) {
        RectF dataRect(dataRectX, dataRectY, bmpW - dataRectX, slotHeight);

        const std::string& text = labels[pos];

        std::wstring wdata = StringToWString(text);

        graphics.DrawString(
            wdata.c_str(),
            -1,
            &font,
            dataRect,
            &format,
            &textBrush
        );

        dataRectY += slotHeight;
    }

    if (!simConnected) {
        DrawNotConnectedOutline(graphics, bmp->GetWidth(), bmp->GetHeight());
    }

    if (!header.empty()) {
        Pen bottomPen(COLOR_OFF_WHITE, 1);
        graphics.DrawLine(&bottomPen, Point(0, BG.Height + bgYoffset * 2), Point(bmpW, BG.Height + bgYoffset * 2));
        DrawHeader(graphics, bmp->GetWidth(), header, COLOR_OFF_WHITE, 15.0f, BG.Height + bgYoffset);
    }

    std::string base64Image = BitmapToBase64(bmp);
    delete bmp;
    return base64Image;
}
