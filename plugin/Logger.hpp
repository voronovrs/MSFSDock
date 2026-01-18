#pragma once
#include <fstream>

#ifndef NDEBUG
void LogInfo(const std::string& line);
void LogWarn(const std::string& line);
#else
inline void LogInfo(const std::string& line) {}
inline void LogWarn(const std::string& line) {}
#endif

void LogInit();
void LogMessage(const std::string& line);
void LogError(const std::string& line);
