#include "pch.h"

#include "../Shooterman/Server/EntityCreator/EntityCreator.cpp"
#include "../Shooterman/Server/EntityManager/EntityManager.cpp"
#include "../Shooterman/Server/Systems/GridSystem/GridSystem.cpp"
#include "../Shooterman/Server/Systems/DeleteSystem/DeleteSystem.cpp"
#include "../Shooterman/Server/Systems/ManaSystem/ManaSystem.cpp"
#include "../Shooterman/Server/Systems/EntityCreatorSystem/EntityCreatorSystem.cpp"
#include "../Shooterman/Server/Systems/CollisionSystem/Collision.cpp"
#include "../Shooterman/Common/Animation/Animation.cpp"
#include "../Shooterman/Common/MessageHandler/Subscriber.cpp"
#include "../Shooterman/Common/MessageHandler/Interface.cpp"
#include "../Shooterman/Common/MessageHandler/MessageHandler.h"
#include "../Shooterman/Common/Messages/SubscribeDoneMessage.cpp"
#include "../Shooterman/common/Messages/SubscribeTimeoutMessage.cpp"
#include "../Shooterman/Common/Messages/SoundMessage.cpp"
#include "../Shooterman/Common/Messages/SubscribeDoneMessage.cpp"
#include "../Shooterman/Common/Messages/SubscribeTimeoutMessage.cpp"

TEST(EntityCreatorTests, createMage) {
  std::shared_ptr<MessageHandler> mh;

  EntityCreator e = EntityCreator(mh);
  sf::Vector2f magePosition = sf::Vector2f(10, 50); // Arbitrary position
  int numberOfMageAnimations = 17;
  auto mage = e.createPlayer(PlayerClass::Mage, magePosition);

  // Get all components a mage is suppose to have.
  auto velocityComponent = ComponentManager<VelocityComponent>::get().getComponent(mage->id);
  auto collisionComponent = ComponentManager<CollisionComponent>::get().getComponent(mage->id);
  auto renderComponent = ComponentManager<RenderComponent>::get().getComponent(mage->id);
  auto animationComponent = ComponentManager<AnimationComponent>::get().getComponent(mage->id);
  auto clockComponent = ComponentManager<ClockComponent>::get().getComponent(mage->id);
  auto healthComponent = ComponentManager<HealthComponent>::get().getComponent(mage->id);
  auto playerComponent = ComponentManager<PlayerComponent>::get().getComponent(mage->id);
  auto manaComponent = ComponentManager<ManaComponent>::get().getComponent(mage->id);

  // Assert that all components exists.
  ASSERT_TRUE(velocityComponent);
  ASSERT_TRUE(collisionComponent);
  ASSERT_TRUE(renderComponent);
  ASSERT_TRUE(animationComponent);
  ASSERT_TRUE(clockComponent);
  ASSERT_TRUE(healthComponent);
  ASSERT_TRUE(playerComponent);
  ASSERT_TRUE(manaComponent);

  // Check that all components have the right values.
  EXPECT_EQ(velocityComponent->currentVelocity, sf::Vector2f(0,0));
  EXPECT_EQ(velocityComponent->maxVelocity, sf::Vector2f(MAGE_MAX_VELOCITY_TEST, MAGE_MAX_VELOCITY_TEST));
  EXPECT_TRUE(velocityComponent->moveOnce);

  EXPECT_FALSE(collisionComponent->collided);
  EXPECT_EQ(collisionComponent->collidedList.size(), 0); // Expect empty.
  EXPECT_FALSE(collisionComponent->destroyOnCollision);

  EXPECT_EQ(animationComponent->currentAnimation, AnimationType::IdleUp);
  EXPECT_EQ(animationComponent->animations.size(), numberOfMageAnimations);

  EXPECT_TRUE(renderComponent->isDynamic);
  EXPECT_EQ(renderComponent->sprite.getPosition(), magePosition);
  EXPECT_EQ(renderComponent->textureId, Textures::CharacterMage);
  EXPECT_TRUE(renderComponent->visible);

  EXPECT_EQ(healthComponent->currentHealth, MAGE_MAX_HEALTH_TEST);
  EXPECT_TRUE(healthComponent->isAlive);
  EXPECT_EQ(healthComponent->maxHealth, MAGE_MAX_HEALTH_TEST);
  
  EXPECT_EQ(playerComponent->attackSpeed, MAGE_ATTACK_SPEED_TEST);
  EXPECT_FALSE(playerComponent->invinsible);
  EXPECT_EQ(playerComponent->nrOfDeaths, 0);
  EXPECT_EQ(playerComponent->nrOfKills, 0);
  EXPECT_EQ(playerComponent->playerClass, PlayerClass::Mage);
  EXPECT_EQ(playerComponent->score, 0);
  EXPECT_EQ(playerComponent->state, PlayerState::Idle);
  EXPECT_EQ(playerComponent->superAttacks, 0);

  EXPECT_EQ(manaComponent->currentMana, MAGE_MAX_MANA_TEST);
  EXPECT_EQ(manaComponent->maxMana, MAGE_MAX_MANA_TEST);
}

