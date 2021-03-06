#include "MovementSystem.h"
#include "../../../Common/KeyBindings.h"

MovementSystem::MovementSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<CollisionSystem> collisionSystem, std::shared_ptr<GridSystem> gridSystem) :
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mCollisionSystem(collisionSystem),
  mGridSystem(gridSystem),
  mEntityManager(&EntityManager::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mMessageHandler(messageHandler)
{}

MovementSystem::~MovementSystem() { TRACE_DEBUG1("Enter Destructor"); }

void MovementSystem::update(InputMessage inputMessage)
{
  //TRACE_INFO("Updating movement");
  std::uint32_t input = inputMessage.getKeyboardBitMask();
  sf::Vector2i mousePos = inputMessage.getMousePosition();
  //std::cout << "[SERVER: MOVEMENT_SYSTEM] update called with input: " << input << std::endl;
  Entity* e = mPlayersMap->at(inputMessage.getId())->getEntity();
  if (e && mRenderComponentManager->hasComponent(e->id)) {
    auto velocity = mVelocityComponentManager->getComponent(e->id);
    auto animation = mAnimationComponentManager->getComponent(e->id);
    auto spritePosition = mRenderComponentManager->getComponent(e->id);
    auto player = mPlayerComponentManager->getComponent(e->id);

    // Calculate the angle between the sprite and the mouse
    float angle = atan2(spritePosition->sprite.getPosition().y - mousePos.y,
      spritePosition->sprite.getPosition().x - mousePos.x) * (180 / 3.1415f) - 90; // Remove 90 degrees to compensate for rotation...
    if (angle < 0) {
      angle += 360;
    }

    if (player->state != PlayerState::SuperAttacking) {
      if (angle >= 315 || angle < 45) {
        animation->currentAnimation = AnimationType::IdleUp;
      }
      else if (angle >= 45 && angle < 135) {
        animation->currentAnimation = AnimationType::IdleRight;
      }
      else if (angle >= 135 && angle < 225) {
        animation->currentAnimation = AnimationType::IdleDown;
      }
      else if (angle >= 225 && angle < 315) {
        animation->currentAnimation = AnimationType::IdleLeft;
      }
    }

    bool velocityUpdated = false;
    if (velocity && player->state != PlayerState::Attacking) {
      if (input & D_KEY) {
        velocityUpdated = true;
        velocity->currentVelocity.x += velocity->maxVelocity.x;
      }
      if (input & A_KEY) {
        velocityUpdated = true;
        velocity->currentVelocity.x -= velocity->maxVelocity.x;
      }
      if (input & W_KEY) {
        velocityUpdated = true;
        velocity->currentVelocity.y -= velocity->maxVelocity.y;
      }
      if (input & S_KEY) {
        velocityUpdated = true;
        velocity->currentVelocity.y += velocity->maxVelocity.y;
      }
    }

    if (velocityUpdated && player->state != PlayerState::SuperAttacking) {
      //animation->animationFrame = 0;
      if (animation->currentAnimation == AnimationType::IdleUp) {
        animation->currentAnimation = AnimationType::RunningUp;
      }
      else if (animation->currentAnimation == AnimationType::IdleDown) {
        animation->currentAnimation = AnimationType::RunningDown;
      }
      else if (animation->currentAnimation == AnimationType::IdleLeft) {
        animation->currentAnimation = AnimationType::RunningLeft;
      }
      else if (animation->currentAnimation == AnimationType::IdleRight) {
        animation->currentAnimation = AnimationType::RunningRight;
      }
    } else if(player->state == PlayerState::Attacking) {
      if (animation->currentAnimation == AnimationType::IdleUp) {
        animation->currentAnimation = AnimationType::AttackingUp;
      }
      else if (animation->currentAnimation == AnimationType::IdleDown) {
        animation->currentAnimation = AnimationType::AttackingDown;
      }
      else if (animation->currentAnimation == AnimationType::IdleLeft) {
        animation->currentAnimation = AnimationType::AttackingLeft;
      }
      else if (animation->currentAnimation == AnimationType::IdleRight) {
        animation->currentAnimation = AnimationType::AttackingRight;
      }
    } else if (player->state == PlayerState::SuperAttacking) {
      if (animation->currentAnimation == AnimationType::IdleUp || animation->currentAnimation == AnimationType::RunningUp) {
        animation->currentAnimation = AnimationType::SuperAttackingUp;
      }
      else if (animation->currentAnimation == AnimationType::IdleDown || animation->currentAnimation == AnimationType::RunningDown) {
        animation->currentAnimation = AnimationType::SuperAttackingDown;
      }
      else if (animation->currentAnimation == AnimationType::IdleLeft || animation->currentAnimation == AnimationType::RunningLeft) {
        animation->currentAnimation = AnimationType::SuperAttackingLeft;
      }
      else if (animation->currentAnimation == AnimationType::IdleRight || animation->currentAnimation == AnimationType::RunningRight) {
        animation->currentAnimation = AnimationType::SuperAttackingRight;
      }
    }
  }
}

void MovementSystem::ownUpdate() {
  for (auto entityWithCollision : mCollisionComponentManager->getAllEntitiesWithComponent())
  {
    auto velocityComponent = mVelocityComponentManager->getComponent(entityWithCollision.first);
    auto playerComponent = ComponentManager<PlayerComponent>::get().getComponent(entityWithCollision.first);
    if (playerComponent) {

    }
    if (
        (velocityComponent && ((velocityComponent->currentVelocity.x != 0 && velocityComponent->maxVelocity.x > 0) || (velocityComponent->currentVelocity.y != 0 && velocityComponent->maxVelocity.y > 0))) ||
        (playerComponent && playerComponent->state == PlayerState::SuperAttacking))
    {
      auto renderComponent = mRenderComponentManager->getComponent(entityWithCollision.first);
      sf::Vector2f currentPosition = renderComponent->sprite.getPosition();
      mCollisionSystem->handleAnyCollision(entityWithCollision.first, currentPosition.x + velocityComponent->currentVelocity.x, currentPosition.y + velocityComponent->currentVelocity.y, mGridSystem);
      move(entityWithCollision.first, renderComponent, velocityComponent);
    }
  }
}

void MovementSystem::move(int entityId, std::shared_ptr<RenderComponent> position, std::shared_ptr<VelocityComponent> velocity) {
  sf::Vector2f pos = position->sprite.getPosition();
  position->sprite.setPosition(pos.x + velocity->currentVelocity.x, pos.y + velocity->currentVelocity.y);
  if (velocity->moveOnce) {
    velocity->currentVelocity.x = 0;
    velocity->currentVelocity.y = 0;
  }
  mGridSystem->moveEntity(entityId, (sf::Vector2i)pos, (sf::Vector2i)position->sprite.getPosition());
}
