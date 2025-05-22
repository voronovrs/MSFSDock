#include <fstream>

#include "GDIPlusManager.h"
#include "Logger.h"
#include "Plugin.h"
#include "SimManager.h"
#include <thread>

#include "StreamDockCPPSDK/StreamDockSDK/HSDMain.h"


int main(int argc, const char** argv) {
  LogInit();
  InitGDIPlus();
  std::atexit(ShutdownGDIPlus);

  SimManager sim;
  sim.Start();

  auto plugin = std::make_unique<MSFSDockPlugin>();
  return esd_main(argc, argv, plugin.get());
}
