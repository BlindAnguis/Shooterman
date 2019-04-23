#include "MapCreator.h"
#include "../Systems/CollisionSystem/Collision.h"

MapCreator::MapCreator(
  EntityManager *entityManager,
  GridSystem *gridSystem
) :
  mEntityManager(entityManager),
  mRenderComponentManager(&ComponentManager<RenderComponent>::get()),
  mCollisionComponentManager(&ComponentManager<CollisionComponent>::get()),
  mHealthComponentManager(&ComponentManager<HealthComponent>::get()),
  mDamageComponentManager(&ComponentManager<DamageComponent>::get()),
  mGridSystem(gridSystem)
{
  mName = "MAP_CREATOR";
  mTextures[static_cast<int>(Textures::HorizontalWall1)] = loadTexture("wall1.png");
  mTextures[static_cast<int>(Textures::VerticalWall1)] = loadTexture("verticalWall1.png");
  mTextures[static_cast<int>(Textures::FloorWhole)] = loadTexture("fantasy-tileset.png");
  mTextures[static_cast<int>(Textures::FloorCracked)] = loadTexture("fantasy-tileset.png");
  mTextures[static_cast<int>(Textures::FloorSpikes)] = loadTexture("fantasy-tileset.png");
}

MapCreator::~MapCreator()
{
}

void MapCreator::createMap(std::array<std::array<int, 32>, 32> gameMap) {
  auto verticalWallTexture = mTextures[static_cast<int>(Textures::VerticalWall1)];
  auto horizontalWallTexture = mTextures[static_cast<int>(Textures::HorizontalWall1)];
  for (unsigned int i = 0; i < gameMap.size(); i++) {
    auto row = gameMap[i];
    for (unsigned int j = 0; j < row.size(); j++) {
      int tile = row[j];
      if (tile == 1 || tile == 2) {
        createHorizontalWall((float)(j * horizontalWallTexture->getSize().x), (float)(i * horizontalWallTexture->getSize().y));
      } else if (tile == 3) {
        createVerticalWall((float)(j * verticalWallTexture->getSize().x), (float)(i * verticalWallTexture->getSize().y));
      } else if (tile == 4) {
        createFloor(Textures::FloorWhole, sf::Vector2f(j * 32.0f, i * 32.0f), sf::IntRect(32 * 2, 32 * 1, 32, 32));
      } else if (tile == 5) {
        createFloor(Textures::FloorCracked, sf::Vector2f(j * 32.0f, i * 32.0f), sf::IntRect(32 * 3, 32 * 1, 32, 32));
      } else if (tile == 6) {
        createFloorSpikes(sf::Vector2f(j * 32.0f, i * 32.0f));
      }
    }
  }
}

Entity* MapCreator::createHorizontalWall(float xPos, float yPos) {
  float size = 32;
  Entity* horizontalWall = mEntityManager->createEntity();
  auto collisionComponent = mCollisionComponentManager->addComponent(horizontalWall->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;
  RenderComponent* rc = mRenderComponentManager->addComponent(horizontalWall->id);
  rc->visible = true;
  rc->sprite = sf::Sprite(*mTextures[static_cast<int>(Textures::HorizontalWall1)], sf::IntRect(0, 0, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::HorizontalWall1;

  mGridSystem->addEntity(horizontalWall->id, (sf::Vector2i)rc->sprite.getPosition());
  return horizontalWall;
}

Entity* MapCreator::createVerticalWall(float xPos, float yPos) {
  float size = 32;
  Entity* verticalWall = mEntityManager->createEntity();
  auto collisionComponent = mCollisionComponentManager->addComponent(verticalWall->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;

  RenderComponent* rc = mRenderComponentManager->addComponent(verticalWall->id);
  rc->visible = true;
  rc->isDynamic = false;
  rc->sprite = sf::Sprite(*mTextures[static_cast<int>(Textures::VerticalWall1)], sf::IntRect(0, 0, (int)size, (int)size));
  rc->sprite.setOrigin(size / 2, size / 2);
  rc->sprite.setPosition(xPos + (size / 2), yPos + (size / 2));
  rc->textureId = Textures::VerticalWall1;

  mGridSystem->addEntity(verticalWall->id, (sf::Vector2i)rc->sprite.getPosition());
  return verticalWall;
}

Entity* MapCreator::createFloor(Textures texture, sf::Vector2f position, sf::IntRect positionInTexture) {
  Entity* floor = mEntityManager->createEntity();
  RenderComponent* rc = mRenderComponentManager->addComponent(floor->id);
  rc->visible = true;
  rc->isDynamic = false;
  rc->sprite = sf::Sprite(*mTextures[static_cast<int>(texture)], positionInTexture);
  rc->sprite.setOrigin(positionInTexture.width / 2.0f, positionInTexture.height / 2.0f);
  rc->sprite.setPosition(position.x + (positionInTexture.width / 2), position.y + (positionInTexture.height / 2));
  rc->textureId = texture;
  return floor;
}

Entity* MapCreator::createFloorSpikes(sf::Vector2f position) {
  Entity* floorSpikes = createFloor(Textures::FloorSpikes, position, sf::IntRect(32 * 3, 32 * 3, 32, 32));

  auto rc = mRenderComponentManager->getComponent(floorSpikes->id);

  auto collisionComponent = mCollisionComponentManager->addComponent(floorSpikes->id);
  collisionComponent->collided = false;
  collisionComponent->destroyOnCollision = false;
  mGridSystem->addEntity(floorSpikes->id, (sf::Vector2i)rc->sprite.getPosition());

  DamageComponent* dc = mDamageComponentManager->addComponent(floorSpikes->id);
  dc->damage = 10;
  return floorSpikes;
}

sf::Texture* MapCreator::loadTexture(std::string fileName) {
  sf::Texture* texture = new sf::Texture();
  if (!Collision::CreateTextureAndBitmask(*texture, "Client/Resources/Sprites/" + fileName)) {
    TRACE_ERROR("[GUI] ERROR could not load file " << "Client/Resources/Sprites/" << fileName);
  }
  return texture;
}
