#pragma once

#include <stdio.h>
#include <chrono>
#include <ctime>
#include <sstream>

class Subscriber;

class Trace {
protected:
  std::string mName = "";
  bool mDebugEnabled1 = false;
  bool mDebugEnabled2 = false;
  bool mDebugEnabled3 = false;
  bool mDebugEnabled4 = false;
  bool mSendRecEnabled = false;
  bool mFunctionEnabled = false;

  void setupDebugMessages(std::string category, std::string name);
  void handleDebugMessages();
  void teardownDebugMessages();

private:
  std::string mCategory;
  std::string mButtonName;
  Subscriber* mDebugSubscriber;
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

#define TRACE_DEBUG1(msg) { \
  if (mDebugEnabled1) { \
    TRACE("DEBUG_1", msg); \
  } \
}

#define TRACE_DEBUG2(msg) { \
  if (mDebugEnabled2) { \
    TRACE("DEBUG_2", msg); \
  } \
}

#define TRACE_DEBUG3(msg) { \
  if (mDebugEnabled3) { \
    TRACE("DEBUG_3", msg); \
  } \
}

#define TRACE_DEBUG4(msg) { \
  if (mDebugEnabled4) { \
    TRACE("DEBUG_4", msg); \
  } \
}

#define TRACE_SEND(msg) { \
  if (mSendRecEnabled) { \
    TRACE("SEND", msg); \
  } \
}

#define TRACE_REC(msg) { \
  if (mSendRecEnabled) { \
    TRACE("REC", msg); \
  } \
}

#define TRACE_FUNC_ENTER() { \
  if (mFunctionEnabled) { \
    TRACE(__FUNCTION__, "Entered"); \
  } \
}

#define TRACE_FUNC_EXIT() { \
  if (mFunctionEnabled) { \
    TRACE(__FUNCTION__, "Exited"); \
  } \
}

#define TRACE_INFO(msg) { TRACE("INFO", msg); }

#define TRACE_WARNING(msg) { TRACE("WARNING", msg); }

#define TRACE_ERROR(msg) { TRACE("ERROR", msg); }

#define TRACE_ERROR_LONG(msg, file, line) { \
  std::ostringstream ss; \
  ss << "[" << mName << "] ERROR: " << msg << ", " << file << ":" << line << "\n"; \
  printf("%s", ss.str().c_str()); }