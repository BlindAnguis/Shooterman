#include "Engine.h"
#include "../../Common/Textures.h"

#include <stdlib.h>
#include <time.h>

Engine::Engine() :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mCollisionComponentManager(ComponentManager<CollisionComponent>()),
  mAnimationComponentManager(ComponentManager<AnimationComponent>()),
  mHealthComponentManager(ComponentManager<HealthComponent>()),
  mDamageComponentManager(ComponentManager<DamageComponent>()),
  mClockComponentManager(ComponentManager<ClockComponent>()),
  mPlayerComponentManager(ComponentManager<PlayerComponent>()),
  mGridSystem(GridSystem()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mVelocityComponentManager, &mCollisionComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionComponentManager, &mCollisionSystem, &mGridSystem, &mEntityManager, &mAnimationComponentManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mAnimationSystem(AnimationSystem(&mAnimationComponentManager, &mVelocityComponentManager, &mRenderComponentManager, &mHealthComponentManager)),
  mHealthSystem(HealthSystem(&mHealthComponentManager, &mDamageComponentManager, &mCollisionComponentManager))
{
  mName = "SERVER: ENGINE";
  mInputSystem.attach(&mMovementSystem);
  mInputSystem.setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { createBullet(entityId, input, mousePosition); });
  srand((int)time(0));
}

Engine::Engine(std::array<std::array<int, 32>, 32> gameMap) :
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mCollisionComponentManager(ComponentManager<CollisionComponent>()),
  mAnimationComponentManager(ComponentManager<AnimationComponent>()),
  mHealthComponentManager(ComponentManager<HealthComponent>()),
  mDamageComponentManager(ComponentManager<DamageComponent>()),
  mClockComponentManager(ComponentManager<ClockComponent>()),
  mPlayerComponentManager(ComponentManager<PlayerComponent>()),
  mInputSystem(InputSystem(&mHealthComponentManager)),
  mGridSystem(GridSystem()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mVelocityComponentManager, &mCollisionComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionComponentManager, &mCollisionSystem, &mGridSystem, &mEntityManager, &mAnimationComponentManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mAnimationSystem(AnimationSystem(&mAnimationComponentManager, &mVelocityComponentManager, &mRenderComponentManager, &mHealthComponentManager)),
  mHealthSystem(HealthSystem(&mHealthComponentManager, &mDamageComponentManager, &mCollisionComponentManager)),
  mGameMap(gameMap)
{
  mName = "SERVER: ENGINE";
  mInputSystem.attach(&mMovementSystem);
  mInputSystem.setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { createBullet(entityId, input, mousePosition); });
  mTextures[static_cast<int>(Textures::CharacterBandana)] = loadTexture("CharacterBandana1.png");
  mTextures[static_cast<int>(Textures::CharacterChainHat)] = loadTexture("CharacterChainHat.png");
  mTextures[static_cast<int>(Textures::CharacterChainHood)] = loadTexture("CharacterChainHood.png");
  mTextures[static_cast<int>(Textures::CharacterClothHood)] = loadTexture("CharacterClothHood.png");
  mTextures[static_cast<int>(Textures::CharacterGoldenHelmet)] = loadTexture("CharacterGoldenHelmet.png");
  mTextures[static_cast<int>(Textures::CharacterLeatherCap)] = loadTexture("CharacterLeatherCap.png");
  mTextures[static_cast<int>(Textures::CharacterMetalHelmet)] = loadTexture("CharacterMetalHelmet.png");
  mTextures[static_cast<int>(Textures::HorizontalWall1)] = loadTexture("wall1.png");
  mTextures[static_cast<int>(Textures::VerticalWall1)] = loadTexture("verticalWall1.png");
  mTextures[static_cast<int>(Textures::Bullet)] = loadTexture("Bullet.png");
  mTextures[static_cast<int>(Textures::Tombstone)] = loadTexture("Tombstone.png");
  mTextures[static_cast<int>(Textures::FloorWhole)] = loadTexture("fantasy-tileset.png");
  mTextures[static_cast<int>(Textures::FloorCracked)] = loadTexture("fantasy-tileset.png");
  mTextures[static_cast<int>(Textures::FloorSpikes)] = loadTexture("fantasy-tileset.png");
  srand((int)time(0));
}

