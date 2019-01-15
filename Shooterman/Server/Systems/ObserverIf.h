#pragma once
#include "../../Common/Trace.h"
#include "../../Common/Messages/InputMessage.h"

class ObserverIf : public Trace
{
public:
  virtual ~ObserverIf() {}
  virtual void update(InputMessage input) { TRACE_ERROR("Unimplemented function"); }
  virtual void update(int input, int ID) { TRACE_ERROR("Unimplemented function"); }
};