#pragma once
#include "../ObserverIf.h"
#include "../../EntityManager/EntityManager.h"
#include "../CollisionSystem/CollisionSystem.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include <SFML/Network.hpp>
#include "../../Player.h"

class MovementSystem : public ObserverIf
{
public:
  MovementSystem();
  MovementSystem(
    ComponentManager<VelocityComponent>* velocityComponentManager,
    ComponentManager<RenderComponent>* renderComponentManager,
    CollisionSystem* collisionSystem,
    EntityManager* entityManager,
    ComponentManager<AnimationComponent>* animationComponentManager
  );
  ~MovementSystem();
  void update(InputMessage inputMessage);
  void ownUpdate();
  void setPlayers(std::shared_ptr<std::map<int, Player*>> playersMap) { mPlayersMap = playersMap; }

private:
  //void move(PositionComponent* position, VelocityComponent* velocity);
  void move(RenderComponent* position, VelocityComponent* velocity);
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  CollisionSystem* mCollisionSystem;
  EntityManager* mEntityManager;
  std::shared_ptr<std::map<int, Player*>> mPlayersMap;
};

