#include "Logger.h"

static std::string LOG_FILE = "log.txt";

void LogInit() {
    std::ofstream log(LOG_FILE, std::ios::trunc);
    log << "==MSFSDock Plugin start==" << std::endl;
}

static void WriteLog(const std::string& level, const std::string& line) {
    std::ofstream log(LOG_FILE, std::ios::app);
    log << level << ": " << line << std::endl;
}

void LogInfo(const std::string& line) {
    WriteLog("INFO", line);
}

void LogWarn(const std::string& line) {
    WriteLog("WARN", line);
}

void LogError(const std::string& line) {
    WriteLog("ERROR", line);
}

