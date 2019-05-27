#pragma once

#include "Collision.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../GridSystem/GridSystem.h"
#include "../DeleteSystem/DeleteSystem.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"

class CollisionSystem : Trace
{
public:
  CollisionSystem();
  ~CollisionSystem();
  static CollisionSystem& get();
  void handleAnyCollision(int causingColliderEntityId, float newXPos, float newYPos, GridSystem* gridSystem);
  void resetCollisionInformation();
  void resetSystem();
private:
  void handleCollision(int causingColliderEntityId, std::shared_ptr<RenderComponent> causingColliderMovingComponent, int affectedCollideeEntityId, std::shared_ptr<RenderComponent> affectedCollideeMovingComponent);
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  DeleteSystem* mDeleteSystem;
  bool mDebugMenuSubscribed = false;
  Subscriber mDebugMenuSubscriber;
  std::map<int, int> mCollisions; // First is the causing collider entity and second is the affected colidee entity.

  void handleDebugMessage();
};

