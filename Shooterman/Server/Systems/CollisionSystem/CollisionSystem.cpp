#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {}

CollisionSystem::CollisionSystem(
  ComponentManager<RenderComponent>* renderComponentManager,
  ComponentManager<VelocityComponent>* velocityComponentManager,
  ComponentManager<CollisionComponent>* collisionComponentManager,
  DeleteSystem* deleteSystem)
  :
  mRenderComponentManager(renderComponentManager),
  mVelocityComponentManager(velocityComponentManager),
  mCollisionComponentManager(collisionComponentManager),
  mDeleteSystem(deleteSystem)
{
  mName = "ENGINE: COLLISION_SYSTEM";
}

CollisionSystem::~CollisionSystem() {}

/**
 * TODO: 
 * Do an update method instead where I go through the list with all entities that have both solid and render component. 
 * Check if first entity collides with something. If it does broadcast the event to every mSubscriber.
 * Check next in the list and so on. For each entity check from the index after itself. Cause the indexes before have already checked against the current entity.
 *
 * It feels like this is going to be so much slower than I'm currently making it. Right now I first check only the things that want to move. 
 * Then I check if that movement will cause a collision. In case of collision I make some decisions. 
 * What I should do is maybe create a Collision Component and put that on the entity that has collided with something. Only on that entity cause if I put it on both I probably change stuff two times.
 * Then the systems that wants to know about collision can do stuff with the info.
 struct CollisionComponent {
  int colliderEntity;
  int collideeEntity;
 }
 * Or I can maybe send out to all subscribers that a collision has occured. It doesn't really matter but I think it's the best way to do it. Otherwise every world update must start with 
 * CollisionComponentManager.deleteAllCollisions(). But that way is fine too I guess. It might actually even be the better choice since then I can have a genereal update() method for
 * all the systems. Instead of having multiple callback methods. 
 */
/*
void CollisionSystem::handleAnyCollision(int movingEntity, float newXPos, float newYPos) {
  RenderComponent* movingComponent = mRenderComponentManager->getComponent(movingEntity);
  SolidComponent* movingIsSolid = mSolidComponentManager->getComponent(movingEntity);
  if (movingIsSolid) {
    sf::Vector2f oldPosition = movingComponent->sprite.getPosition();
    movingComponent->sprite.setPosition(newXPos, newYPos);

    for (auto stillEntityWithRender : mRenderComponentManager->getAllEntitiesWithRender()) {
      SolidComponent* stillIsSolid = mSolidComponentManager->getComponent(stillEntityWithRender.first);
      if (movingEntity != stillEntityWithRender.first && stillIsSolid && Collision::PixelPerfectTest(movingComponent->sprite, stillEntityWithRender.second->sprite)) {
        handleCollision(movingEntity, movingComponent, stillEntityWithRender.first, stillEntityWithRender.second);
        movingComponent->sprite.setPosition(oldPosition);
        return; // Remove this to allow something to collide with several things at once;
      }
    }
    movingComponent->sprite.setPosition(oldPosition);
  }
}
*/
void CollisionSystem::handleAnyCollision(int causingColliderEntityId, float newXPos, float newYPos, GridSystem* gridSystem) {
  RenderComponent* causingColliderMovingComponent = mRenderComponentManager->getComponent(causingColliderEntityId);
  CollisionComponent* causingColliderCollisionComponent = mCollisionComponentManager->getComponent(causingColliderEntityId);
  if (causingColliderCollisionComponent) {
    sf::Vector2f oldPosition = causingColliderMovingComponent->sprite.getPosition();
    causingColliderMovingComponent->sprite.setPosition(newXPos, newYPos);
    /*
    for (auto affectedCollideeEntity : mCollisionComponentManager->getAllEntitiesWithComponent()) {
      RenderComponent* affectedCollideeMovingComponent = mRenderComponentManager->getComponent(affectedCollideeEntity.first);
      // If collision with another entity
      if (causingColliderEntityId != affectedCollideeEntity.first && Collision::PixelPerfectTest(causingColliderMovingComponent->sprite, affectedCollideeMovingComponent->sprite)) {
        TRACE_INFO("Entity: " << causingColliderEntityId << " collided with: " << affectedCollideeEntity.first);

        handleCollision(causingColliderEntityId, causingColliderMovingComponent, affectedCollideeEntity.first, affectedCollideeMovingComponent);
        causingColliderMovingComponent->sprite.setPosition(oldPosition); // Reset movement.

        auto collisionList = causingColliderCollisionComponent->collidedList;

        // Add collision if causing collider has not previously collided with affected collidee
        if (std::find(collisionList.begin(), collisionList.end(), affectedCollideeEntity.first) == collisionList.end()) {
          mCollisions.emplace(causingColliderEntityId, affectedCollideeEntity.first);
          causingColliderCollisionComponent->collided = true;
          causingColliderCollisionComponent->collidedList.push_back(affectedCollideeEntity.first);

          affectedCollideeEntity.second->collided = true;
          affectedCollideeEntity.second->collidedList.push_back(causingColliderEntityId);
        }
        
        return; // Remove this to allow something to collide with several things at once;
      }
    }
    */

    for (auto affectedCollideeEntityId : gridSystem->getNearEntities((sf::Vector2i)causingColliderMovingComponent->sprite.getPosition())) {
      RenderComponent* affectedCollideeMovingComponent = mRenderComponentManager->getComponent(affectedCollideeEntityId);
      CollisionComponent* affectedCollideeCollisionComponent = mCollisionComponentManager->getComponent(affectedCollideeEntityId);

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

void CollisionSystem::handleCollision(int causingColliderEntityId, RenderComponent* causingColliderMovingComponent, int affectedCollideeEntityId, RenderComponent* affectedCollideeMovingComponent) {
  // TODO: Check if any of the components can deal damage and so on. What should happen when the moving component touches the stillComponent.

  VelocityComponent* causingColliderEntityVelocity = mVelocityComponentManager->getComponent(causingColliderEntityId);
  if (causingColliderEntityVelocity) {
    causingColliderEntityVelocity->currentVelocity.x = 0;
    causingColliderEntityVelocity->currentVelocity.y = 0;
  }

  /*
  if (!movingEntityVelocity->moveOnce) {
    //movingEntityVelocity->currentVelocity.x = movingEntityVelocity->currentVelocity.x + (-2 * movingEntityVelocity->currentVelocity.x);
    //movingEntityVelocity->currentVelocity.y = movingEntityVelocity->currentVelocity.y + (-2 * movingEntityVelocity->currentVelocity.y);
    movingEntityVelocity->currentVelocity.x = 0;
    movingEntityVelocity->currentVelocity.y = 0;
  } else {
    movingEntityVelocity->currentVelocity.x = 0;
    movingEntityVelocity->currentVelocity.y = 0;
  }
  */
}

void CollisionSystem::resetCollisionInformation() {
  //TRACE_INFO("Collisions made last update: " << mCollisions.size());
  for (auto& collision : mCollisions) {
    CollisionComponent* causingColliderEntity = mCollisionComponentManager->getComponent(collision.first);
    if (causingColliderEntity) {
      causingColliderEntity->collided = false;
      causingColliderEntity->collidedList.clear();
    }

    CollisionComponent* affectedCollideeEntity = mCollisionComponentManager->getComponent(collision.second);
    if (affectedCollideeEntity) {
      affectedCollideeEntity->collided = false;
      affectedCollideeEntity->collidedList.clear();
    }
  }

  mCollisions.clear();
  //TRACE_INFO("Collisions after reset: " << mCollisions.size());
}
