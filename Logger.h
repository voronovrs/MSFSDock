#pragma once
#include <fstream>

#ifndef NDEBUG
void LogInfo(const std::string& line);
void LogWarn(const std::string& line);
#else
inline LogInfo(const std::string& line) {}
inline LogWarn(const std::string& line) {}
#endif

void LogInit();
void LogError(const std::string& line);
