#include "HealthSystem.h"

HealthSystem::HealthSystem() :
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mDamageComponentManager(&ComponentManager<DamageComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get())
{
  mName = "SERVER: HEALTH_SYSTEM";
}

HealthSystem::~HealthSystem()
{
}

HealthSystem& HealthSystem::get() {
  static HealthSystem instance;
  return instance;
}

void HealthSystem::changeHealth(int entityId, int addedHealthEffect) {
  if (mHealthComponentManager->hasComponent(entityId)) {
    auto hc = mHealthComponentManager->getComponent(entityId);
    hc->currentHealth += addedHealthEffect;
    if (hc->currentHealth > hc->maxHealth) {
      hc->currentHealth = hc->maxHealth;
    }
    else if (hc->currentHealth <= 0) {
      hc->isAlive = false;
    }
  }
}

void HealthSystem::update()
{
  for (auto entityWithHealth : mHealthComponentManager->getAllEntitiesWithComponent()) {
    //TRACE_INFO("Entity: " << entityWithHealth.first << " has health: " << entityWithHealth.second->health);
    if (entityWithHealth.second->isAlive) {
      CollisionComponent* collisionComponent = mCollisionComponentManager->getComponent(entityWithHealth.first);
      //TRACE_INFO("Entity: " << entityWithHealth.first << " has colided? " << (collisionComponent->collided ? "TRUE" : "FALSE"));
      if (entityWithHealth.second->isAlive && collisionComponent && collisionComponent->collided) {
        //TRACE_INFO("Entity: " << entityWithHealth.first << " has health: " << entityWithHealth.second->health);
        //TRACE_INFO("Check if health should go down");
        for (auto collider : collisionComponent->collidedList) {
          DamageComponent* collidingDamage = mDamageComponentManager->getComponent(collider);
          if (collidingDamage) {
            entityWithHealth.second->currentHealth -= collidingDamage->damage;
            if (entityWithHealth.second->currentHealth <= 0) {
              entityWithHealth.second->isAlive = false;
            }
          }
        }
        //TRACE_INFO("Entity: " << entityWithHealth.first << " has new health: " << entityWithHealth.second->health);
      }
    }
  }
}
