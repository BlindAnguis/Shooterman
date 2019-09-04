#include "Engine.h"
#include "../../Common/Textures.h"
#include "../../Common/Messages/PlayerDataMessage.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../Common/Interfaces.h"

Engine::Engine(std::shared_ptr<MessageHandler> messageHandler) :
  mEntityManager(&EntityManager::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mMapCreator(MapCreator(mEntityManager, mGridSystem)),
  mMessageHandler(messageHandler)
{
  mName = "SERVER: ENGINE";
  mInputSystem->attach(mMovementSystem);
  mInputSystem->setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { });
  setupDebugMessages("Server", "Engine", mMessageHandler);
}

Engine::Engine(std::array<std::array<Textures, 32>, 32> gameMap, std::shared_ptr<MessageHandler> messageHandler, std::shared_ptr<NetworkSystem> networkHandler) :
  mEntityManager(&EntityManager::get()),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mVelocityComponentManager(&ComponentManager<VelocityComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mAnimationComponentManager(&ComponentManager<AnimationComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mClockComponentManager(&ComponentManager<ClockComponent>::get()),
  mPlayerComponentManager(&ComponentManager<PlayerComponent>::get()),
  mHealthChangerComponentManager(&ComponentManager<HealthChangerComponent>::get()),
  mMapCreator(MapCreator(mEntityManager, mGridSystem)),
  mGameMap(gameMap),
  mMessageHandler(messageHandler),
  mGridSystem(std::make_shared<GridSystem>()),
  mDeleteSystem(std::make_shared<DeleteSystem>()),
  mAnimationSystem(std::make_shared<AnimationSystem>()),
  mClockSystem(std::make_shared<ClockSystem>()),
  mHealthSystem(std::make_shared<HealthSystem>(messageHandler)),
  mInputSystem(std::make_shared<InputSystem>(messageHandler)),
  mNetworkSystem(networkHandler),
  mRenderSystem(std::make_shared<RenderSystem>(messageHandler, networkHandler)),
  mManaSystem(std::make_shared<ManaSystem>())
{
  mCollisionSystem = std::make_shared<CollisionSystem>(messageHandler, mDeleteSystem);
  mEntityCreatorSystem = std::make_shared<EntityCreatorSystem>(messageHandler, mDeleteSystem, mGridSystem, mManaSystem);
  mPickupSystem = std::make_shared<PickupSystem>(messageHandler, mEntityCreatorSystem, mManaSystem);
  mMovementSystem = std::make_shared<MovementSystem>(messageHandler, mCollisionSystem, mGridSystem);

  mName = "SERVER: ENGINE";
  mInputSystem->attach(mMovementSystem);
  mInputSystem->setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) {
    auto player = mPlayerComponentManager->getComponent(entityId);
    auto attackClock = mClockComponentManager->getComponent(entityId);
    if (attackClock->clock.getElapsedTime().asMilliseconds() >= player->attackSpeed) {
      //TRACE_INFO("Setting attack mouse info to, x: " << mousePosition.x << ", y: " << mousePosition.y);
      player->nextAttackMousePosition = mousePosition;
      if (player->superAttacks > 0) {
        TRACE_DEBUG2("----------------- START SUPER ATTACK -------------------------");
        player->state = PlayerState::SuperAttacking;
        player->invinsible = true;
        auto hcc = ComponentManager<HealthChangerComponent>::get().addComponent(entityId);
        hcc->healthChange = -30;
      } else {
        TRACE_DEBUG2("----------------- START REGULAR ATTACK -------------------------");
        player->state = PlayerState::Attacking;
      }
    }
  });
  setupDebugMessages("Server", "Engine", mMessageHandler);
}

Engine::~Engine() {
  for (auto entity : mEntityManager->getAllEntities()) {
    destroyEntity(entity.second->id);
  }
  ComponentManager<ComponentType>::get().clearManager();
  ComponentManager<PlayerState>::get().clearManager();
  ComponentManager<PlayerClass>::get().clearManager();
  ComponentManager<PickupType>::get().clearManager();
  ComponentManager<PositionComponent>::get().clearManager();
  ComponentManager<RenderComponent>::get().clearManager();
  ComponentManager<VelocityComponent>::get().clearManager();
  ComponentManager<CollisionComponent>::get().clearManager();
  ComponentManager<AnimationComponent>::get().clearManager();
  ComponentManager<HealthComponent>::get().clearManager();
  ComponentManager<ManaComponent>::get().clearManager();
  ComponentManager<StaminaComponent>::get().clearManager();
  ComponentManager<HealthChangerComponent>::get().clearManager();
  ComponentManager<ClockComponent>::get().clearManager();
  ComponentManager<PlayerComponent>::get().clearManager();
  ComponentManager<PickupComponent>::get().clearManager();
  teardownDebugMessages();
}