Engine::~Engine() {
}

void Engine::update() {
  sf::Clock c;
  // Reset
  mCollisionSystem.resetCollisionInformation();
  sf::Int64 resetTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  // Update
  mInputSystem.handleInput();
  sf::Int64 inputTime = c.getElapsedTime().asMicroseconds();
  c.restart();
  mMovementSystem.ownUpdate();
  sf::Int64 movementTime = c.getElapsedTime().asMicroseconds();
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

  // Remove dead entities
  for (auto entity : mCollisionComponentManager.getAllEntitiesWithComponent()) {
    if (!mPlayerComponentManager.hasComponent(entity.first) && entity.second->collided && entity.second->destroyOnCollision) {
      destroyEntity(entity.first);
    }
  }

  for (auto entity : mHealthComponentManager.getAllEntitiesWithComponent()) {
    if (!mPlayerComponentManager.hasComponent(entity.first) && !entity.second->isAlive) {
      destroyEntity(entity.first);
    }
  }

  for (auto entity : mRenderComponentManager.getAllEntitiesWithComponent()) {
    if (mGridSystem.isOutOfBounds((sf::Vector2i)entity.second->sprite.getPosition())) {
      destroyEntity(entity.first);
    }
  }
 
  for (auto entity : mPlayerComponentManager.getAllEntitiesWithComponent()) {
    if (!mHealthComponentManager.getComponent(entity.first)->isAlive) {
      auto entityRenderComponent = mRenderComponentManager.getComponent(entity.first);
      mGridSystem.removeEntity(entity.first, (sf::Vector2i)entityRenderComponent->sprite.getPosition());
      mCollisionComponentManager.removeComponent(entity.first);
      mVelocityComponentManager.removeComponent(entity.first);
      mDamageComponentManager.removeComponent(entity.first);
      mClockComponentManager.removeComponent(entity.first);
    }
  }

  sf::Int64 deleteTime = c.getElapsedTime().asMicroseconds();
  c.restart();

  sf::Int64 totalTime = resetTime + inputTime + movementTime + healthTime + animationTime + renderTime + deleteTime;

  TRACE_INFO("ResetTime: " << resetTime << "us, InputTime: " << inputTime << "us, MovementTime: " << movementTime << "us, HealthTime: " << healthTime << "us, AnimationTime: " << animationTime << "us, RenderTime: " << renderTime << "us, DeleteTime: " << deleteTime << "us, TotalTime: " << totalTime << "us");
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
  for (auto it = mConnectedClients->begin(); it != mConnectedClients->end(); ++it) {
    it->second->setEntity(createPlayer(x, 100, 6, 6, 100));
    x += 100;
  }
}

