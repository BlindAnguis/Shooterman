#include "ClockSystem.h"

ClockSystem::ClockSystem() : mClockComponentManager(&ComponentManager<ClockComponent>::get()) { mName = "SERVER: CLOCK_SYSTEM"; }

ClockSystem::~ClockSystem() { TRACE_DEBUG1("Enter Destructor"); }

void ClockSystem::update() {
  for (auto clockComponent : mClockComponentManager->getAllComponents()) {
    if (clockComponent->timeout != -1 && clockComponent->clock.getElapsedTime().asMilliseconds() >= clockComponent->timeout) {
      clockComponent->timeoutCallback();
      clockComponent->clock.restart();
    }
  }
}