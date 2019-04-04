#include "Engine.h"
#include "../../Common/Textures.h"
#include "../../Common/Messages/PlayerDataMessage.h"
#include <stdlib.h>
#include <time.h>

Engine::Engine(std::shared_ptr<NetworkSystem> networkSystem) :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mNetworkSystem(networkSystem),
  mDeleteSystem(DeleteSystem()),
  mGridSystem(GridSystem()),
  mClockSystem(ClockSystem()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mDamageComponentManager(&ComponentManager<DamageComponent>::get()),
  mCollisionSystem(CollisionSystem(&mDeleteSystem)),
  mMovementSystem(MovementSystem(&mCollisionSystem, &mGridSystem, &mEntityManager)),
  mRenderSystem(RenderSystem(mNetworkSystem)),
  mAnimationSystem(AnimationSystem()),
  mHealthSystem(HealthSystem()),
  mEntityCreator(EntityCreator(&mEntityManager, &mGridSystem, &mDeleteSystem)),
  mPickupSystem(PickupSystem(&mEntityCreator)),
  mMapCreator(MapCreator(&mEntityManager, &mGridSystem))
{
  mName = "SERVER: ENGINE";
  mInputSystem.attach(&mMovementSystem);
  mInputSystem.setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { });
  srand((int)time(0));
}

Engine::Engine(std::array<std::array<int, 32>, 32> gameMap, std::shared_ptr<NetworkSystem> networkSystem) :
  mEntityManager(EntityManager()),
  mNetworkSystem(networkSystem),
  mDeleteSystem(DeleteSystem()),
  mInputSystem(InputSystem()),
  mGridSystem(GridSystem()),
  mClockSystem(ClockSystem()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mDamageComponentManager(&ComponentManager<DamageComponent>::get()),
  mCollisionSystem(CollisionSystem(&mDeleteSystem)),
  mMovementSystem(MovementSystem(&mCollisionSystem, &mGridSystem, &mEntityManager)),
  mRenderSystem(RenderSystem(networkSystem)),
  mAnimationSystem(AnimationSystem()),
  mHealthSystem(HealthSystem()),
  mGameMap(gameMap),
  mEntityCreator(EntityCreator(&mEntityManager, &mGridSystem, &mDeleteSystem)),
  mPickupSystem(PickupSystem(&mEntityCreator)),
  mMapCreator(MapCreator(&mEntityManager, &mGridSystem))
{
  mName = "SERVER: ENGINE";
  mInputSystem.attach(&mMovementSystem);
  mInputSystem.setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {
    auto player = mPlayerComponentManager->getComponent(entityId);
    auto attackClock = mClockComponentManager->getComponent(entityId);
    if (attackClock->clock.getElapsedTime().asMilliseconds() >= player->attackSpeed) {
      //TRACE_INFO("Setting attack mouse info to, x: " << mousePosition.x << ", y: " << mousePosition.y);
      player->nextAttackMousePosition = mousePosition;
    }
  });
  srand((int)time(0));
}

Engine::~Engine() {
}

void Engine::update() {
  //TRACE_INFO("Frame begins!!");
  sf::Clock c;
  // Reset
  //TRACE_INFO("Reset");
  mCollisionSystem.resetCollisionInformation();
  sf::Int64 resetTime = c.getElapsedTime().asMicroseconds();

  for (auto player : mPlayerComponentManager->getAllEntitiesWithComponent()) {
    if (player.second->state == PlayerState::Attacking) {
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
  mInputSystem.handleInput();
  sf::Int64 inputTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mMovementSystem.ownUpdate();
  sf::Int64 movementTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mPickupSystem.update();
  sf::Int64 pickupTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mHealthSystem.update();
  sf::Int64 healthTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mAnimationSystem.update();
  sf::Int64 animationTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mRenderSystem.render(mConnectedClients);
  sf::Int64 renderTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mClockSystem.update();

  // Remove dead entities
  //for (auto entity : mCollisionComponentManager.getAllEntitiesWithComponent()) {
  //  if (!mPlayerComponentManager.hasComponent(entity.first) && entity.second->collided && entity.second->destroyOnCollision) {
  //    destroyEntity(entity.first);
  //  }
  //}
  for (auto entityId : mDeleteSystem.getEntities()) {
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
      mGridSystem.removeEntity(entity.first, (sf::Vector2i)entityRenderComponent->sprite.getPosition());
      mCollisionComponentManager->removeComponent(entity.first);
      mVelocityComponentManager->removeComponent(entity.first);
      mDamageComponentManager->removeComponent(entity.first);
      mClockComponentManager->removeComponent(entity.first);
    }
  }

  sf::Int64 deleteTime = c.getElapsedTime().asMicroseconds();
  c.restart();

  sf::Int64 totalTime = resetTime + inputTime + movementTime + healthTime + animationTime + renderTime + deleteTime;
  //TRACE_INFO("ResetTime: " << resetTime << "us, InputTime: " << inputTime << "us, MovementTime: " << movementTime << "us, PickupTime: " << pickupTime << "us, HealthTime: " << healthTime << "us, AnimationTime: " << animationTime << "us, RenderTime: " << renderTime << "us, DeleteTime: " << deleteTime << "us, TotalTime: " << totalTime << "us");
}

InputSystem* Engine::getInputSystem() {
  return &mInputSystem;
}

MovementSystem* Engine::getMovementSystem() {
  return &mMovementSystem;
}

EntityManager* Engine::getEntityManager() {
  return &mEntityManager;
}

void Engine::createPlayers() {
  float xPos = 100;
  for (auto it = mConnectedClients->begin(); it != mConnectedClients->end(); ++it) {
    int id = rand() % 1;
    switch (id)
    {
      case 0:
        it->second->setEntity(mEntityCreator.createPlayer(PlayerClass::Mage, sf::Vector2f(xPos, 100)));
        break;
      case 1:
        it->second->setEntity(mEntityCreator.createPlayer(PlayerClass::Knight, sf::Vector2f(xPos, 100)));
        break;
      case 2:
        it->second->setEntity(mEntityCreator.createPlayer(PlayerClass::Spearman, sf::Vector2f(xPos, 100)));
        break;
      case 3:
        it->second->setEntity(mEntityCreator.createPlayer(PlayerClass::Archer, sf::Vector2f(xPos, 100)));
        break;
      default:
        break;
    }
    xPos += 100;
  }

  MessageHandler::get().subscribeTo("ServerPlayerData", &mPlayerDataSubscriber);
}

void Engine::createMap() {
  mMapCreator.createMap(mGameMap);
}

void Engine::destroyEntity(int entityId) {
  auto entityRenderComponent = mRenderComponentManager->getComponent(entityId);
  mGridSystem.removeEntity(entityId, (sf::Vector2i)entityRenderComponent->sprite.getPosition());
  mRenderComponentManager->removeComponent(entityId);
  mCollisionComponentManager->removeComponent(entityId);
  mVelocityComponentManager->removeComponent(entityId);
  mAnimationComponentManager->removeComponent(entityId);
  mHealthComponentManager->removeComponent(entityId);
  mDamageComponentManager->removeComponent(entityId);
  mClockComponentManager->removeComponent(entityId);
  ComponentManager<PickupComponent>::get().removeComponent(entityId);
  for (auto client : *mConnectedClients) {
    if (client.second->getEntity() && client.second->getEntity()->id == entityId) {
      client.second->setEntity(nullptr);
    }
  }
  mEntityManager.destroyEntity(entityId);
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