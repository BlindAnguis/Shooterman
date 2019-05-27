#pragma once

#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include "../EntityManager/EntityManager.h"
#include "../Systems/GridSystem/GridSystem.h"
#include "../Systems/DeleteSystem/DeleteSystem.h"
#include "../../Common/Trace.h"

#include <array>

class EntityCreator : Trace
{
public:
  EntityCreator();
  ~EntityCreator();

  Entity* createPlayer(PlayerClass playerClass, sf::Vector2f position);
  Entity* createRandomPickup();
  Entity* createLightningStrike(sf::Vector2f position, std::set<int> immuneEntityIds);

  void clearSprites();

  DeleteSystem *mDeleteSystem;

private:
  EntityManager *mEntityManager;
  ComponentManager<RenderComponent> *mRenderComponentManager;
  ComponentManager<VelocityComponent> *mVelocityComponentManager;
  ComponentManager<CollisionComponent> *mCollisionComponentManager;
  ComponentManager<AnimationComponent> *mAnimationComponentManager;
  ComponentManager<HealthComponent> *mHealthComponentManager;
  ComponentManager<ClockComponent>* mClockComponentManager;
  ComponentManager<PlayerComponent> *mPlayerComponentManager;
  ComponentManager<HealthChangerComponent> *mHealthChangerComponentManager;
  
  GridSystem *mGridSystem;

  std::map<Textures, sf::Texture> mTextures;

  Entity* createPlayerBase(float maxVelocity, Textures textureType, sf::Vector2f position, int health, int attackSpeed);
  Entity* createMage(sf::Vector2f position);
  Entity* createKnight(sf::Vector2f position);
  Entity* createSpearman(sf::Vector2f position);
  void loadTexture(Textures textureId, std::string fileName);
  Entity* createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition, bool visible = true);
  Entity* createMelee(int entityId, std::uint32_t input, sf::Vector2i mousePosition);
  void createRandomLightningBolts();
};
