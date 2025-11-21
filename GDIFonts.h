#pragma once
#include <Windows.h>
#include <gdiplus.h>
#include <memory>
#include <map>
#include <string>

namespace GDIFonts {
    // Initialize and load custom font
    void LoadCustomFont(const std::wstring& relativePath);

    // Get cached font or create new one
    Gdiplus::Font* GetFont(float size);

    // Free cached fonts and reset
    void CleanupFont();
}
