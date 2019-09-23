#pragma once

#include "../../Common/Trace.h"
#include "../Systems/MovementSystem/MovementSystem.h"
#include "../Systems/RenderSystem/RenderSystem.h"
#include "../Systems/InputSystem/InputSystem.h"
#include "../Systems/CollisionSystem/CollisionSystem.h"
#include "../Systems/AnimationSystem/AnimationSystem.h"
#include "../Systems/HealthSystem/HealthSystem.h"
#include "../Systems/GridSystem/GridSystem.h"
#include "../Systems/NetworkSystem/NetworkSystem.h"
#include "../Systems/DeleteSystem/DeleteSystem.h"
#include "../Systems/ClockSystem/ClockSystem.h"
#include "../Systems/EntityCreatorSystem/EntityCreatorSystem.h"
#include "../Systems/ScoreSystem/ScoreSystem.h"
#include "../EntityManager/EntityManager.h"
#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include "../MapCreator/MapCreator.h"
#include "../Systems/PickupSystem/PickupSystem.h"
#include <vector>
#include <SFML\Network.hpp>
#include <array>
#include <map>

class Engine : Trace {
public:
  Engine(std::shared_ptr<MessageHandler> messageHandler);
  Engine(std::array<std::array<Textures, 32>, 32> gameMap, std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<NetworkSystem> networkHandler);
  ~Engine();
  void update();
  // EntityFactory entityFactory;
  void createPlayers();
  void createMap();
  void setConnectedClients(std::shared_ptr<std::map<int, Player*>> connectedClients) {
    mConnectedClients = connectedClients; 
    mInputSystem->setPlayers(mConnectedClients); 
    mMovementSystem->setPlayers(mConnectedClients);
  }
  void setMap(std::array<std::array<Textures, 32>, 32> map);

  std::shared_ptr<std::map<int, Player*>> getConnectedClients() { return mConnectedClients; }

  void collectPlayerData();

  bool isRoundFinished();

  void shutDown();

private:
  // Systems
  std::shared_ptr<MovementSystem> mMovementSystem;
  std::shared_ptr<InputSystem> mInputSystem;
  std::shared_ptr<RenderSystem> mRenderSystem;
  std::shared_ptr<CollisionSystem> mCollisionSystem;
  std::shared_ptr<ClockSystem> mClockSystem;
  std::shared_ptr<AnimationSystem> mAnimationSystem;
  std::shared_ptr<HealthSystem> mHealthSystem;
  std::shared_ptr<GridSystem> mGridSystem;
  std::shared_ptr<DeleteSystem> mDeleteSystem;
  std::shared_ptr<NetworkSystem> mNetworkSystem;
  std::shared_ptr<PickupSystem> mPickupSystem;
  std::shared_ptr<EntityCreatorSystem> mEntityCreatorSystem;
  std::shared_ptr<ManaSystem> mManaSystem;
  std::shared_ptr<ScoreSystem> mScoreSystem;

  // Managers
  EntityManager* mEntityManager;

  ComponentManager<PlayerComponent>* mPlayerComponentManager;
  ComponentManager<ClockComponent>* mClockComponentManager;
  ComponentManager<AnimationComponent>* mAnimationComponentManager;
  ComponentManager<HealthComponent>* mHealthComponentManager;
  ComponentManager<RenderComponent>* mRenderComponentManager;
  ComponentManager<CollisionComponent>* mCollisionComponentManager;
  ComponentManager<VelocityComponent>* mVelocityComponentManager;
  ComponentManager<HealthChangerComponent>* mHealthChangerComponentManager;
  ComponentManager<PickupComponent>* mPickupComponentManager;

  Subscriber mPlayerDataSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::shared_ptr<std::map<int, Player*>> mConnectedClients;
  std::array<std::array<Textures, 32>, 32> mGameMap;
  std::array<sf::Texture*, 99> mTextures;

  void destroyEntity(int entityId);
  MapCreator mMapCreator;

  void showScoreBoard();
};

