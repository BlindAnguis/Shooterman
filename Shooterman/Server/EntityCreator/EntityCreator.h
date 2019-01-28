#pragma once

#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include "../EntityManager/EntityManager.h"
#include "../Systems/GridSystem/GridSystem.h"
#include "../../Common/Trace.h"

#include <array>

class EntityCreator : Trace
{
public:
  EntityCreator(
    EntityManager *entityManager,
    ComponentManager<RenderComponent> *renderComponentManager,
    ComponentManager<VelocityComponent> *velocityComponentManager,
    ComponentManager<CollisionComponent> *collisionComponentManager,
    ComponentManager<AnimationComponent> *animationComponentManager,
    ComponentManager<HealthComponent> *healthComponentManager,
    ComponentManager<ClockComponent> *clockComponentManager,
    ComponentManager<PlayerComponent> *playerComponentManager,
    ComponentManager<DamageComponent> *damageComponentManager,
    GridSystem *gridSystem
  );
  ~EntityCreator();

  Entity* createPlayer(PlayerClass playerClass, sf::Vector2f position);

private:
  EntityManager *mEntityManager;
  ComponentManager<RenderComponent> *mRenderComponentManager;
  ComponentManager<VelocityComponent> *mVelocityComponentManager;
  ComponentManager<CollisionComponent> *mCollisionComponentManager;
  ComponentManager<AnimationComponent> *mAnimationComponentManager;
  ComponentManager<HealthComponent> *mHealthComponentManager;
  ComponentManager<ClockComponent>* mClockComponentManager;
  ComponentManager<PlayerComponent> *mPlayerComponentManager;
  ComponentManager<DamageComponent> *mDamageComponentManager;
  
  GridSystem *mGridSystem;

  std::array<sf::Texture*, 99> mTextures;

  Entity* createPlayerBase(float maxVelocity, Textures textureType, sf::Vector2f position, int health, int attackSpeed);
  Entity* createMage(sf::Vector2f position);
  Entity* createKnight(sf::Vector2f position);
  Entity* createSpearman(sf::Vector2f position);
  sf::Texture* loadTexture(std::string fileName);
  Entity* createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition);
};

