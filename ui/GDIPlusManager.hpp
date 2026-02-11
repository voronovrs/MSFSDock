#pragma once
#define NOMINMAX
#include <windows.h>
#include <gdiplus.h>
#include <string>
#include <vector>

struct ScaleMarker {
    int position = 0;
    std::string color = "#ffffff";
};

#define TO_REAL(x) static_cast<Gdiplus::REAL>(x)
extern const Gdiplus::Color COLOR_WHITE;
extern const Gdiplus::Color COLOR_OFF_WHITE;
extern const Gdiplus::Color COLOR_ORANGE;
extern const Gdiplus::Color COLOR_BRIGHT_ORANGE;
extern const Gdiplus::Color COLOR_GRAY;
extern const Gdiplus::Color COLOR_YELLOW;
extern const Gdiplus::Color COLOR_RED;
extern const Gdiplus::Color COLOR_BRIGHT_RED;
extern const Gdiplus::Color COLOR_GREEN;
extern const Gdiplus::Color COLOR_DARK_GREEN;
extern const Gdiplus::Color COLOR_CYAN;
extern const Gdiplus::Color COLOR_BLUE;
extern const Gdiplus::Color COLOR_NEAR_BLACK;
extern const Gdiplus::Color COLOR_DARK_BLUE;


void InitGDIPlus();
void ShutdownGDIPlus();
ULONG_PTR GetGdiPlusToken();
std::string DrawButtonImage(const std::wstring& imagePath,
                            const std::string& header = "", Gdiplus::Color headerColor = COLOR_WHITE,
                            const std::string& data = "", Gdiplus::Color dataColor = COLOR_WHITE,
                            int headerOffset = 4, int headerFontSize = 16,
                            int dataOffset = 25, int dataFontSize = 20, bool simConnected = false);

std::string DrawDialImage(const std::wstring& imagePath,
                            const std::string& header = "", Gdiplus::Color headerColor = COLOR_WHITE,
                            const std::string& data = "", Gdiplus::Color dataColor = COLOR_WHITE,
                            const std::string& data2 = "", Gdiplus::Color data2Color = COLOR_WHITE,
                            int headerOffset = 4, int headerFontSize = 16,
                            int dataOffset = 25, int dataFontSize = 20,
                            int data2Offset = 25, int data2FontSize = 18, bool simConnected = false);

std::string DrawRadioImage(const std::wstring& imagePath,
                            const std::string& header = "", Gdiplus::Color headerColor = COLOR_WHITE,
                            int int_val = 0, int frac_val = 0, int stdb_val = 0, int stdb_frac_val = 0,
                            Gdiplus::Color mainColor = COLOR_WHITE,
                            Gdiplus::Color stdbColorInt = COLOR_WHITE,
                            Gdiplus::Color stdbColorFrac = COLOR_WHITE,
                            int headerOffset = 4, int headerFontSize = 16,
                            int dataOffset = 25, int dataFontSize = 20,
                            int data2Offset = 25, int data2FontSize = 18, bool simConnected = false);

std::string DrawGaugeImage(const std::string& header = "", Gdiplus::Color headerColor = COLOR_WHITE,
                           double value = 0, const std::string& data = "", Gdiplus::Color dataColor = COLOR_WHITE,
                           int headerOffset = 4, int headerFontSize = 16,
                           int dataOffset = 25, int dataFontSize = 20,
                           int minVal=0, int maxVal=10000, bool fill=false,
                           std::string scaleColor = "#ffff00", std::string indicatorColor = "#8b0000",
                           std::string bgColor = "#141414",
                           bool simConnected = false);

std::string DrawSwitchImage(const std::vector<std::string>& labels, int currentPosition,
                            const std::string& header = "", bool simConnected = false);

std::string DrawVerticalGaugeImage(const std::string& header = "", Gdiplus::Color headerColor = COLOR_WHITE,
                           double value = 0, const std::string& data = "", Gdiplus::Color dataColor = COLOR_WHITE,
                           int headerOffset = 57, int headerFontSize = 12,
                           int dataOffset = 0, int dataFontSize = 16,
                           int minVal = 0, int maxVal = 10000, bool fill = false,
                           std::string scaleColor = "#ffff00", std::string indicatorColor = "#8b0000",
                           std::string bgColor = "#141414",
                           bool simConnected = false,
                           const std::vector<ScaleMarker>& scaleMarkers = {});
