#pragma once

#include "../../../Common/Trace.h"

class Task : public Trace {
public:
  virtual Task* execute() = 0;
  virtual bool isTaskDone() { return false; }
  bool needRerun() { return mNeedRerun;  }

protected:
  bool mNeedRerun = false;
};