#include "Engine.h"
#include "../../Common/Textures.h"

Engine::Engine() :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mSolidComponentManager(ComponentManager<SolidComponent>()),
  mAnimationComponentManager(ComponentManager<AnimationComponent>()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mSolidComponentManager, &mVelocityComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionSystem, &mEntityManager, &mAnimationComponentManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mAnimationSystem(&mAnimationComponentManager, &mVelocityComponentManager, &mRenderComponentManager)
{
  mInputSystem.attach(&mMovementSystem);
}

Engine::Engine(std::array<std::array<int, 16>, 16> gameMap) :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mSolidComponentManager(ComponentManager<SolidComponent>()),
  mAnimationComponentManager(ComponentManager<AnimationComponent>()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mSolidComponentManager, &mVelocityComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionSystem, &mEntityManager, &mAnimationComponentManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mAnimationSystem(&mAnimationComponentManager, &mVelocityComponentManager, &mRenderComponentManager),
  mGameMap(gameMap)
{
  mInputSystem.attach(&mMovementSystem);
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
    it->second.second = createPlayer(x, 60, 5, 5, 100);
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

  mSolidComponentManager.addComponent(player->id);

  RenderComponent* rc = mRenderComponentManager.addComponent(player->id);
  rc->texture = *mTextures[static_cast<int>(Textures::Player1)];
  rc->visible = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(10, 0, 80, 100));
  rc->sprite.setPosition(xStartPos, yStartPos);
  rc->textureId = Textures::Player1;

  AnimationComponent* ac = mAnimationComponentManager.addComponent(player->id);
  ac->animation = Animations::Idle;
  ac->animationFrame = 0;

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

  mSolidComponentManager.addComponent(ball->id);

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
  Entity* horizontalWall = mEntityManager.createEntity();
  mSolidComponentManager.addComponent(horizontalWall->id);
  RenderComponent* rc = mRenderComponentManager.addComponent(horizontalWall->id);
  rc->texture = *mTextures[static_cast<int>(Textures::HorizontalWall1)];
  rc->visible = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 32, 32));
  rc->sprite.setPosition(xPos, yPos);
  rc->textureId = Textures::HorizontalWall1;
  //rc->isPlayer = false;
  return horizontalWall;
}

Entity* Engine::createVerticalWall(float xPos, float yPos) {
  Entity* verticalWall = mEntityManager.createEntity();
  mSolidComponentManager.addComponent(verticalWall->id);
  RenderComponent* rc = mRenderComponentManager.addComponent(verticalWall->id);
  rc->texture = *mTextures[static_cast<int>(Textures::VerticalWall1)];
  rc->visible = true;
  rc->sprite = sf::Sprite(rc->texture, sf::IntRect(0, 0, 32, 32));
  rc->sprite.setPosition(xPos, yPos);
  rc->textureId = Textures::VerticalWall1;
  //rc->isPlayer = false;
  return verticalWall;
}

sf::Texture* Engine::loadTexture(std::string fileName) {
  sf::Texture* texture = new sf::Texture();
  if (!Collision::CreateTextureAndBitmask(*texture, "Client/Resources/Sprites/" + fileName)) {
    std::cout << "[GUI] ERROR could not load file " << "Client/Resources/Sprites/" << fileName << std::endl;
  }
  return texture;
}
