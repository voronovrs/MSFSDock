#pragma once
#include <windows.h>
#include <gdiplus.h>
#include <string>

#define TO_REAL(x) static_cast<Gdiplus::REAL>(x)

void InitGDIPlus();
void ShutdownGDIPlus();
ULONG_PTR GetGdiPlusToken();
std::string DrawButtonImage(const std::wstring& imagePath, const std::string& header = "", const std::string& data = "",
                      const std::string& data2 = "", int headerOffset = 4, int headerFontSize = 16,
                      int dataOffset = 25, int dataFontSize = 20, int data2Offset = 25, int data2FontSize = 18);
