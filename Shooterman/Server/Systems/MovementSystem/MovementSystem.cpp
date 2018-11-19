#include "MovementSystem.h"
#include "../../../Common/KeyBindings.h"
#include <iostream>

MovementSystem::MovementSystem() {}

MovementSystem::MovementSystem(
  ComponentManager<VelocityComponent>* velocityComponentManager,
  ComponentManager<RenderComponent>* renderComponentManager,
  CollisionSystem* collisionSystem,
  EntityManager* entityManager
) :
  mVelocityComponentManager(velocityComponentManager),
  mRenderComponentManager(renderComponentManager),
  mCollisionSystem(collisionSystem),
  mEntityManager(entityManager) 
{}

MovementSystem::~MovementSystem() {}

void MovementSystem::update(int input, int ID)
{
  // TODO: Change this so it only moves the relevant entity! Since one input is just for a specific player. Right now all entities will be moved.

  //std::cout << "[SERVER: MOVEMENT_SYSTEM] update called with input: " << input << std::endl;
  // If input is a moving input
  if (input == D_KEY || input == A_KEY || input == S_KEY || input == W_KEY ||
    input == A_S_KEY || input == A_W_KEY || input == D_W_KEY || input == D_S_KEY) {
    //for (auto entityWithPosition : mPositionComponentManager->getAllEntitiesWithPosition())
    
    Entity* e = mPlayersMap->at(ID).second;
    if (e && mRenderComponentManager->hasComponent(e->id)) {
      VelocityComponent* velocity = mVelocityComponentManager->getComponent(e->id);
      if (velocity) {
        if (input == D_KEY) {
          velocity->currentVelocity.x = velocity->maxVelocity.x;
        }
        else if (input == A_KEY) {
          velocity->currentVelocity.x = -velocity->maxVelocity.x;
        }
        else if (input == W_KEY) {
          velocity->currentVelocity.y = -velocity->maxVelocity.y;
        }
        else if (input == S_KEY) {
          velocity->currentVelocity.y = velocity->maxVelocity.y;
        }
        else if (input == A_S_KEY) {
          velocity->currentVelocity.x = -velocity->maxVelocity.x;
          velocity->currentVelocity.y = velocity->maxVelocity.y;
        }
        else if (input == A_W_KEY) {
          velocity->currentVelocity.x = -velocity->maxVelocity.x;
          velocity->currentVelocity.y = -velocity->maxVelocity.y;
        }
        else if (input == D_W_KEY) {
          velocity->currentVelocity.x = velocity->maxVelocity.x;
          velocity->currentVelocity.y = -velocity->maxVelocity.y;
        }
        else if (input == D_S_KEY) {
          velocity->currentVelocity.x = velocity->maxVelocity.x;
          velocity->currentVelocity.y = velocity->maxVelocity.y;
        }
      }
    }
  }
}

void MovementSystem::ownUpdate() {
  for (auto entityWithRender : mRenderComponentManager->getAllEntitiesWithComponent())
  {
    VelocityComponent* velocity = mVelocityComponentManager->getComponent(entityWithRender.first);
    if (velocity && ((velocity->currentVelocity.x != 0 && velocity->maxVelocity.x > 0) || (velocity->currentVelocity.y != 0 && velocity->maxVelocity.y > 0))) {
      sf::Vector2f currentPosition = entityWithRender.second->sprite.getPosition();
      mCollisionSystem->handleAnyCollision(entityWithRender.first, currentPosition.x + velocity->currentVelocity.x, currentPosition.y + velocity->currentVelocity.y);
      move(entityWithRender.second, velocity);
    }
  }
}

void MovementSystem::move(RenderComponent* position, VelocityComponent* velocity) {
  sf::Vector2f pos = position->sprite.getPosition();
  position->sprite.setPosition(pos.x + velocity->currentVelocity.x, pos.y + velocity->currentVelocity.y);
  if (velocity->moveOnce) {
    velocity->currentVelocity.x = 0;
    velocity->currentVelocity.y = 0;
  }
}
