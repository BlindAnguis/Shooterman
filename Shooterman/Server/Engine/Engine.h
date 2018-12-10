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
  void createPlayers();
  Entity* createPlayer(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity, float maxHealth);
  Entity* createBall(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity);
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

  int x = 0;
};

