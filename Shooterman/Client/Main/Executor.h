#pragma once

#include <queue>

#include "Tasks/Task.h"
#include "../../Common/Trace.h"

class Executor : Trace {
public:
  Executor();
  ~Executor();

  void execute();
  void addTask(Task* newTask) { mExecuteQueue.push(newTask); }

private:
  std::queue<Task*> mExecuteQueue;
};