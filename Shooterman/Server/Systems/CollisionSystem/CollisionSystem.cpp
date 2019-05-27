#include "CollisionSystem.h"

CollisionSystem::CollisionSystem()
  :
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mDeleteSystem(&DeleteSystem::get()) {
  mName = "ENGINE: COLLISION_SYSTEM";
}

CollisionSystem& CollisionSystem::get() {
  static CollisionSystem instance;
  return instance;
}

CollisionSystem::~CollisionSystem() { TRACE_DEBUG("Enter Destructor"); }

void CollisionSystem::handleAnyCollision(int causingColliderEntityId, float newXPos, float newYPos, GridSystem* gridSystem) {
  auto causingColliderMovingComponent = mRenderComponentManager->getComponent(causingColliderEntityId);
  auto causingColliderCollisionComponent = mCollisionComponentManager->getComponent(causingColliderEntityId);
  if (causingColliderCollisionComponent) {
    sf::Vector2f oldPosition = causingColliderMovingComponent->sprite.getPosition();
    causingColliderMovingComponent->sprite.setPosition(newXPos, newYPos);

    for (auto affectedCollideeEntityId : gridSystem->getNearEntities((sf::Vector2i)causingColliderMovingComponent->sprite.getPosition())) {
      auto affectedCollideeMovingComponent = mRenderComponentManager->getComponent(affectedCollideeEntityId);
      auto affectedCollideeCollisionComponent = mCollisionComponentManager->getComponent(affectedCollideeEntityId);

      // If collision with another entity
      if (causingColliderEntityId != affectedCollideeEntityId && Collision::PixelPerfectTest(causingColliderMovingComponent->sprite, affectedCollideeMovingComponent->sprite)) {
        //TRACE_INFO("Entity: " << causingColliderEntityId << " collided with: " << affectedCollideeEntityId);

        handleCollision(causingColliderEntityId, causingColliderMovingComponent, affectedCollideeEntityId, affectedCollideeMovingComponent);
        causingColliderMovingComponent->sprite.setPosition(oldPosition); // Reset movement.

        auto collisionList = causingColliderCollisionComponent->collidedList;

        // Add collision if causing collider has not previously collided with affected collidee
        if (std::find(collisionList.begin(), collisionList.end(), affectedCollideeEntityId) == collisionList.end()) {
          mCollisions.emplace(causingColliderEntityId, affectedCollideeEntityId);

          causingColliderCollisionComponent->collided = true;
          causingColliderCollisionComponent->collidedList.push_back(affectedCollideeEntityId);
          if (causingColliderCollisionComponent->destroyOnCollision) {
            mDeleteSystem->addEntity(causingColliderEntityId);
          }

          affectedCollideeCollisionComponent->collided = true;
          affectedCollideeCollisionComponent->collidedList.push_back(causingColliderEntityId);
          if (affectedCollideeCollisionComponent->destroyOnCollision) {
            mDeleteSystem->addEntity(affectedCollideeEntityId);
          }
        }

        return; // Remove this to allow something to collide with several things at once;
      }
    }

    causingColliderMovingComponent->sprite.setPosition(oldPosition);
  }
}

void CollisionSystem::handleCollision(int causingColliderEntityId, std::shared_ptr<RenderComponent> causingColliderMovingComponent, int affectedCollideeEntityId, std::shared_ptr<RenderComponent> affectedCollideeMovingComponent) {
  // TODO: Check if any of the components can deal damage and so on. What should happen when the moving component touches the stillComponent.

  auto causingColliderEntityVelocity = mVelocityComponentManager->getComponent(causingColliderEntityId);
  if (causingColliderEntityVelocity) {
    causingColliderEntityVelocity->currentVelocity.x = 0;
    causingColliderEntityVelocity->currentVelocity.y = 0;
  }
}

void CollisionSystem::resetCollisionInformation() {
  //TRACE_INFO("Collisions made last update: " << mCollisions.size());
  for (auto& collision : mCollisions) {
    auto causingColliderEntity = mCollisionComponentManager->getComponent(collision.first);
    if (causingColliderEntity) {
      causingColliderEntity->collided = false;
      causingColliderEntity->collidedList.clear();
    }

    auto affectedCollideeEntity = mCollisionComponentManager->getComponent(collision.second);
    if (affectedCollideeEntity) {
      affectedCollideeEntity->collided = false;
      affectedCollideeEntity->collidedList.clear();
    }
  }

  mCollisions.clear();
  //TRACE_INFO("Collisions after reset: " << mCollisions.size());
}
