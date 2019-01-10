#include "MovementSystem.h"
#include "../../../Common/KeyBindings.h"
#include <iostream>

MovementSystem::MovementSystem() {}

MovementSystem::MovementSystem(
  ComponentManager<VelocityComponent>* velocityComponentManager,
  ComponentManager<RenderComponent>* renderComponentManager,
  CollisionSystem* collisionSystem,
  EntityManager* entityManager,
  ComponentManager<AnimationComponent>* animationComponentManager
) :
  mVelocityComponentManager(velocityComponentManager),
  mRenderComponentManager(renderComponentManager),
  mCollisionSystem(collisionSystem),
  mEntityManager(entityManager),
  mAnimationComponentManager(animationComponentManager)
{}

MovementSystem::~MovementSystem() {}

void MovementSystem::update(InputMessage inputMessage, int ID)
{
  std::uint32_t input = inputMessage.getKeyboardBitMask();
  sf::Vector2i mousePos = inputMessage.getMousePosition();
  //std::cout << "[SERVER: MOVEMENT_SYSTEM] update called with input: " << input << std::endl;
  Entity* e = mPlayersMap->at(inputMessage.getId()).second;
  if (e && mRenderComponentManager->hasComponent(e->id)) {
    VelocityComponent* velocity = mVelocityComponentManager->getComponent(e->id);
    AnimationComponent* animation = mAnimationComponentManager->getComponent(e->id);
    RenderComponent* spritePosition = mRenderComponentManager->getComponent(e->id);

    // Calculate the angle between the sprite and the mouse
    float angle = atan2(spritePosition->sprite.getPosition().y - mousePos.y,
      spritePosition->sprite.getPosition().x - mousePos.x) * (180 / 3.1415f) + 90; // Add 90 degrees to compensate for rotation...
    if (angle < 0) {
      angle += 360;
    }
    spritePosition->sprite.setRotation(angle);

    if (velocity) {
      if (input & D_KEY) {
        velocity->currentVelocity.x += velocity->maxVelocity.x;
        animation->animation = Animations::RunningRight;
      }
      if (input & A_KEY) {
        velocity->currentVelocity.x -= velocity->maxVelocity.x;
       animation->animation = Animations::RunningLeft;
      }
      if (input & W_KEY) {
        velocity->currentVelocity.y -= velocity->maxVelocity.y;
        animation->animation = Animations::RunningDown;
      }
      if (input & S_KEY) {
        velocity->currentVelocity.y += velocity->maxVelocity.y;
        animation->animation = Animations::RunningDown;
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
