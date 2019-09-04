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
  MovementSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<CollisionSystem> collisionSystem, std::shared_ptr<GridSystem> gridSystem);
  ~MovementSystem();
  void update(InputMessage inputMessage);
  void ownUpdate();
  void setPlayers(std::shared_ptr<std::map<int, Player*>> playersMap) { mPlayersMap = playersMap; }

private:
  //void move(PositionComponent* position, VelocityComponent* velocity);
  void move(int entityId, std::shared_ptr<RenderComponent> position, std::shared_ptr<VelocityComponent> velocity);
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  ComponentManager<PlayerComponent>* mPlayerComponentManager;
  std::shared_ptr<CollisionSystem> mCollisionSystem;
  std::shared_ptr<GridSystem> mGridSystem;
  EntityManager* mEntityManager;
  std::shared_ptr<std::map<int, Player*>> mPlayersMap;
  std::shared_ptr<MessageHandler> mMessageHandler;
};

