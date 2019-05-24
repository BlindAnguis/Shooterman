#include "Engine.h"
#include "../../Common/Textures.h"
#include "../../Common/Messages/PlayerDataMessage.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"
#include <stdlib.h>
#include <time.h>

Engine::Engine() :
  mInputSystem(&InputSystem::get()),
  mEntityManager(&EntityManager::get()),
  mNetworkSystem(&NetworkSystem::get()),
  mDeleteSystem(&DeleteSystem::get()),
  mGridSystem(&GridSystem::get()),
  mClockSystem(&ClockSystem::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mCollisionSystem(&CollisionSystem::get()),
  mMovementSystem(&MovementSystem::get()),
  mRenderSystem(&RenderSystem::get()),
  mAnimationSystem(&AnimationSystem::get()),
  mHealthSystem(&HealthSystem::get()),
  mEntityCreatorSystem(&EntityCreatorSystem::get()),
  mPickupSystem(&PickupSystem::get()),
  mMapCreator(MapCreator(mEntityManager, mGridSystem))
{
  mName = "SERVER: ENGINE";
  mInputSystem->attach(mMovementSystem);
  mInputSystem->setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { });
  srand((int)time(0));
}

Engine::Engine(std::array<std::array<int, 32>, 32> gameMap) :
  mInputSystem(&InputSystem::get()),
  mEntityManager(&EntityManager::get()),
  mNetworkSystem(&NetworkSystem::get()),
  mDeleteSystem(&DeleteSystem::get()),
  mGridSystem(&GridSystem::get()),
  mClockSystem(&ClockSystem::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mCollisionSystem(&CollisionSystem::get()),
  mMovementSystem(&MovementSystem::get()),
  mRenderSystem(&RenderSystem::get()),
  mAnimationSystem(&AnimationSystem::get()),
  mHealthSystem(&HealthSystem::get()),
  mEntityCreatorSystem(&EntityCreatorSystem::get()),
  mPickupSystem(&PickupSystem::get()),
  mMapCreator(MapCreator(mEntityManager, mGridSystem)),
  mGameMap(gameMap)
{
  mName = "SERVER: ENGINE";
  mInputSystem->attach(mMovementSystem);
  mInputSystem->setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {
    auto player = mPlayerComponentManager->getComponent(entityId);
    auto attackClock = mClockComponentManager->getComponent(entityId);
    if (attackClock->clock.getElapsedTime().asMilliseconds() >= player->attackSpeed) {
      //TRACE_INFO("Setting attack mouse info to, x: " << mousePosition.x << ", y: " << mousePosition.y);
      player->state = PlayerState::Attacking;
      player->nextAttackMousePosition = mousePosition;
      if (player->superAttacks > 0) {
        player->state = PlayerState::SuperAttacking;
        player->invinsible = true;
        auto hcc = ComponentManager<HealthChangerComponent>::get().addComponent(entityId);
        hcc->healthChange = -30;
      }
    }
  });
  srand((int)time(0));
}

Engine::~Engine() {
  for (auto entity : mEntityManager->getAllEntities()) {
    destroyEntity(entity.second->id);
  }
  mRenderSystem->resetSystem();
}

void Engine::shutDown() {
  RemoveDebugButtonMessage rdbm(mDebugMenuSubscriber.getId());
  sf::Packet packet = rdbm.pack();
  mDebugMenuSubscriber.reverseSendMessage(packet);

  MessageHandler::get().unsubscribeTo("ServerPlayerData", &mPlayerDataSubscriber);
  MessageHandler::get().unsubscribeTo("ServerDebugMenu", &mDebugMenuSubscriber);
}

void Engine::update() {
  handleDebugMessage();
  //TRACE_DEBUG("Frame begins!!");
  sf::Clock c;
  // Reset
  //TRACE_INFO("Reset");
  mCollisionSystem->resetCollisionInformation();
  sf::Int64 resetTime = c.getElapsedTime().asMicroseconds();

  for (auto player : mPlayerComponentManager->getAllEntitiesWithComponent()) {
    if (player.second->state == PlayerState::Attacking || player.second->state == PlayerState::SuperAttacking) {
      //TRACE_INFO("Player is attacking");
      auto animation = mAnimationComponentManager->getComponent(player.first);
      auto &currentAnimation = animation->animations.find(animation->currentAnimation)->second;
      if (currentAnimation.hasBeenPlayedOnce()) {
        //TRACE_INFO("Attack animation has been played, player state set to idle and animation is set to idleDown and animation is reset");
        player.second->state = PlayerState::Idle;
        animation->currentAnimation = AnimationType::IdleDown;
        currentAnimation.reset();
      }
    }
  }
  c.restart();


  // Update
  mInputSystem->handleInput();
  sf::Int64 inputTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mMovementSystem->ownUpdate();
  sf::Int64 movementTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mPickupSystem->update();
  sf::Int64 pickupTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mHealthSystem->update();
  sf::Int64 healthTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mAnimationSystem->update();
  sf::Int64 animationTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mEntityCreatorSystem->update();
  sf::Int64 entityCreatorTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mRenderSystem->render(mConnectedClients);
  sf::Int64 renderTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mClockSystem->update();

  // Remove dead entities
  //for (auto entity : mCollisionComponentManager.getAllEntitiesWithComponent()) {
  //  if (!mPlayerComponentManager.hasComponent(entity.first) && entity.second->collided && entity.second->destroyOnCollision) {
  //    destroyEntity(entity.first);
  //  }
  //}
  for (auto entityId : mDeleteSystem->getEntities()) {
    destroyEntity(entityId);
  }

  for (auto entity : mHealthComponentManager->getAllEntitiesWithComponent()) {
    if (!mPlayerComponentManager->hasComponent(entity.first) && !entity.second->isAlive) {
      destroyEntity(entity.first);
    }
  }

  /*
  for (auto entity : mRenderComponentManager.getAllEntitiesWithComponent()) {
    if (mGridSystem.isOutOfBounds((sf::Vector2i)entity.second->sprite.getPosition())) {
      destroyEntity(entity.first);
    }
  }
  */
 
  for (auto entity : mPlayerComponentManager->getAllEntitiesWithComponent()) {
    if (!mHealthComponentManager->getComponent(entity.first)->isAlive) {
      auto entityRenderComponent = mRenderComponentManager->getComponent(entity.first);
      mGridSystem->removeEntity(entity.first, (sf::Vector2i)entityRenderComponent->sprite.getPosition());
      mCollisionComponentManager->removeComponent(entity.first);
      mVelocityComponentManager->removeComponent(entity.first);
      mHealthChangerComponentManager->removeComponent(entity.first);
      mClockComponentManager->removeComponent(entity.first);
    }
  }

  sf::Int64 deleteTime = c.getElapsedTime().asMicroseconds();
  c.restart();

  sf::Int64 totalTime = resetTime + inputTime + movementTime + healthTime + animationTime + renderTime + deleteTime;
  TRACE_DEBUG("ResetTime: " << resetTime << "us, InputTime: " << inputTime << "us, MovementTime: " << movementTime << "us, PickupTime: " << pickupTime << "us, HealthTime: " << healthTime << "us, AnimationTime: " << animationTime << "us, RenderTime: " << renderTime << "us, DeleteTime: " << deleteTime << "us, TotalTime: " << totalTime << "us");
}

