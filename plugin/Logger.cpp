#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>
#include "Logger.hpp"

static std::string LOG_FILE = "log.txt";

static std::string NowTimeString() {
    using namespace std::chrono;

    auto now = system_clock::now();
    std::time_t tt = system_clock::to_time_t(now);

    std::tm tm{};
#if defined(_WIN32)
    localtime_s(&tm, &tt);
#else
    localtime_r(&tt, &tm);
#endif

    std::ostringstream ss;
    ss << std::put_time(&tm, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}

void LogInit() {
    std::ofstream log(LOG_FILE, std::ios::trunc);
    log << "==MSFSDock Plugin start==" << std::endl;
}

static void WriteLog(const std::string& level, const std::string& line) {
    std::ofstream log(LOG_FILE, std::ios::app);
    log << NowTimeString()
        << " [" << level << "] "
        << line
        << std::endl;
}

void LogMessage(const std::string& line) {
    WriteLog("MSG", line);
}

void LogError(const std::string& line) {
    WriteLog("ERR", line);
}

#ifndef NDEBUG
void LogInfo(const std::string& line) {
    WriteLog("INF", line);
}

void LogWarn(const std::string& line) {
    WriteLog("WRN", line);
}
#endif
