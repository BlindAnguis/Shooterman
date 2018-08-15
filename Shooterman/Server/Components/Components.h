#pragma once

#include "SFML/Graphics.hpp"

enum class ComponentType {
  Position,
  Render,
  Velocity,
  Collision,
  Solid
};

// TODO: Correct these components and start using these together with the general ComponentManager everywhere!
// TODO: Do I have a use for ComponentType?
struct PositionComponent {
  sf::Vector2f position;
};

struct RenderComponent {
  sf::Texture texture;
  sf::Int16 textureId;
  sf::Sprite sprite;
  bool visible;
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
