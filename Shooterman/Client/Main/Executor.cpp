#include "Executor.h"

Executor::Executor() {

}

Executor::~Executor() {
  while (!mExecuteQueue.empty()) {
    Task* task = mExecuteQueue.front();
    mExecuteQueue.pop();
    delete task;
  }
}

void Executor::execute() {
  if (!mExecuteQueue.empty()) {
    Task* task = mExecuteQueue.front();
    mExecuteQueue.pop();

    Task* nextTask = task->execute();

    if (nextTask->isTaskDone()) {
      delete task;
      delete nextTask;
      return;
    }
    
    if (!task->needRerun()) {
      delete task;
    }

    mExecuteQueue.push(nextTask);
  }
}