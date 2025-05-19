#include <fstream>

#include "GDIPlusManager.h"
#include "Logger.h"
#include "Plugin.h"
#include "Sim.h"
#include <thread>

#include "StreamDockCPPSDK/StreamDockSDK/HSDMain.h"


int main(int argc, const char** argv) {
  LogInit();
  InitGDIPlus();
  std::atexit(ShutdownGDIPlus);

  std::thread simThread([] {
    RunSimConnectLoop();  // your main loop
  });

  auto plugin = std::make_unique<MSFSDockPlugin>();
  return esd_main(argc, argv, plugin.get());
}