TEST(EntityCreatorTests, createKnight) {
  std::shared_ptr<MessageHandler> mh;
  EntityCreator e = EntityCreator(mh);
  sf::Vector2f knightPosition = sf::Vector2f(550, 230); // Arbitrary position
  int numberOfKnightAnimations = 17;
  auto knight = e.createPlayer(PlayerClass::Knight, knightPosition);

  // Get all components a knight is suppose to have.
  auto velocityComponent = ComponentManager<VelocityComponent>::get().getComponent(knight->id);
  auto collisionComponent = ComponentManager<CollisionComponent>::get().getComponent(knight->id);
  auto renderComponent = ComponentManager<RenderComponent>::get().getComponent(knight->id);
  auto animationComponent = ComponentManager<AnimationComponent>::get().getComponent(knight->id);
  auto clockComponent = ComponentManager<ClockComponent>::get().getComponent(knight->id);
  auto healthComponent = ComponentManager<HealthComponent>::get().getComponent(knight->id);
  auto playerComponent = ComponentManager<PlayerComponent>::get().getComponent(knight->id);
  auto staminaComponent = ComponentManager<StaminaComponent>::get().getComponent(knight->id);

  // Assert that all components exists.
  ASSERT_TRUE(velocityComponent);
  ASSERT_TRUE(collisionComponent);
  ASSERT_TRUE(renderComponent);
  ASSERT_TRUE(animationComponent);
  ASSERT_TRUE(clockComponent);
  ASSERT_TRUE(healthComponent);
  ASSERT_TRUE(playerComponent);
  ASSERT_TRUE(staminaComponent);

  // Check that all components have the right values.
  EXPECT_EQ(velocityComponent->currentVelocity, sf::Vector2f(0, 0));
  EXPECT_EQ(velocityComponent->maxVelocity, sf::Vector2f(KNIGHT_MAX_VELOCITY_TEST, KNIGHT_MAX_VELOCITY_TEST));
  EXPECT_TRUE(velocityComponent->moveOnce);

  EXPECT_FALSE(collisionComponent->collided);
  EXPECT_EQ(collisionComponent->collidedList.size(), 0); // Expect empty.
  EXPECT_FALSE(collisionComponent->destroyOnCollision);

  EXPECT_EQ(animationComponent->currentAnimation, AnimationType::IdleUp);
  EXPECT_EQ(animationComponent->animations.size(), numberOfKnightAnimations);

  EXPECT_TRUE(renderComponent->isDynamic);
  EXPECT_EQ(renderComponent->sprite.getPosition(), knightPosition);
  EXPECT_EQ(renderComponent->textureId, Textures::CharacterKnight);
  EXPECT_TRUE(renderComponent->visible);

  EXPECT_EQ(healthComponent->currentHealth, KNIGHT_MAX_HEALTH_TEST);
  EXPECT_TRUE(healthComponent->isAlive);
  EXPECT_EQ(healthComponent->maxHealth, KNIGHT_MAX_HEALTH_TEST);

  EXPECT_EQ(playerComponent->attackSpeed, KNIGHT_ATTACK_SPEED_TEST);
  EXPECT_FALSE(playerComponent->invinsible);
  EXPECT_EQ(playerComponent->nrOfDeaths, 0);
  EXPECT_EQ(playerComponent->nrOfKills, 0);
  EXPECT_EQ(playerComponent->playerClass, PlayerClass::Knight);
  EXPECT_EQ(playerComponent->score, 0);
  EXPECT_EQ(playerComponent->state, PlayerState::Idle);
  EXPECT_EQ(playerComponent->superAttacks, 0);

  EXPECT_EQ(staminaComponent->currentStamina, KNIGHT_MAX_STAMINA_TEST);
  EXPECT_EQ(staminaComponent->maxStamina, KNIGHT_MAX_STAMINA_TEST);
}

