#include "Process.h"

Process::Process(std::unique_ptr<Runnable> runnable) {
  mRunnable = std::move(runnable);
  mThreadLock = new std::mutex();
  mProcessThread = std::make_unique<std::thread>(&Process::execute, this);
}

Process::~Process() {
  delete mThreadLock;
}

void Process::execute() {
  setIsRunning(true);

  mRunnable->start();

  while (isRunning()) {
    mRunnable->handleSubscribers();
    mRunnable->run();
  }

  mRunnable->stop();
}

bool Process::isRunning() {
  std::lock_guard<std::mutex> lockGuard(*mThreadLock);
  return mIsRunning;
}

void Process::setIsRunning(bool isRunning) {
  std::lock_guard<std::mutex> lockGuard(*mThreadLock);
  mIsRunning = isRunning;
}

void Process::shutdown() {
  setIsRunning(false);
  mProcessThread->join();
}

