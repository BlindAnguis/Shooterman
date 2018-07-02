#pragma once

#include <iostream>
#include <sstream>

//#define STR(msg) {  }

class Trace {
protected:
  std::string mName = "";
  bool mDebugEnabled = false;

  /*void TRACE_DEBUG(std::string msg) {
    if (mDebugEnabled) {
      std::ostringstream ss;
      ss << "[" << mName << "] DEBUG: " << msg << "\n";
      std::cout << ss.str(); 
    }
  }

  void TRACE_INFO(std::string msg) {
    std::ostringstream ss;
    ss << "[" << mName << "] INFO: " << msg << "\n";
    std::cout << ss.str();
  }

  void TRACE_WARNING(std::string msg) {
    std::ostringstream ss;
    ss << "[" << mName << "] WARNING: " << msg << "\n";
    std::cout << ss.str();
  }

  void TRACE_ERROR(std::string msg) {
    std::ostringstream ss;
    ss << "[" << mName << "] ERROR: " << msg << "\n";
    std::cout << ss.str();
  }*/
};


#define TRACE_DEBUG(msg) { \
  if (mDebugEnabled) { \
    std::ostringstream ss; \
    ss << "[" << mName << "] DEBUG: " << msg << "\n"; \
    std::cout << ss.str(); \
  } \
}

#define TRACE_INFO(msg) { \
  std::ostringstream ss; \
  ss << "[" << mName << "] INFO: " << msg << "\n"; \
  std::cout << ss.str(); }

#define TRACE_WARNING(msg) { \
  std::ostringstream ss; \
  ss << "[" << mName << "] WARNING: " << msg << "\n"; \
  std::cout << ss.str(); }

#define TRACE_ERROR(msg) { \
  std::ostringstream ss; \
  ss << "[" << mName << "] ERROR: " << msg << "\n"; \
  std::cout << ss.str(); }