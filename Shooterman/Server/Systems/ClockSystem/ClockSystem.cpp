#include "ClockSystem.h"

ClockSystem::ClockSystem() : mClockComponentManager(&ComponentManager<ClockComponent>::get()) {}

ClockSystem::~ClockSystem() {}

void ClockSystem::update() {
  for (auto clockComponent : mClockComponentManager->getAllComponents()) {
    if (clockComponent->timeout != -1 && clockComponent->clock.getElapsedTime().asMilliseconds() >= clockComponent->timeout) {
      clockComponent->timeoutCallback();
      clockComponent->clock.restart();
    }
  }
}