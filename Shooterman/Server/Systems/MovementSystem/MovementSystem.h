#pragma once
#include "../ObserverIf.h"
#include "../../EntityManager/EntityManager.h"
#include "../CollisionSystem/CollisionSystem.h"
#include "../GridSystem/GridSystem.h"
#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include <SFML/Network.hpp>
#include "../../Player.h"

class MovementSystem : public ObserverIf
{
public:
  MovementSystem();
  ~MovementSystem();
  static MovementSystem& get();
  void update(InputMessage inputMessage);
  void ownUpdate();
  void setPlayers(std::shared_ptr<std::map<int, Player*>> playersMap) { mPlayersMap = playersMap; }

private:
  //void move(PositionComponent* position, VelocityComponent* velocity);
  void move(int entityId, RenderComponent* position, VelocityComponent* velocity);
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  ComponentManager<PlayerComponent>* mPlayerComponentManager;
  CollisionSystem* mCollisionSystem;
  GridSystem* mGridSystem;
  EntityManager* mEntityManager;
  std::shared_ptr<std::map<int, Player*>> mPlayersMap;
};

