#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"

class ClockSystem : public Trace {
public:
  ClockSystem();
  ~ClockSystem();
  void update();

private:
  ComponentManager<ClockComponent>* mClockComponentManager;
};

