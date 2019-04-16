#pragma once

#include <stdio.h>
#include <chrono>
#include <ctime>
#include <sstream>

class Trace {
protected:
  std::string mName = "";
  bool mDebugEnabled = false;
};

#define TRACE(type, msg) { \
  std::ostringstream ss; \
  time_t currentTimeInS = time(NULL); \
  auto currentTimeInMs = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count(); \
  char timeBuffer[80]; \
  tm timeInfo; \
  localtime_s(&timeInfo, &currentTimeInS); \
  strftime(timeBuffer, 80, "%F %H:%M:%S", &timeInfo); \
  ss << msg; \
  printf("[%s.%03lld] (%s) %s: %s\n", timeBuffer, (currentTimeInMs % 1000), mName.c_str(), type, ss.str().c_str()); }

#define TRACE_DEBUG(msg) { \
  if (mDebugEnabled) { \
    TRACE("DEBUG", msg); \
  } \
}

#define TRACE_INFO(msg) { TRACE("INFO", msg); }

#define TRACE_WARNING(msg) { TRACE("WARNING", msg); }

#define TRACE_ERROR(msg) { TRACE("ERROR", msg); }

#define TRACE_ERROR_LONG(msg, file, line) { \
  std::ostringstream ss; \
  ss << "[" << mName << "] ERROR: " << msg << ", " << file << ":" << line << "\n"; \
  printf("%s", ss.str().c_str()); }