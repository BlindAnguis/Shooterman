#include "Engine.h"
#include "../../Common/Textures.h"

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
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mVelocityComponentManager, &mCollisionComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionSystem, &mEntityManager, &mAnimationComponentManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mAnimationSystem(&mAnimationComponentManager, &mVelocityComponentManager, &mRenderComponentManager)
{
  mInputSystem.attach(&mMovementSystem);
  mInputSystem.setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { createBullet(entityId, input, mousePosition); });
}

Engine::Engine(std::array<std::array<int, 32>, 32> gameMap) :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mCollisionComponentManager(ComponentManager<CollisionComponent>()),
  mAnimationComponentManager(ComponentManager<AnimationComponent>()),
  mHealthComponentManager(ComponentManager<HealthComponent>()),
  mDamageComponentManager(ComponentManager<DamageComponent>()),
  mClockComponentManager(ComponentManager<ClockComponent>()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mVelocityComponentManager, &mCollisionComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionSystem, &mEntityManager, &mAnimationComponentManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mAnimationSystem(&mAnimationComponentManager, &mVelocityComponentManager, &mRenderComponentManager),
  mGameMap(gameMap)
{
  mInputSystem.attach(&mMovementSystem);
  mInputSystem.setAttackCallback([this](int entityId, std::uint32_t input, sf::Vector2i mousePosition) { createBullet(entityId, input, mousePosition); });
  mTextures[static_cast<int>(Textures::Player1)] = loadTexture("Player.png");
  mTextures[static_cast<int>(Textures::HorizontalWall1)] = loadTexture("wall1.png");
  mTextures[static_cast<int>(Textures::VerticalWall1)] = loadTexture("verticalWall1.png");
}

Engine::~Engine() {
}

void Engine::update() {
  for (auto entity : mAnimationComponentManager.getAllEntitiesWithComponent()) {
    entity.second->animation = Animations::Idle;
  }
  mInputSystem.handleInput();
  mMovementSystem.ownUpdate();
  mAnimationSystem.update();
  mRenderSystem.render(mConnectedClients);
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
    it->second.second = createPlayer(x, 100, 5, 5, 100);
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

  //mSolidComponentManager.addComponent(player->id);

  auto collisionComponent = mCollisionComponentManager.addComponent(player->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  RenderComponent* rc = mRenderComponentManager.addComponent(player->id);
  rc->texture = *mTextures[static_cast<int>(Textures::Player1)];
  rc->visible = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(10, 0, 80, 100));
  rc->sprite.setOrigin(47, 47);
  rc->sprite.setPosition(xStartPos, yStartPos);
  rc->textureId = Textures::Player1;

  AnimationComponent* ac = mAnimationComponentManager.addComponent(player->id);
  ac->animation = Animations::Idle;
  ac->animationFrame = 0;

  ClockComponent* cc = mClockComponentManager.addComponent(player->id);

  return player;
}

Entity* Engine::createBall(float xStartPos, float yStartPos, float xMaxVelocity, float yMaxVelocity) {
  Entity* ball = mEntityManager.createEntity();
  VelocityComponent* vc = mVelocityComponentManager.addComponent(ball->id);
  vc->currentVelocity.x = 0;
  vc->currentVelocity.y = 0;
  vc->maxVelocity.x = xMaxVelocity;
  vc->maxVelocity.y = yMaxVelocity;
  vc->moveOnce = false;

  auto collisionComponent = mCollisionComponentManager.addComponent(ball->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  RenderComponent* rc = mRenderComponentManager.addComponent(ball->id);
  rc->texture = *mTextures[static_cast<int>(Textures::Player1)];
  rc->visible = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 200, 200));
  rc->sprite.setPosition(xStartPos, yStartPos);
  rc->sprite.setScale(0.25, 0.25);
  rc->textureId = Textures::Player1;
  return ball;
}

void Engine::createMap() {
  auto verticalWallTexture = mTextures[static_cast<int>(Textures::VerticalWall1)];
  auto horizontalWallTexture = mTextures[static_cast<int>(Textures::HorizontalWall1)];
  int tileIndex = 0;
  for (unsigned int i = 0; i < mGameMap.size(); i++) {
    auto row = mGameMap[i];
    for (unsigned int j = 0; j < row.size(); j++) {
      int tile = row[j];
      if (tile == 1 || tile == 2) {
        createHorizontalWall((float)(j * horizontalWallTexture->getSize().x), (float)(i * horizontalWallTexture->getSize().y));
      }
      else if (tile == 3) {
        createVerticalWall((float)(j * verticalWallTexture->getSize().x), (float)(i * verticalWallTexture->getSize().y));
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
  //rc->isPlayer = false;
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
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::VerticalWall1;
  //rc->isPlayer = false;

  HealthComponent* hc = mHealthComponentManager.addComponent(verticalWall->id);
  hc->health = 100;
  hc->isAlive = true;

  return verticalWall;
}

Entity* Engine::createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition) {
  auto playerShootClockComponent = mClockComponentManager.getComponent(entityId);
  if (playerShootClockComponent->clock.getElapsedTime() >= sf::milliseconds(500)) {
    playerShootClockComponent->clock.restart();

    auto playerPositionComponent = mRenderComponentManager.getComponent(entityId);
    int originXPos = playerPositionComponent->sprite.getPosition().x;
    int originYPos = playerPositionComponent->sprite.getPosition().y;

    sf::Vector2f bulletVelocity(mousePosition.x - originXPos, mousePosition.y - originYPos);

    // Normalize velocity to avoid huge speeds, and multiply with 10 for extra speed
    float divider = sqrt(bulletVelocity.x*bulletVelocity.x + bulletVelocity.y*bulletVelocity.y);
    bulletVelocity.x = (bulletVelocity.x / divider) * 15;
    bulletVelocity.y = (bulletVelocity.y / divider) * 15;

    // Move origin position to avoid colliding with the player
    originXPos += bulletVelocity.x * 5;
    originYPos += bulletVelocity.y * 5;

    Entity* bullet = mEntityManager.createEntity();

    VelocityComponent* vc = mVelocityComponentManager.addComponent(bullet->id);
    vc->currentVelocity.x = bulletVelocity.x;
    vc->currentVelocity.y = bulletVelocity.y;
    vc->maxVelocity.x = 15;
    vc->maxVelocity.y = 15;
    vc->moveOnce = false;
  
    RenderComponent* rc = mRenderComponentManager.addComponent(bullet->id);
    rc->texture = *mTextures[static_cast<int>(Textures::VerticalWall1)];
    rc->visible = true;
    rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 32, 32));
    rc->sprite.setOrigin(16, 16);
    rc->sprite.setPosition(originXPos, originYPos);
    rc->textureId = Textures::VerticalWall1;

    DamageComponent* dc = mDamageComponentManager.addComponent(bullet->id);
    dc->damage = 10;

    HealthComponent* hc = mHealthComponentManager.addComponent(bullet->id);
    hc->health = 1;
    hc->isAlive = true;

    CollisionComponent* cc = mCollisionComponentManager.addComponent(bullet->id);
    cc->collided = false;
    cc->destroyOnCollision = true;

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