Entity* Engine::createPlayer(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity, float maxHealth) {
  Entity* player = mEntityManager.createEntity();
  VelocityComponent* vc = mVelocityComponentManager.addComponent(player->id);
  vc->currentVelocity.x = 0;
  vc->currentVelocity.y = 0;
  vc->maxVelocity.x = xMaxVelocity;
  vc->maxVelocity.y = yMaxVelocity;
  vc->moveOnce = true;

  auto collisionComponent = mCollisionComponentManager.addComponent(player->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  int id = rand() % 7;
  //id = 0; // For testing the death animation since it only exist on this sprite for now.
  RenderComponent* rc = mRenderComponentManager.addComponent(player->id);
  switch (id)
  {
  case 0:
    rc->textureId = Textures::CharacterBandana;
    break;
  case 1:
    rc->textureId = Textures::CharacterChainHat;
    break;
  case 2:
    rc->textureId = Textures::CharacterChainHood;
    break;
  case 3:
    rc->textureId = Textures::CharacterClothHood;
    break;
  case 4:
    rc->textureId = Textures::CharacterGoldenHelmet;
    break;
  case 5:
    rc->textureId = Textures::CharacterLeatherCap;
    break;
  case 6:
  default:
    rc->textureId = Textures::CharacterMetalHelmet;
  }
  rc->texture = *mTextures[static_cast<int>(rc->textureId)];
  rc->visible = true;
  rc->isDynamic = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(14, 14, 36, 50));
  rc->sprite.setOrigin(18, 25);
  rc->sprite.setPosition(xStartPos, yStartPos);
  rc->deathTexture = *mTextures[static_cast<int>(Textures::Tombstone)];

  AnimationComponent* ac = mAnimationComponentManager.addComponent(player->id);
  ac->currentAnimation = AnimationType::IdleUp;

  Animation idleUpAnimation(rc->sprite, false);
  idleUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(14, 14, 36, 50), 140 });
  idleUpAnimation.addAnimationFrame(AnimationFrame{sf::IntRect(78, 14, 36, 50), 140});
  Animation idleDownAnimation(rc->sprite, false);
  idleDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(14, 142, 36, 50), 140 });
  idleDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(78, 142, 36, 50), 140 });
  Animation idleLeftAnimation(rc->sprite, false);
  idleLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(270, 78, 36, 50), 140 });
  idleLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(398, 78, 36, 50), 140 });
  Animation idleRightAnimation(rc->sprite, false);
  idleRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(270, 206, 36, 50), 140 });
  idleRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(398, 206, 36, 50), 140 });

  Animation runningUpAnimation(rc->sprite, false);
  for (int i = 0; i < 9; i++) {
    runningUpAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 14, 36, 50), 35 });
  }
  Animation runningDownAnimation(rc->sprite, false);
  for (int i = 0; i < 9; i++) {
    runningDownAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 142, 36, 50), 35 });
  }
  Animation runningLeftAnimation(rc->sprite, false);
  for (int i = 0; i < 9; i++) {
    runningLeftAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 78, 36, 50), 35 });
  }
  Animation runningRightAnimation(rc->sprite, false);
  for (int i = 0; i < 9; i++) {
    runningRightAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 206, 36, 50), 35 });
  }
  Animation deathAnimation(rc->sprite, true);
  for (int i = 0; i < 8; i++) {
    deathAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 270, 36, 50), 140 });
  }
  for (int i = 0; i < 8; i++) {
    deathAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 270, 36, 50), 70 });
  }
  for (int i = 0; i < 8; i++) {
    deathAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(i * 64 + 14, 270, 36, 50), 35 });
  }
  deathAnimation.addAnimationFrame(AnimationFrame{ sf::IntRect(512, 270, 64, 64), 70 });

  ac->animations.emplace(AnimationType::IdleUp, idleUpAnimation);
  ac->animations.emplace(AnimationType::IdleDown, idleDownAnimation);
  ac->animations.emplace(AnimationType::IdleLeft, idleLeftAnimation);
  ac->animations.emplace(AnimationType::IdleRight, idleRightAnimation);
  ac->animations.emplace(AnimationType::RunningUp, runningUpAnimation);
  ac->animations.emplace(AnimationType::RunningDown, runningDownAnimation);
  ac->animations.emplace(AnimationType::RunningLeft, runningLeftAnimation);
  ac->animations.emplace(AnimationType::RunningRight, runningRightAnimation);
  ac->animations.emplace(AnimationType::Death, deathAnimation);

  ClockComponent* cc = mClockComponentManager.addComponent(player->id);

  HealthComponent* hc = mHealthComponentManager.addComponent(player->id);
  hc->health = 100;
  hc->isAlive = true;

  PlayerComponent* pc = mPlayerComponentManager.addComponent(player->id);

  mGridSystem.addEntity(player->id, (sf::Vector2i)rc->sprite.getPosition());

  return player;
}

