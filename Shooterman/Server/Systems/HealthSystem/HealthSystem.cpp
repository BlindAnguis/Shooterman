#include "HealthSystem.h"

HealthSystem::HealthSystem() {}

HealthSystem::HealthSystem(
  ComponentManager<HealthComponent>* healthComponentManager,
  ComponentManager<DamageComponent>* damageComponentManager,
  ComponentManager<CollisionComponent>* collisionComponentManager
) :
  mHealthComponentManager(healthComponentManager),
  mDamageComponentManager(damageComponentManager),
  mCollisionComponentManager(collisionComponentManager)
{
  mName = "SERVER: HEALTH_SYSTEM";
}

HealthSystem::~HealthSystem()
{
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
            entityWithHealth.second->health -= collidingDamage->damage;
            if (entityWithHealth.second->health <= 0) {
              entityWithHealth.second->isAlive = false;
            }
          }
        }
        //TRACE_INFO("Entity: " << entityWithHealth.first << " has new health: " << entityWithHealth.second->health);
      }
    }
  }
}