void Engine::shutDown() {
  mGridSystem->resetGridSystem();
  mCollisionSystem->resetSystem();
  teardownDebugMessages();
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_PLAYER_DATA, &mPlayerDataSubscriber);
}

void Engine::update() {
  handleDebugMessages();
  //TRACE_DEBUG1("Frame begins!!");
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
        TRACE_DEBUG2("----------------- ATTACK FINISHED -------------------------");
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
  TRACE_DEBUG1("ResetTime: " << resetTime << "us, InputTime: " << inputTime << "us, MovementTime: " << movementTime << "us, PickupTime: " << pickupTime << "us, HealthTime: " << healthTime << "us, AnimationTime: " << animationTime << "us, RenderTime: " << renderTime << "us, DeleteTime: " << deleteTime << "us, TotalTime: " << totalTime << "us");
}

void Engine::createPlayers() {
  auto spawnPositions = mMapCreator.getSpawnPositions();
  int index = 0;

  for (auto it = mConnectedClients->begin(); it != mConnectedClients->end(); ++it) {

    auto spawnPosition = spawnPositions[index];

    switch (it->second->getPlayerClass()) {
      case PlayerClass::Mage:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerMage, sf::Vector2f(spawnPosition.first, spawnPosition.second), {}));
        break;
      case PlayerClass::Knight:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerKnight, sf::Vector2f(spawnPosition.first, spawnPosition.second), {}));
        break;
      case PlayerClass::Spearman:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerSpearman, sf::Vector2f(spawnPosition.first, spawnPosition.second), {}));
        break;
      case PlayerClass::Archer:
        it->second->setEntity(mEntityCreatorSystem->createEntity(EntityType::PlayerArcher, sf::Vector2f(spawnPosition.first, spawnPosition.second), {}));
        //it->second->setEntity(mEntityCreator.createPlayer(PlayerClass::Archer, sf::Vector2f(xPos, 100)));
        break;
      default:
        break;
    }
    ++index;
  }
  
  mMessageHandler->subscribeTo(Interfaces::SERVER_PLAYER_DATA, &mPlayerDataSubscriber);
}

void Engine::createMap() {
  mMapCreator.createMap(mGameMap);
}

void Engine::setMap(std::array<std::array<Textures, 32>, 32> map) {
  mGameMap = map;
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
  PlayerDataMessage pdm;
  for (auto player : *mConnectedClients) {
    PlayerData playerData;
    int playerId = player.second->getEntity()->id;

    playerData.username = player.second->getUsername();

    auto renderComponent = mRenderComponentManager->getComponent(playerId);
    if (renderComponent) {
      sf::Vector2f position = renderComponent->sprite.getPosition();
      playerData.position = position;

      playerData.globalBounds.width = renderComponent->sprite.getGlobalBounds().width;
      playerData.globalBounds.height = renderComponent->sprite.getGlobalBounds().height;
    } else {
      TRACE_ERROR("Player with id: " << playerId << " doesn't have a renderComponent");
    }

    auto healthComponent = mHealthComponentManager->get().getComponent(playerId);
    if (healthComponent) {
      playerData.hasHealth = true;
      playerData.currentHealth = healthComponent->currentHealth;
      playerData.maxHealth = healthComponent->maxHealth;
    } else {
      TRACE_ERROR("Player with id: " << playerId << " doesn't have a healtComponent!");
    }

    auto manaComponent = ComponentManager<ManaComponent>::get().getComponent(playerId);
    if (manaComponent) {
      playerData.hasMana = true;
      playerData.currentMana = manaComponent->currentMana;
      playerData.maxMana = manaComponent->maxMana;
    }

    auto staminaComponent = ComponentManager<StaminaComponent>::get().getComponent(playerId);
    if (staminaComponent) {
      playerData.hasStamina = true;
      playerData.currentStamina = staminaComponent->currentStamina;
      playerData.maxStamina = staminaComponent->maxStamina;
    }

    pdm.addPlayerData(playerData);
  }
  mPlayerDataSubscriber.reverseSendMessage(pdm.pack());
}