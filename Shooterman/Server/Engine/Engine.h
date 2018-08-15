#pragma once

#include "../Systems/MovementSystem/MovementSystem.h"
#include "../Systems/RenderSystem/RenderSystem.h"
#include "../Systems/InputSystem/InputSystem.h"
#include "../Systems/CollisionSystem/CollisionSystem.h"
#include "../EntityManager/EntityManager.h"
#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include <vector>

class Engine
{
public:
  Engine();
  ~Engine();
  void update();
  InputSystem* getInputSystem();
  MovementSystem* getMovementSystem();
  EntityManager* getEntityManager();
  // EntityFactory entityFactory;
  Entity* createPlayer(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity, float maxHealth);
  Entity* createBall(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity);
private:
  // Systems
  MovementSystem mMovementSystem;
  InputSystem mInputSystem;
  RenderSystem mRenderSystem;
  CollisionSystem mCollisionSystem;

  // Managers
  EntityManager mEntityManager;
  ComponentManager<PositionComponent> mPositionComponentManager;
  ComponentManager<RenderComponent> mRenderComponentManager;
  ComponentManager<VelocityComponent> mVelocityComponentManager;
  ComponentManager<SolidComponent> mSolidComponentManager;
  Subscriber mInputSubscriber;
};

