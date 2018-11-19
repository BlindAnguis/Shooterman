#include "CollisionSystem.h"

CollisionSystem::CollisionSystem() {}


CollisionSystem::CollisionSystem(
  ComponentManager<RenderComponent>* renderComponentManager,
  ComponentManager<SolidComponent>* solidComponentManager,
  ComponentManager<VelocityComponent>* velocityComponentManager)
  :
  mRenderComponentManager(renderComponentManager),
  mSolidComponentManager(solidComponentManager),
  mVelocityComponentManager(velocityComponentManager)
{}

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
void CollisionSystem::handleAnyCollision(int movingEntity, float newXPos, float newYPos) {
  RenderComponent* movingComponent = mRenderComponentManager->getComponent(movingEntity);
  SolidComponent* movingIsSolid = mSolidComponentManager->getComponent(movingEntity);
  if (movingIsSolid) {
    sf::Vector2f oldPosition = movingComponent->sprite.getPosition();
    movingComponent->sprite.setPosition(newXPos, newYPos);

    for (auto stillEntityWithRender : mRenderComponentManager->getAllEntitiesWithComponent()) {
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

void CollisionSystem::handleCollision(int movingEntityId, RenderComponent* movingComponent, int stillEntityId, RenderComponent* stillComponent) {
  // TODO: Check if any of the components can deal damage and so on. What should happen when the moving component touches the stillComponent.
  VelocityComponent* movingEntityVelocity = mVelocityComponentManager->getComponent(movingEntityId);

  if (!movingEntityVelocity->moveOnce) {
    movingEntityVelocity->currentVelocity.x = movingEntityVelocity->currentVelocity.x + (-2 * movingEntityVelocity->currentVelocity.x);
    movingEntityVelocity->currentVelocity.y = movingEntityVelocity->currentVelocity.y + (-2 * movingEntityVelocity->currentVelocity.y);
  }
  else {
    movingEntityVelocity->currentVelocity.x = 0;
    movingEntityVelocity->currentVelocity.y = 0;
  }
}
