#pragma once

#include "../Components/ComponentManager.h"
#include "../Components/Components.h"
#include "../EntityManager/EntityManager.h"
#include "../Systems/GridSystem/GridSystem.h"
#include "../Systems/DeleteSystem/DeleteSystem.h"
#include "../../Common/Trace.h"
#include "../../Common/MessageHandler/MessageHandler.h"
#include "../../Common/MessageHandler/Subscriber.h"

#include <array>

#define KNIGHT_MAX_VELOCITY 4.8f
#define KNIGHT_MAX_HEALTH 200
#define KNIGHT_MAX_STAMINA 150
#define KNIGHT_ATTACK_SPEED 400

#define SPEARMAN_MAX_VELOCITY 4.65f
#define SPEARMAN_MAX_HEALTH 150
#define SPEARMAN_MAX_STAMINA 100
#define SPEARMAN_ATTACK_SPEED 450

#define MAGE_MAX_VELOCITY 4.5f
#define MAGE_MAX_HEALTH 100
#define MAGE_MAX_MANA 100
#define MAGE_MANA_COST 20
#define MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MIN 15
#define MAGE_NR_OF_SUPER_LIGHTNING_STRIKES_MAX 40
#define MAGE_ATTACK_SPEED 500

#define HEALTH_POTION_EFFECT 50
#define MANA_POTION_EFFECT 50
#define STAMINA_POTION_EFFECT 50

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

  std::map<Textures, sf::Texture*> mTextures;

  Entity* createPlayerBase(float maxVelocity, Textures textureType, sf::Vector2f position, int health, int attackSpeed);
  Entity* createMage(sf::Vector2f position);
  Entity* createKnight(sf::Vector2f position);
  Entity* createSpearman(sf::Vector2f position);
  void loadTexture(Textures textureId, std::string fileName);
  Entity* createBullet(int entityId, std::uint32_t input, sf::Vector2i mousePosition, bool visible = true);
  Entity* createMelee(int entityId, std::uint32_t input, sf::Vector2i mousePosition);
  void createRandomLightningBolts();
  Subscriber mSoundSubscriber;
};
