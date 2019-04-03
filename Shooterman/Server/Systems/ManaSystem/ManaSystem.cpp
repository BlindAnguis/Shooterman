#include "ManaSystem.h"

ManaSystem::ManaSystem() :
  mManaComponentManager(&ComponentManager<ManaComponent>::get())
{
  mName = "SERVER: MANA_SYSTEM";
}

ManaSystem::~ManaSystem()
{
}

ManaSystem& ManaSystem::get() {
  static ManaSystem instance;
  return instance;
}

void ManaSystem::changeMana(int entityId, int addedManaEffect) {
  if (mManaComponentManager->hasComponent(entityId)) {
    auto hc = mManaComponentManager->getComponent(entityId);
    hc->currentMana += addedManaEffect;
    if (hc->currentMana > hc->maxMana) {
      hc->currentMana = hc->maxMana;
    }
    if (hc->currentMana < 0) {
      hc->currentMana = 0;
    }
  }
}

void ManaSystem::update()
{
  // TODO: What?
}
