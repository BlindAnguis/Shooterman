#include "ClockSystem.h"

ClockSystem::ClockSystem() {}

ClockSystem::ClockSystem(ComponentManager<ClockComponent>* clockComponentManager) : mClockComponentManager(clockComponentManager) {}

ClockSystem::~ClockSystem() {}

void ClockSystem::update() {
  for (auto clockComponent : mClockComponentManager->getAllComponents()) {
    if (clockComponent->timeout != -1 && clockComponent->clock.getElapsedTime().asMilliseconds() >= clockComponent->timeout) {
      clockComponent->timeoutCallback();
      clockComponent->clock.restart();
    }
  }
}