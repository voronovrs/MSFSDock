#include <fstream>

#include "plugin/Logger.hpp"
#include "plugin/Plugin.hpp"
#include "SimManager/SimManager.hpp"
#include "ui/GDIFonts.hpp"
#include "ui/GDIPlusManager.hpp"
#include <thread>

#include "StreamDockCPPSDK/StreamDockSDK/HSDMain.h"

#ifdef COREDUMP
#include <windows.h>
#include <dbghelp.h>

#pragma comment(lib, "dbghelp.lib")


void WriteMiniDump(EXCEPTION_POINTERS* pep)
{
    HANDLE hFile = CreateFileA(
        "msfsdock_crash.dmp",
        GENERIC_WRITE,
        0,
        nullptr,
        CREATE_ALWAYS,
        FILE_ATTRIBUTE_NORMAL,
        nullptr
    );

    if (hFile == INVALID_HANDLE_VALUE)
        return;

    MINIDUMP_EXCEPTION_INFORMATION mdei{};
    mdei.ThreadId = GetCurrentThreadId();
    mdei.ExceptionPointers = pep;
    mdei.ClientPointers = FALSE;

    MiniDumpWriteDump(
        GetCurrentProcess(),
        GetCurrentProcessId(),
        hFile,
        MiniDumpNormal,
        pep ? &mdei : nullptr,
        nullptr,
        nullptr
    );

    CloseHandle(hFile);
}

LONG WINAPI CrashHandler(EXCEPTION_POINTERS* pep)
{
    WriteMiniDump(pep);
    return EXCEPTION_EXECUTE_HANDLER;
}
#endif

void StartupLog(void) {
  LogMessage("== MSFSDock Plugin start ==");
  LogMessage(std::string("== Version: ") + MSFSDOCK_VERSION + " ==");
  LogMessage("== Developed by Roman Voronov ==");
#if defined(HW_PLATFORM_ELGATO)
  LogMessage("== Platform: Elgato Streamdeck ==");
#elif defined(HW_PLATFORM_MIRABOX)
  LogMessage("== Platform: Mirabox/Ajazz Streamdock ==");
#endif
}

void Exit(void) {
  SimManager::Instance().Stop();
  GDIFonts::CleanupFont();
  ShutdownGDIPlus();
}

int main(int argc, const char** argv) {
  LogInit();
  StartupLog();

#ifdef COREDUMP
  LogMessage("Core dump enabled");
  SetUnhandledExceptionFilter(CrashHandler);
#endif

  InitGDIPlus();
  GDIFonts::LoadCustomFont(L"\\fonts\\G7_Segment_7a.ttf");
  std::atexit(Exit);

  SimManager::Instance().Start();

  auto plugin = std::make_unique<MSFSDockPlugin>();
  return esd_main(argc, argv, plugin.get());
}
