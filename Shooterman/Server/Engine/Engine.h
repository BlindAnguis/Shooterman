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
  Engine();
  Engine(std::array<std::array<int, 32>, 32> gameMap);
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
  std::shared_ptr<std::map<int, Player*>> getConnectedClients() { return mConnectedClients; }

  void collectPlayerData();

private:
  // Systems
  MovementSystem* mMovementSystem;
  InputSystem* mInputSystem;
  RenderSystem* mRenderSystem;
  CollisionSystem* mCollisionSystem;
  ClockSystem* mClockSystem;
  AnimationSystem* mAnimationSystem;
  HealthSystem* mHealthSystem;
  GridSystem* mGridSystem;
  DeleteSystem* mDeleteSystem;
  NetworkSystem* mNetworkSystem;
  PickupSystem* mPickupSystem;
  EntityCreatorSystem* mEntityCreatorSystem;

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
  std::shared_ptr<std::map<int, Player*>> mConnectedClients;
  std::array<std::array<int, 32>, 32> mGameMap;
  std::array<sf::Texture*, 99> mTextures;

  void destroyEntity(int entityId);
  MapCreator mMapCreator;
};