void Engine::createPlayers() {
  float xPos = 100;
  for (auto it = mConnectedClients->begin(); it != mConnectedClients->end(); ++it) {
    int id = (rand() % 1);
    switch (id)
    {
      case 0:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerMage, sf::Vector2f(xPos, 100), {}));
        break;
      case 1:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerKnight, sf::Vector2f(xPos, 100), {}));
        break;
      case 2:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerSpearman, sf::Vector2f(xPos, 100), {}));
        break;
      case 3:
        //it->second->setEntity(mEntityCreator.createPlayer(PlayerClass::Archer, sf::Vector2f(xPos, 100)));
        break;
      default:
        break;
    }
    xPos += 100;
  }

  MessageHandler::get().subscribeTo("ServerPlayerData", &mPlayerDataSubscriber);
  MessageHandler::get().subscribeTo("ServerDebugMenu", &mDebugMenuSubscriber);
  AddDebugButtonMessage debMess(mDebugMenuSubscriber.getId(), "Engine debug traces", "Server");
  mDebugMenuSubscriber.reverseSendMessage(debMess.pack());
}

void Engine::createMap() {
  mMapCreator.createMap(mGameMap);
}

void Engine::destroyEntity(int entityId) {
  auto entityRenderComponent = mRenderComponentManager->getComponent(entityId);
  mGridSystem->removeEntity(entityId, (sf::Vector2i)entityRenderComponent->sprite.getPosition());
  mRenderComponentManager->removeComponent(entityId);
  mCollisionComponentManager->removeComponent(entityId);
  mVelocityComponentManager->removeComponent(entityId);
  mAnimationComponentManager->removeComponent(entityId);
  mHealthComponentManager->removeComponent(entityId);
  mHealthChangerComponentManager->removeComponent(entityId);
  mClockComponentManager->removeComponent(entityId);
  ComponentManager<PickupComponent>::get().removeComponent(entityId);
  for (auto client : *mConnectedClients) {
    if (client.second->getEntity() && client.second->getEntity()->id == entityId) {
      client.second->setEntity(nullptr);
    }
  }
  mEntityManager->destroyEntity(entityId);
}

void Engine::collectPlayerData() {
  for (auto player : *mConnectedClients) {
    PlayerDataMessage pdm(player.first);

    int playerId = player.second->getEntity()->id;

    auto renderComponent = mRenderComponentManager->getComponent(playerId);
    if (renderComponent) {
      sf::Vector2f position = renderComponent->sprite.getPosition();
      pdm.setPosition(position);

      GlobalBounds globalBounds;
      globalBounds.width = renderComponent->sprite.getGlobalBounds().width;
      globalBounds.height = renderComponent->sprite.getGlobalBounds().height;
      pdm.setGlobalBounds(globalBounds);
    } else {
      TRACE_ERROR("Player with id: " << playerId << " doesn't have a renderComponent");
    }

    auto healthComponent = mHealthComponentManager->get().getComponent(playerId);
    if (healthComponent) {
      pdm.setCurrentHealth(healthComponent->currentHealth);
      pdm.setMaxHealth(healthComponent->maxHealth);
    } else {
      TRACE_ERROR("Player with id: " << playerId << " doesn't have a healtComponent!");
    }

    auto manaComponent = ComponentManager<ManaComponent>::get().getComponent(playerId);
    if (manaComponent) {
      pdm.setCurrentMana(manaComponent->currentMana);
      pdm.setMaxMana(manaComponent->maxMana);
    }

    mPlayerDataSubscriber.reverseSendMessage(pdm.pack());
  }
}

void Engine::handleDebugMessage() {
  std::queue<sf::Packet> debugMessageQueue = mDebugMenuSubscriber.getMessageQueue();
  sf::Packet debugMessage;
  while (!debugMessageQueue.empty()) {
    debugMessage = debugMessageQueue.front();
    debugMessageQueue.pop();
    TRACE_DEBUG("Toggle debug traces");
    mDebugEnabled = !mDebugEnabled;
    TRACE_DEBUG("Toggle debug traces");
  }
}