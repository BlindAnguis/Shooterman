#pragma once

#include "Collision.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../GridSystem/GridSystem.h"
#include "../DeleteSystem/DeleteSystem.h"
#include "../../../Common/Trace.h"

class CollisionSystem : Trace
{
public:
  CollisionSystem();
  CollisionSystem(ComponentManager<RenderComponent>* renderComponentManager, 
                  ComponentManager<VelocityComponent>* velocityComponentManager,
                  ComponentManager<CollisionComponent>* collisionComponentManager,
                  DeleteSystem* deleteSystem);
  ~CollisionSystem();
  void handleAnyCollision(int causingColliderEntityId, float newXPos, float newYPos, GridSystem* gridSystem);
  void resetCollisionInformation();
private:
  void handleCollision(int causingColliderEntityId, RenderComponent* causingColliderMovingComponent, int affectedCollideeEntityId, RenderComponent* affectedCollideeMovingComponent);
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  DeleteSystem* mDeleteSystem;
  std::map<int, int> mCollisions; // First is the causing collider entity and second is the affected colidee entity.
};

