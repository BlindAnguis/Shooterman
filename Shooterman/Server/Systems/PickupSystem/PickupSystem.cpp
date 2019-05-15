#include "PickupSystem.h"
#include <time.h>
#include <vector>
#include <map>
#include "../HealthSystem/HealthSystem.h"
#include "../ManaSystem/ManaSystem.h"

const int MAX_NR_OF_PICKUPS = 10;
const int MAX_TIME_TO_NEXT_PICKUP = 5000;
const int MIN_TIME_TO_NEXT_PICKUP = 2500;

PickupSystem::PickupSystem()
  :
  mPickupComponentManager(&ComponentManager<PickupComponent>::get()),
  mEntityCreatorSystem(&EntityCreatorSystem::get())
{
  srand((int)time(0));
  mTimeToNextPickup = (rand() % MIN_TIME_TO_NEXT_PICKUP) + (MAX_TIME_TO_NEXT_PICKUP - MIN_TIME_TO_NEXT_PICKUP);
  mName = "SERVER: PICKUP_SYSTEM";
}

PickupSystem::~PickupSystem() {}

PickupSystem& PickupSystem::get() {
  static PickupSystem instance;
  return instance;
}

void PickupSystem::update() {
  if (mPickupClock.getElapsedTime().asMilliseconds() >= mTimeToNextPickup) {
    if (mPickupComponentManager->getAllComponents().size() <= MAX_NR_OF_PICKUPS) {
      TRACE_DEBUG("*************************************** NEW PICKUP ***********************************");
      auto pickup = mEntityCreatorSystem->createEntity(EntityType::RandomPickup, sf::Vector2f(), {});
      mTimeToNextPickup = (rand() % MIN_TIME_TO_NEXT_PICKUP) + (MAX_TIME_TO_NEXT_PICKUP - MIN_TIME_TO_NEXT_PICKUP);
      TRACE_DEBUG("Created a pickup of type: " << static_cast<int>(mPickupComponentManager->getComponent(pickup->id)->type) << " with id: " << pickup->id);
      mPickupClock.restart();
    }
  } else {
    mTimeToNextPickup--;
  }

  for (auto pickup : mPickupComponentManager->getAllEntitiesWithComponent()) {
    // Check for collision.
    auto cc = ComponentManager<CollisionComponent>::get().getComponent(pickup.first);
    if (cc && cc->collided) {
      TRACE_DEBUG("Pickup of type: " << static_cast<int>(pickup.second->type) << "with id: " << pickup.first << " picked up!");
      switch (pickup.second->type)
      {
      case PickupType::HealthPotion:
        // This case is handled by the HealthSystem since a health potion now has a HealthChangerComponent.
        break;
      case PickupType::ManaPotion:
        // TODO: Change to mana component.
        for (auto entity : cc->collidedList) {
          if (ComponentManager<ManaComponent>::get().hasComponent(entity)) {
            ManaSystem::get().changeMana(entity, pickup.second->addedEffect);
          }
        }
        break;
      case PickupType::Ammo:
        // TODO: Change to ammo component.
        for (auto entity : cc->collidedList) {
          if (ComponentManager<HealthComponent>::get().hasComponent(entity)) {
            //ComponentManager<HealthComponent>::get().getComponent(entity)->health += pickup.second->addedEffect;
            auto player = ComponentManager<PlayerComponent>::get().getComponent(entity);
            if (player) {
              player->superAttacks++;
            }
          }
        }
        break;
      default:
        break;
      }
    }
    // If collision, give addedEffect to the collidee?
  }
}
