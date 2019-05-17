#include "ClockSystem.h"

ClockSystem::ClockSystem() : mClockComponentManager(&ComponentManager<ClockComponent>::get()) { mName = "SERVER: CLOCK_SYSTEM"; }

ClockSystem::~ClockSystem() { TRACE_DEBUG("Enter Destructor"); }

ClockSystem& ClockSystem::get() {
  static ClockSystem instance;
  return instance;
}

void ClockSystem::update() {
  for (auto clockComponent : mClockComponentManager->getAllComponents()) {
    if (clockComponent->timeout != -1 && clockComponent->clock.getElapsedTime().asMilliseconds() >= clockComponent->timeout) {
      clockComponent->timeoutCallback();
      clockComponent->clock.restart();
    }
  }
}