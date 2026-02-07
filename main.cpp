#include <fstream>

#include "plugin/Logger.hpp"
#include "plugin/Plugin.hpp"
#include "SimManager/SimManager.hpp"
#include "ui/GDIFonts.hpp"
#include "ui/GDIPlusManager.hpp"
#include <thread>

#include "StreamDockCPPSDK/StreamDockSDK/HSDMain.h"

void Exit(void) {
  SimManager::Instance().Stop();
  GDIFonts::CleanupFont();
  ShutdownGDIPlus();
}

int main(int argc, const char** argv) {
  LogInit();
  InitGDIPlus();
  GDIFonts::LoadCustomFont(L"\\fonts\\G7_Segment_7a.ttf");
  std::atexit(Exit);

#if defined(HW_PLATFORM_ELGATO)
  LogMessage("Platform: Elgato Streamdeck");
#elif defined(HW_PLATFORM_MIRABOX)
  LogMessage("Platform: Mirabox/Ajazz Streamdock");
#endif

  SimManager::Instance().Start();

  auto plugin = std::make_unique<MSFSDockPlugin>();
  return esd_main(argc, argv, plugin.get());
}
