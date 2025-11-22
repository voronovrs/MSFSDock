#include <fstream>

#include "GDIFonts.h"
#include "GDIPlusManager.h"
#include "Logger.h"
#include "Plugin.h"
#include "SimManager.h"
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

  SimManager::Instance().Start();

  auto plugin = std::make_unique<MSFSDockPlugin>();
  return esd_main(argc, argv, plugin.get());
}
