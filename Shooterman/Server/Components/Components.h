#pragma once

#include "SFML/Graphics.hpp"
#include "../../Common/AnimationType.h"
#include "../../Common/Textures.h"
#include "../../Common/Animation/Animation.h"

#include <map>
#include <set>

enum class ComponentType {
  Position,
  Render,
  Velocity,
  Collision,
  Solid,
  Animated,
};

enum class PlayerState {
  Idle,
  Running,
  Attacking,
  SuperAttacking,
  Dead
};

enum class PlayerClass {
  Mage,
  Knight,
  Archer,
  Spearman
};

enum class PickupType {
  HealthPotion,
  ManaPotion,
  Ammo,
};

struct PositionComponent {
  sf::Vector2f position;
};

struct RenderComponent {
  Textures textureId;
  sf::Sprite sprite;
  bool visible;
  bool isDynamic = false;
};

struct VelocityComponent {
  sf::Vector2f maxVelocity;
  sf::Vector2f currentVelocity;
  bool moveOnce; // Use this to know if the velocity should be reset to 0 after a movement has been made (for instance for a player who only should move if a key is pressed).
};

struct CollisionComponent {
  bool collided;
  std::vector<int> collidedList;
  bool destroyOnCollision;
};

struct AnimationComponent {
  AnimationType currentAnimation;
  std::map<AnimationType, Animation> animations;
};

struct HealthComponent {
  bool isAlive = false;
  int currentHealth = 0;
  int maxHealth = 0;
};

struct ManaComponent {
  int currentMana = 0;
  int maxMana = 0;
};

struct HealthChangerComponent {
  int healthChange = 0;
  std::set<int> immuneEntityIds;
};

struct ClockComponent {
  sf::Clock clock;
  int timeout = -1;
  std::function<void()> timeoutCallback = []() {};
};

struct PlayerComponent {
  PlayerState state;
  PlayerClass playerClass;
  bool invinsible = false;
  int attackSpeed;
  sf::Vector2i nextAttackMousePosition;
  int superAttacks = 0;
  int nrOfKills;
  int nrOfDeaths;
  int score;
};

struct PickupComponent {
  PickupType type;
  int addedEffect = 0;
};
