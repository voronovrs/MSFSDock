#include "GDIFonts.h"
#include "Logger.h"
#include <vector>

using namespace Gdiplus;

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
            LogError("Failed to load custom font: " + std::string(path.begin(), path.end()));
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
            LogInfo("Custom font loaded: " + std::string(customFontName.begin(), customFontName.end()));
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
