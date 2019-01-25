#pragma once

#include "../../Common/Trace.h"
#include "../Systems/MovementSystem/MovementSystem.h"
#include "../Systems/RenderSystem/RenderSystem.h"
#include "../Systems/InputSystem/InputSystem.h"
#include "../Systems/CollisionSystem/CollisionSystem.h"
#include "../Systems/AnimationSystem/AnimationSystem.h"
#include "../Systems/HealthSystem/HealthSystem.h"
#include "../Systems/GridSystem/GridSystem.h"
#include "../EntityManager/EntityManager.h"
#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
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
  InputSystem* getInputSystem();
  MovementSystem* getMovementSystem();
  EntityManager* getEntityManager();
  // EntityFactory entityFactory;
  void createPlayers();
  Entity* createPlayer(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity, float maxHealth);
  Entity* createHorizontalWall(float xPos, float yPos);
  Entity* createVerticalWall(float xPos, float yPos);
  Entity* createWholeFloor(float xPos, float yPos);
  Entity* createCrackedFloor(float xPos, float yPos);
  Entity* createFloorSpikes(float xPos, float yPos);
  Entity* createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition);
  void createMap();
  void setConnectedClients(std::shared_ptr<std::map<int, Player*>> connectedClients) {
    mConnectedClients = connectedClients; 
    mInputSystem.setPlayers(mConnectedClients); 
    mMovementSystem.setPlayers(mConnectedClients);
  }
  std::shared_ptr<std::map<int, Player*>> getConnectedClients() { return mConnectedClients; }

private:
  // Systems
  MovementSystem mMovementSystem;
  InputSystem mInputSystem;
  RenderSystem mRenderSystem;
  CollisionSystem mCollisionSystem;
  AnimationSystem mAnimationSystem;
  HealthSystem mHealthSystem;
  GridSystem mGridSystem;

  // Managers
  EntityManager mEntityManager;
  ComponentManager<PositionComponent> mPositionComponentManager;
  ComponentManager<RenderComponent> mRenderComponentManager;
  ComponentManager<VelocityComponent> mVelocityComponentManager;
  ComponentManager<CollisionComponent> mCollisionComponentManager;
  ComponentManager<AnimationComponent> mAnimationComponentManager;
  ComponentManager<HealthComponent> mHealthComponentManager;
  ComponentManager<DamageComponent> mDamageComponentManager;
  ComponentManager<ClockComponent> mClockComponentManager;
  ComponentManager<PlayerComponent> mPlayerComponentManager;

  Subscriber mInputSubscriber;
  std::shared_ptr<std::map<int, Player*>> mConnectedClients;
  std::array<std::array<int, 32>, 32> mGameMap;
  std::array<sf::Texture*, 99> mTextures;

  sf::Texture* loadTexture(std::string fileName);
  void destroyEntity(int entityId);
  float x = 100;
};

