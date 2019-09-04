#include "CollisionSystem.h"

#include "../../../Common/Interfaces.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"
#include "../../../Common/Messages/RemoveDebugButtonMessage.h"

CollisionSystem::CollisionSystem(std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<DeleteSystem> deleteSystem)
  :
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mDeleteSystem(deleteSystem),
  mMessageHandler(messageHandler) {
  mName = "ENGINE: COLLISION_SYSTEM";
}

CollisionSystem::~CollisionSystem() { TRACE_DEBUG1("Enter Destructor"); }

void CollisionSystem::handleAnyCollision(int causingColliderEntityId, float newXPos, float newYPos, std::shared_ptr<GridSystem> gridSystem) {
  if (!mDebugMenuSubscribed) {
    mDebugMenuSubscribed = mMessageHandler->subscribeTo(Interfaces::SERVER_DEBUG_MENU, &mDebugMenuSubscriber);
    if (mDebugMenuSubscribed) {
      AddDebugButtonMessage debMess(mDebugMenuSubscriber.getId(), "Collision debug traces", "Server");
      mDebugMenuSubscriber.reverseSendMessage(debMess.pack());
    }
  }
  handleDebugMessage();

  auto causingColliderMovingComponent = mRenderComponentManager->getComponent(causingColliderEntityId);
  auto causingColliderCollisionComponent = mCollisionComponentManager->getComponent(causingColliderEntityId);
  if (causingColliderCollisionComponent) {
    sf::Vector2f oldPosition = causingColliderMovingComponent->sprite.getPosition();
    causingColliderMovingComponent->sprite.setPosition(newXPos, newYPos);
    
    TRACE_DEBUG1("No of nearEntities: " << gridSystem->getNearEntities((sf::Vector2i)causingColliderMovingComponent->sprite.getPosition()).size());

    for (auto affectedCollideeEntityId : gridSystem->getNearEntities((sf::Vector2i)causingColliderMovingComponent->sprite.getPosition())) {
      auto affectedCollideeMovingComponent = mRenderComponentManager->getComponent(affectedCollideeEntityId);
      auto affectedCollideeCollisionComponent = mCollisionComponentManager->getComponent(affectedCollideeEntityId);

      // If collision with another entity
      if (causingColliderEntityId != affectedCollideeEntityId && Collision::PixelPerfectTest(causingColliderMovingComponent->sprite, affectedCollideeMovingComponent->sprite)) {
        TRACE_DEBUG1("Entity: " << causingColliderEntityId << " collided with: " << affectedCollideeEntityId);

        handleCollision(causingColliderEntityId, causingColliderMovingComponent, affectedCollideeEntityId, affectedCollideeMovingComponent);
        causingColliderMovingComponent->sprite.setPosition(oldPosition); // Reset movement.

        auto collisionList = causingColliderCollisionComponent->collidedList;

        // Add collision if causing collider has not previously collided with affected collidee
        if (std::find(collisionList.begin(), collisionList.end(), affectedCollideeEntityId) == collisionList.end()) {
          mCollisions.emplace(causingColliderEntityId, affectedCollideeEntityId);

          causingColliderCollisionComponent->collided = true;
          causingColliderCollisionComponent->collidedList.push_back(affectedCollideeEntityId);
          if (causingColliderCollisionComponent->destroyOnCollision) {
            mDeleteSystem->addEntity(causingColliderEntityId);
          }

          affectedCollideeCollisionComponent->collided = true;
          affectedCollideeCollisionComponent->collidedList.push_back(causingColliderEntityId);
          if (affectedCollideeCollisionComponent->destroyOnCollision) {
            mDeleteSystem->addEntity(affectedCollideeEntityId);
          }
        }

        return; // Remove this to allow something to collide with several things at once;
      }
    }

    causingColliderMovingComponent->sprite.setPosition(oldPosition);
  }
}

void CollisionSystem::handleCollision(int causingColliderEntityId, std::shared_ptr<RenderComponent> causingColliderMovingComponent, int affectedCollideeEntityId, std::shared_ptr<RenderComponent> affectedCollideeMovingComponent) {
  // TODO: Check if any of the components can deal damage and so on. What should happen when the moving component touches the stillComponent.

  auto causingColliderEntityVelocity = mVelocityComponentManager->getComponent(causingColliderEntityId);
  if (causingColliderEntityVelocity) {
    causingColliderEntityVelocity->currentVelocity.x = 0;
    causingColliderEntityVelocity->currentVelocity.y = 0;
  }
}

void CollisionSystem::resetCollisionInformation() {
  //TRACE_INFO("Collisions made last update: " << mCollisions.size());
  for (auto& collision : mCollisions) {
    auto causingColliderEntity = mCollisionComponentManager->getComponent(collision.first);
    if (causingColliderEntity) {
      causingColliderEntity->collided = false;
      causingColliderEntity->collidedList.clear();
    }

    auto affectedCollideeEntity = mCollisionComponentManager->getComponent(collision.second);
    if (affectedCollideeEntity) {
      affectedCollideeEntity->collided = false;
      affectedCollideeEntity->collidedList.clear();
    }
  }

  mCollisions.clear();
  //TRACE_INFO("Collisions after reset: " << mCollisions.size());
}

void CollisionSystem::resetSystem() {
  RemoveDebugButtonMessage rdbm(mDebugMenuSubscriber.getId());
  sf::Packet packet = rdbm.pack();
  mDebugMenuSubscriber.reverseSendMessage(packet);
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_DEBUG_MENU, &mDebugMenuSubscriber);
  mDebugMenuSubscribed = false;
  mDebugEnabled1 = false;
}

void CollisionSystem::handleDebugMessage() {
  std::queue<sf::Packet> debugMessageQueue = mDebugMenuSubscriber.getMessageQueue();
  sf::Packet debugMessage;
  while (!debugMessageQueue.empty()) {
    debugMessage = debugMessageQueue.front();
    debugMessageQueue.pop();
    TRACE_DEBUG1("Toggle debug traces");
    mDebugEnabled1 = !mDebugEnabled1;
    TRACE_DEBUG1("Toggle debug traces");
  }
}