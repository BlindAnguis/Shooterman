#pragma once

#include "SFML/Graphics.hpp"
#include "../../Common/Animations.h"
#include "../../Common/Textures.h"

enum class ComponentType {
  Position,
  Render,
  Velocity,
  Collision,
  Solid,
  Animated,
};

struct PositionComponent {
  sf::Vector2f position;
};

struct RenderComponent {
  sf::Texture texture;
  Textures textureId;
  sf::Sprite sprite;
  bool visible;
  bool isPlayer;
};

struct VelocityComponent {
  sf::Vector2f maxVelocity;
  sf::Vector2f currentVelocity;
  bool moveOnce; // Use this to know if the velocity should be reset to 0 after a movement has been made (for instance for a player who only should move if a key is pressed).
};

struct CollisionComponent {
  bool collided;
};

struct SolidComponent {
  bool solid;
};

struct AnimationComponent {
  Animations animation;
  sf::Int16 animationFrame;
};
