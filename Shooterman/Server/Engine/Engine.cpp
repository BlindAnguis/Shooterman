#include "Engine.h"
#include "../../Common/Textures.h"

Engine::Engine() :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mSolidComponentManager(ComponentManager<SolidComponent>()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mSolidComponentManager, &mVelocityComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionSystem, &mEntityManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager))
{
  mInputSystem.attach(&mMovementSystem);
}

Engine::Engine(std::array<std::array<int, 16>, 16> gameMap) :
  mInputSystem(InputSystem()),
  mEntityManager(EntityManager()),
  mRenderComponentManager(ComponentManager<RenderComponent>()),
  mVelocityComponentManager(ComponentManager<VelocityComponent>()),
  mSolidComponentManager(ComponentManager<SolidComponent>()),
  mCollisionSystem(CollisionSystem(&mRenderComponentManager, &mSolidComponentManager, &mVelocityComponentManager)),
  mMovementSystem(MovementSystem(&mVelocityComponentManager, &mRenderComponentManager, &mCollisionSystem, &mEntityManager)),
  mRenderSystem(RenderSystem(&mRenderComponentManager)),
  mGameMap(gameMap)
{
  mInputSystem.attach(&mMovementSystem);
  mTextures[0] = loadTexture("Player.png");
  mTextures[1] = loadTexture("wall1.png");
  mTextures[2] = loadTexture("verticalWall1.png");
}

Engine::~Engine() {
}

void Engine::update() {
  mInputSystem.handleInput();
  mMovementSystem.ownUpdate();

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
  rc->texture = sf::Texture();
  if (!Collision::CreateTextureAndBitmask(rc->texture, "Client/Resources/Sprites/Player.png")) {
    std::cout << "[SERVER: ENGINE] ERROR could not load file " << "Client/Resources/Sprites/Player.png" << std::endl;
  }
  rc->visible = true;
  rc->sprite.setPosition(xStartPos, yStartPos);
  rc->sprite.setTexture(rc->texture);
  rc->textureId = static_cast<sf::Int16>(Textures::Player1);
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
  rc->texture = sf::Texture();
  if (!Collision::CreateTextureAndBitmask(rc->texture, "Client/Resources/Sprites/Player.png")) {
    std::cout << "[SERVER: ENGINE] ERROR could not load file " << "Client/Resources/Sprites/Player.png" << std::endl;
  }
  rc->visible = true;
  rc->sprite.setPosition(xStartPos, yStartPos);
  rc->sprite.setTexture(rc->texture);
  rc->textureId = static_cast<sf::Int16>(Textures::Player1);
  return ball;
}

void Engine::createMap() {
  auto verticalWallTexture = mTextures[static_cast<int>(Textures::VerticalWall1)];
  auto horizontalWallTexture = mTextures[static_cast<int>(Textures::HorizontalWall1)];
  int tileIndex = 0;
  for (int i = 0; i < mGameMap.size(); i++) {
    auto row = mGameMap[i];
    for (int j = 0; j < row.size(); j++) {
      int tile = row[j];
      if (tile == 1 || tile == 2) {
        createHorizontalWall(j * horizontalWallTexture->getSize().x, i * horizontalWallTexture->getSize().y);
      }
      else if (tile == 3) {
        createVerticalWall(j * verticalWallTexture->getSize().x, i * verticalWallTexture->getSize().y);
      }
    }
  }
}

Entity* Engine::createHorizontalWall(float xPos, float yPos) {
  Entity* horizontalWall = mEntityManager.createEntity();
  mSolidComponentManager.addComponent(horizontalWall->id);
  RenderComponent* rc = mRenderComponentManager.addComponent(horizontalWall->id);
  /*
  rc->texture = sf::Texture();
  if (!Collision::CreateTextureAndBitmask(rc->texture, "Client/Resources/Sprites/Player.png")) {
  std::cout << "[SERVER: ENGINE] ERROR could not load file " << "Client/Resources/Sprites/Player.png" << std::endl;
  }
  */
  rc->texture = *mTextures[static_cast<int>(Textures::HorizontalWall1)];
  rc->visible = true;
  rc->sprite.setPosition(xPos, yPos);
  rc->sprite.setTexture(rc->texture);
  rc->textureId = static_cast<sf::Int16>(Textures::HorizontalWall1);
  //rc->isPlayer = false;
  return horizontalWall;
}

Entity* Engine::createVerticalWall(float xPos, float yPos) {
  Entity* verticalWall = mEntityManager.createEntity();
  mSolidComponentManager.addComponent(verticalWall->id);

  RenderComponent* rc = mRenderComponentManager.addComponent(verticalWall->id);
  /*
  rc->texture = sf::Texture();
  if (!Collision::CreateTextureAndBitmask(rc->texture, "Client/Resources/Sprites/Player.png")) {
  std::cout << "[SERVER: ENGINE] ERROR could not load file " << "Client/Resources/Sprites/Player.png" << std::endl;
  }
  */
  rc->texture = *mTextures[static_cast<int>(Textures::VerticalWall1)];
  rc->visible = true;
  rc->sprite.setPosition(xPos, yPos);
  rc->sprite.setTexture(rc->texture);
  rc->textureId = static_cast<sf::Int16>(Textures::VerticalWall1);
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
