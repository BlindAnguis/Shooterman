#pragma once

#include "../Systems/MovementSystem/MovementSystem.h"
#include "../Systems/RenderSystem/RenderSystem.h"
#include "../Systems/InputSystem/InputSystem.h"
#include "../Systems/CollisionSystem/CollisionSystem.h"
#include "../EntityManager/EntityManager.h"
#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include <vector>
#include <SFML\Network.hpp>
#include <array>
#include <map>

class Engine
{
public:
  Engine();
  Engine(std::array<std::array<int, 16>, 16> gameMap);
  ~Engine();
  void update();
  InputSystem* getInputSystem();
  MovementSystem* getMovementSystem();
  EntityManager* getEntityManager();
  // EntityFactory entityFactory;
  void createPlayers();
  Entity* createPlayer(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity, float maxHealth);
  Entity* createBall(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity);
  Entity* createHorizontalWall(float xPos, float yPos);
  Entity* createVerticalWall(float xPos, float yPos);
  void createMap();
  void setConnectedClients(std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> connectedClients) {
    mConnectedClients = connectedClients; 
    mInputSystem.setPlayers(mConnectedClients); 
    mMovementSystem.setPlayers(mConnectedClients);
  }
  std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> getConnectedClients() { return mConnectedClients; }
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
  std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> mConnectedClients;
  std::array<std::array<int, 16>, 16> mGameMap;
  std::array<sf::Texture*, 3> mTextures;

  sf::Texture* loadTexture(std::string fileName);
  float x = 60;
};