void Engine::createMap() {
  auto verticalWallTexture = mTextures[static_cast<int>(Textures::VerticalWall1)];
  auto horizontalWallTexture = mTextures[static_cast<int>(Textures::HorizontalWall1)];
  for (unsigned int i = 0; i < mGameMap.size(); i++) {
    auto row = mGameMap[i];
    for (unsigned int j = 0; j < row.size(); j++) {
      int tile = row[j];
      if (tile == 1 || tile == 2) {
        createHorizontalWall((float)(j * horizontalWallTexture->getSize().x), (float)(i * horizontalWallTexture->getSize().y));
      } else if (tile == 3) {
        createVerticalWall((float)(j * verticalWallTexture->getSize().x), (float)(i * verticalWallTexture->getSize().y));
      } else if (tile == 4) {
        createWholeFloor((float)(j * 32), (float)(i * 32));
      } else if (tile == 5) {
        createCrackedFloor((float)(j * 32), (float)(i * 32));
      } else if (tile == 6) {
        createFloorSpikes((float)(j * 32), (float)(i * 32));
      }
    }
  }
}

Entity* Engine::createHorizontalWall(float xPos, float yPos) {
  float size = 32;
  Entity* horizontalWall = mEntityManager.createEntity();
  auto collisionComponent = mCollisionComponentManager.addComponent(horizontalWall->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;
  RenderComponent* rc = mRenderComponentManager.addComponent(horizontalWall->id);
  rc->texture = *mTextures[static_cast<int>(Textures::HorizontalWall1)];
  rc->visible = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::HorizontalWall1;

  mGridSystem.addEntity(horizontalWall->id, (sf::Vector2i)rc->sprite.getPosition());
  return horizontalWall;
}

Entity* Engine::createVerticalWall(float xPos, float yPos) {
  float size = 32;
  Entity* verticalWall = mEntityManager.createEntity();
  auto collisionComponent = mCollisionComponentManager.addComponent(verticalWall->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  RenderComponent* rc = mRenderComponentManager.addComponent(verticalWall->id);
  rc->texture = *mTextures[static_cast<int>(Textures::VerticalWall1)];
  rc->visible = true;
  rc->isDynamic = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::VerticalWall1;

  HealthComponent* hc = mHealthComponentManager.addComponent(verticalWall->id);
  hc->health = 100;
  hc->isAlive = true;

  mGridSystem.addEntity(verticalWall->id, (sf::Vector2i)rc->sprite.getPosition());
  return verticalWall;
}

Entity* Engine::createWholeFloor(float xPos, float yPos) {
  float size = 32;
  Entity* wholeFloor = mEntityManager.createEntity();
  RenderComponent* rc = mRenderComponentManager.addComponent(wholeFloor->id);
  rc->texture = *mTextures[static_cast<int>(Textures::FloorWhole)];
  rc->visible = true;
  rc->isDynamic = false;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(32*2, 32*1, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::FloorWhole;

  return wholeFloor;
}

Entity* Engine::createCrackedFloor(float xPos, float yPos) {
  float size = 32;
  Entity* crackedFloor = mEntityManager.createEntity();
  RenderComponent* rc = mRenderComponentManager.addComponent(crackedFloor->id);
  rc->texture = *mTextures[static_cast<int>(Textures::FloorWhole)];
  rc->visible = true;
  rc->isDynamic = false;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(32 * 3, 32 * 1, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::FloorCracked;

  return crackedFloor;
}

Entity* Engine::createFloorSpikes(float xPos, float yPos) {
  float size = 32;
  Entity* floorSpikes = mEntityManager.createEntity();
  RenderComponent* rc = mRenderComponentManager.addComponent(floorSpikes->id);
  rc->texture = *mTextures[static_cast<int>(Textures::FloorWhole)];
  rc->visible = true;
  rc->isDynamic = false;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(32 * 3, 32 * 3, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::FloorSpikes;

  auto collisionComponent = mCollisionComponentManager.addComponent(floorSpikes->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;
  mGridSystem.addEntity(floorSpikes->id, (sf::Vector2i)rc->sprite.getPosition());

  DamageComponent* dc = mDamageComponentManager.addComponent(floorSpikes->id);
  dc->damage = 20;

  return floorSpikes;
}

Entity* Engine::createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition) {
  //TRACE_INFO("Creating bullet");
  auto playerShootClockComponent = mClockComponentManager.getComponent(entityId);
  if (playerShootClockComponent->clock.getElapsedTime() >= sf::milliseconds(100)) {
    playerShootClockComponent->clock.restart();

    auto playerPositionComponent = mRenderComponentManager.getComponent(entityId);
    float originXPos = playerPositionComponent->sprite.getPosition().x;
    float originYPos = playerPositionComponent->sprite.getPosition().y;

    sf::Vector2f bulletVelocity((float)(mousePosition.x - originXPos), (float)(mousePosition.y - originYPos));

    // Normalize velocity to avoid huge speeds, and multiply with 10 for extra speed
    float divider = sqrt(bulletVelocity.x*bulletVelocity.x + bulletVelocity.y*bulletVelocity.y);
    bulletVelocity.x = (bulletVelocity.x / divider) * 10;
    bulletVelocity.y = (bulletVelocity.y / divider) * 10;

    float angle = atan2(originYPos - mousePosition.y, originXPos - mousePosition.x) * (180 / 3.1415f) - 90; // Remove 90 degrees to compensate for rotation...

    // Move origin position to avoid colliding with the player
    if ((angle <= -125 && angle >= -235) || (angle >= -55 && angle <= 55)) {
      originXPos += bulletVelocity.x * 5.5f;
      originYPos += bulletVelocity.y * 5.5f;
    } else {
      originXPos += bulletVelocity.x * 4.0f;
      originYPos += bulletVelocity.y * 4.0f;
    }

    Entity* bullet = mEntityManager.createEntity();

    VelocityComponent* vc = mVelocityComponentManager.addComponent(bullet->id);
    vc->currentVelocity.x = bulletVelocity.x;
    vc->currentVelocity.y = bulletVelocity.y;
    vc->maxVelocity.x = 15;
    vc->maxVelocity.y = 15;
    vc->moveOnce = false;

    RenderComponent* rc = mRenderComponentManager.addComponent(bullet->id);
    rc->texture = *mTextures[static_cast<int>(Textures::Bullet)];
    rc->visible = true;
    rc->isDynamic = true;
    rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 8, 8));
    rc->sprite.setOrigin(4, 4);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->sprite.setRotation(angle);
    rc->textureId = Textures::Bullet;

    DamageComponent* dc = mDamageComponentManager.addComponent(bullet->id);
    dc->damage = 10;

    CollisionComponent* cc = mCollisionComponentManager.addComponent(bullet->id);
    cc->collided = false;
    cc->destroyOnCollision = true;

    mGridSystem.addEntity(bullet->id, (sf::Vector2i)rc->sprite.getPosition());

    return bullet;
  }
  return nullptr;
}

sf::Texture* Engine::loadTexture(std::string fileName) {
  sf::Texture* texture = new sf::Texture();
  if (!Collision::CreateTextureAndBitmask(*texture, "Client/Resources/Sprites/" + fileName)) {
    std::cout << "[GUI] ERROR could not load file " << "Client/Resources/Sprites/" << fileName << std::endl;
  }
  return texture;
}

void Engine::destroyEntity(int entityId) {
  auto entityRenderComponent = mRenderComponentManager.getComponent(entityId);
  mGridSystem.removeEntity(entityId, (sf::Vector2i)entityRenderComponent->sprite.getPosition());
  mRenderComponentManager.removeComponent(entityId);
  mCollisionComponentManager.removeComponent(entityId);
  mVelocityComponentManager.removeComponent(entityId);
  mAnimationComponentManager.removeComponent(entityId);
  mHealthComponentManager.removeComponent(entityId);
  mDamageComponentManager.removeComponent(entityId);
  mClockComponentManager.removeComponent(entityId);
  for (auto client : *mConnectedClients) {
    if (client.second->getEntity() && client.second->getEntity()->id == entityId) {
      client.second->setEntity(nullptr);
    }
  }
  mEntityManager.destroyEntity(entityId);
}
