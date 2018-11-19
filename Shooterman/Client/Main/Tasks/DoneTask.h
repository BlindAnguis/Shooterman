#pragma once

#include "Task.h"

class DoneTask : public Task {
public:
  Task* execute() { return this; }
  bool isTaskDone() { return true; }
};