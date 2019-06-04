#include "HealthSystem.h"
#include "../../../Common/Messages/SoundMessage.h"

HealthSystem::HealthSystem() :
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()) {
  mName = "SERVER: HEALTH_SYSTEM";
  MessageHandler::get().subscribeTo("ServerSoundList", &mSoundSubscriber);
}

HealthSystem::~HealthSystem() {
  TRACE_DEBUG1("Enter Destructor");
  MessageHandler::get().unsubscribeTo("ServerSoundList", &mSoundSubscriber);
}

HealthSystem& HealthSystem::get() {
  static HealthSystem instance;
  return instance;
}

void HealthSystem::changeHealth(int entityId, int addedHealthEffect) {
  if (mHealthComponentManager->hasComponent(entityId)) {
    auto health = mHealthComponentManager->getComponent(entityId);
    auto player = ComponentManager<PlayerComponent>::get().getComponent(entityId);
    if (health && (!player || !player->invinsible)) {
      health->currentHealth += addedHealthEffect;
      if (health->currentHealth > health->maxHealth) {
        health->currentHealth = health->maxHealth;
      } else if (health->currentHealth <= 0) {
        health->isAlive = false;
        health->currentHealth = 0;
        SoundMessage sm;
        sm.addSound(Sounds::Death);
        mSoundSubscriber.reverseSendMessage(sm.pack());
      }

      if (player && addedHealthEffect < 0 && health->isAlive) {
        SoundMessage sm;
        sm.addSound(Sounds::Hit1);
        mSoundSubscriber.reverseSendMessage(sm.pack());
      }
    }
  } else {
    TRACE_WARNING("Entity with id: " << entityId << " is set to receive a health change but it has no health component!");
  }
}

void HealthSystem::update()
{
  for (auto entityWithHealth : mHealthComponentManager->getAllEntitiesWithComponent()) {
    if (entityWithHealth.second->isAlive) {
      auto collisionComponent = mCollisionComponentManager->getComponent(entityWithHealth.first);
      if (collisionComponent && collisionComponent->collided) {
        for (auto collider : collisionComponent->collidedList) {
          auto collidingHealthChanger = mHealthChangerComponentManager->getComponent(collider);
          if (collidingHealthChanger) {
            if (collidingHealthChanger->immuneEntityIds.find(entityWithHealth.first) == collidingHealthChanger->immuneEntityIds.end()) {
              changeHealth(entityWithHealth.first, collidingHealthChanger->healthChange);
            }
          }
        }
      }
    }
  }
}
