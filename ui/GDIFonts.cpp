#include "GDIFonts.hpp"
#include "plugin/Logger.hpp"
#include <vector>

using namespace Gdiplus;

std::string WideToUtf8(const std::wstring& wstr)
{
    if (wstr.empty())
        return {};

    int size = WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(), (int)wstr.size(),
        nullptr, 0,
        nullptr, nullptr
    );

    std::string result(size, 0);

    WideCharToMultiByte(
        CP_UTF8, 0,
        wstr.data(), (int)wstr.size(),
        result.data(), size,
        nullptr, nullptr
    );

    return result;
}

namespace GDIFonts {
    static std::unique_ptr<Gdiplus::PrivateFontCollection> fontCollection;
    static std::wstring customFontName;

    static std::wstring GetExecutableDir() {
        wchar_t buffer[MAX_PATH];
        GetModuleFileNameW(nullptr, buffer, MAX_PATH);
        std::wstring path(buffer);
        return path.substr(0, path.find_last_of(L"\\/"));
    }

    void LoadCustomFont(const std::wstring& path) {
        fontCollection = std::make_unique<Gdiplus::PrivateFontCollection>();

        std::wstring fullPath = GetExecutableDir() + path;
        if (fontCollection->AddFontFile(fullPath.c_str()) != Ok) {
            LogError("Failed to load custom font: " + WideToUtf8(path));
            fontCollection.reset();
            return;
        }

        INT count = fontCollection->GetFamilyCount();
        if (count == 0) {
            LogError("No font families found in collection");
            fontCollection.reset();
            return;
        }

        std::vector<Gdiplus::FontFamily> families(count);
        INT found = 0;
        fontCollection->GetFamilies(count, families.data(), &found);

        if (found > 0) {
            WCHAR familyName[LF_FACESIZE];
            families[0].GetFamilyName(familyName);
            customFontName = familyName;
            LogInfo("Custom font loaded: " + WideToUtf8(customFontName));
        }
    }

    Font* GetFont(float size) {
        if (!fontCollection || customFontName.empty()) {
            static Font fallbackFont(L"Arial", size, FontStyleRegular, UnitPixel);
            return &fallbackFont;
        }

        auto font = std::make_unique<Gdiplus::Font>(
            customFontName.c_str(),   // family name
            size,                     // size in pixels
            Gdiplus::FontStyleRegular,
            Gdiplus::UnitPixel,
            fontCollection.get()      // <-- tell GDI+ to look inside our private collection
        );

        if (font->GetLastStatus() != Ok) {
            LogError("Failed to create font, fallback to Arial");
            static Font fallbackFont(L"Arial", size, FontStyleRegular, UnitPixel);
            return &fallbackFont;
        }

        Gdiplus::Font* result = font.get();
        static std::map<float, std::unique_ptr<Gdiplus::Font>> cache;
        cache[size] = std::move(font);
        return result;
    }

    void CleanupFont() {
        customFontName.clear();
    }
}