TEST(EntityCreatorTests, createSpearman) {
  std::shared_ptr<MessageHandler> mh;
  EntityCreator e = EntityCreator(mh);
  sf::Vector2f spearmanPosition = sf::Vector2f(1002, 1004); // Arbitrary position
  int numberOfSpearmanAnimations = 17;
  auto spearman = e.createPlayer(PlayerClass::Spearman, spearmanPosition);

  // Get all components a spearman is suppose to have.
  auto velocityComponent = ComponentManager<VelocityComponent>::get().getComponent(spearman->id);
  auto collisionComponent = ComponentManager<CollisionComponent>::get().getComponent(spearman->id);
  auto renderComponent = ComponentManager<RenderComponent>::get().getComponent(spearman->id);
  auto animationComponent = ComponentManager<AnimationComponent>::get().getComponent(spearman->id);
  auto clockComponent = ComponentManager<ClockComponent>::get().getComponent(spearman->id);
  auto healthComponent = ComponentManager<HealthComponent>::get().getComponent(spearman->id);
  auto playerComponent = ComponentManager<PlayerComponent>::get().getComponent(spearman->id);
  auto staminaComponent = ComponentManager<StaminaComponent>::get().getComponent(spearman->id);

  // Assert that all components exists.
  ASSERT_TRUE(velocityComponent);
  ASSERT_TRUE(collisionComponent);
  ASSERT_TRUE(renderComponent);
  ASSERT_TRUE(animationComponent);
  ASSERT_TRUE(clockComponent);
  ASSERT_TRUE(healthComponent);
  ASSERT_TRUE(playerComponent);
  ASSERT_TRUE(staminaComponent);

  // Check that all components have the right values.
  EXPECT_EQ(velocityComponent->currentVelocity, sf::Vector2f(0, 0));
  EXPECT_EQ(velocityComponent->maxVelocity, sf::Vector2f(SPEARMAN_MAX_VELOCITY_TEST, SPEARMAN_MAX_VELOCITY_TEST));
  EXPECT_TRUE(velocityComponent->moveOnce);

  EXPECT_FALSE(collisionComponent->collided);
  EXPECT_EQ(collisionComponent->collidedList.size(), 0); // Expect empty.
  EXPECT_FALSE(collisionComponent->destroyOnCollision);

  EXPECT_EQ(animationComponent->currentAnimation, AnimationType::IdleUp);
  EXPECT_EQ(animationComponent->animations.size(), numberOfSpearmanAnimations);

  EXPECT_TRUE(renderComponent->isDynamic);
  EXPECT_EQ(renderComponent->sprite.getPosition(), spearmanPosition);
  EXPECT_EQ(renderComponent->textureId, Textures::CharacterSpearman);
  EXPECT_TRUE(renderComponent->visible);

  EXPECT_EQ(healthComponent->currentHealth, SPEARMAN_MAX_HEALTH_TEST);
  EXPECT_TRUE(healthComponent->isAlive);
  EXPECT_EQ(healthComponent->maxHealth, SPEARMAN_MAX_HEALTH_TEST);

  EXPECT_EQ(playerComponent->attackSpeed, SPEARMAN_ATTACK_SPEED_TEST);
  EXPECT_FALSE(playerComponent->invinsible);
  EXPECT_EQ(playerComponent->nrOfDeaths, 0);
  EXPECT_EQ(playerComponent->nrOfKills, 0);
  EXPECT_EQ(playerComponent->playerClass, PlayerClass::Spearman);
  EXPECT_EQ(playerComponent->score, 0);
  EXPECT_EQ(playerComponent->state, PlayerState::Idle);
  EXPECT_EQ(playerComponent->superAttacks, 0);

  EXPECT_EQ(staminaComponent->currentStamina, SPEARMAN_MAX_STAMINA_TEST);
  EXPECT_EQ(staminaComponent->maxStamina, SPEARMAN_MAX_STAMINA_TEST);
}

TEST(EntityCreatorTests, createRandomPickup) {
  std::shared_ptr<MessageHandler> mh;
  EntityCreator e = EntityCreator(mh);
  auto randomPickup = e.createRandomPickup();
  
  // Get all components a spearman is suppose to have.
  auto pickupComponent = ComponentManager<PickupComponent>::get().getComponent(randomPickup->id);
  auto collisionComponent = ComponentManager<CollisionComponent>::get().getComponent(randomPickup->id);
  auto renderComponent = ComponentManager<RenderComponent>::get().getComponent(randomPickup->id);
  auto healthChangerComponent = ComponentManager<HealthChangerComponent>::get().getComponent(randomPickup->id);

  // Assert that all components exists.
  ASSERT_TRUE(collisionComponent);
  ASSERT_TRUE(renderComponent);
  if (pickupComponent->type == PickupType::HealthPotion) {
    ASSERT_TRUE(healthChangerComponent);
  } else {
    ASSERT_FALSE(healthChangerComponent);
  }

  switch (pickupComponent->type)
  {
  case PickupType::HealthPotion: {
    EXPECT_EQ(pickupComponent->addedEffect, HEALTH_POTION_EFFECT_TEST);
    break;
  }
  case PickupType::ManaPotion: {
    EXPECT_EQ(pickupComponent->addedEffect, MANA_POTION_EFFECT_TEST);
    break;
  }
  case PickupType::Ammo: {
    EXPECT_EQ(pickupComponent->addedEffect, 0); // Doesn't have an addedEffect.
    break;
  }
  default:
    break;
  }
}
