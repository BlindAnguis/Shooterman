#pragma once

#include "Collision.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"

class CollisionSystem
{
public:
  CollisionSystem();
  CollisionSystem(ComponentManager<RenderComponent>* renderComponentManager, 
                  ComponentManager<SolidComponent>* solidComponentManager, 
                  ComponentManager<VelocityComponent>* velocityComponentManager,
                  ComponentManager<CollisionComponent>* collisionComponentManager);
  ~CollisionSystem();
  void handleAnyCollision(int movingEntity, float newXPos, float newYPos);
private:
  void handleCollision(int movingEntityId, RenderComponent* movingComponent, int stillEntityId, RenderComponent* stillComponent);
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<SolidComponent>* mSolidComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
};

