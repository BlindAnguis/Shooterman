#pragma once

#include <thread>
#include <memory>

#include "../MessageHandler/MessageHandler.h"
#include "Runnable.h"

class Process {
public:
  Process(std::unique_ptr<Runnable> runnable);
  ~Process();

  void shutdown();

private:
  bool mIsRunning;
  std::mutex* mThreadLock;
  std::unique_ptr<std::thread> mProcessThread;
  std::unique_ptr<Runnable> mRunnable;

  void execute();

  bool isRunning();
  void setIsRunning(bool isRunning);
};

