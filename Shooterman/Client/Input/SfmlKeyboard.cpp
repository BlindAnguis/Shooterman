#include "SfmlKeyboard.h"

#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>

SfmlKeyboard::SfmlKeyboard() {}


SfmlKeyboard::~SfmlKeyboard() {}

bool SfmlKeyboard::isKeyPressed(KEY key) {
  switch (key) {
  case LEFT:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Left);
  case RIGHT:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Right);
  case UP:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Up);
  case DOWN:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Down);
  case SPACE:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
  case A:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::A);
  case D:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::D);
  case S:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::S);
  case W:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::W);
  case ESC:
    return sf::Keyboard::isKeyPressed(sf::Keyboard::Escape);
  case LEFT_MOUSE:
    return sf::Mouse::isButtonPressed(sf::Mouse::Left);
  case RIGHT_MOUSE:
    return sf::Mouse::isButtonPressed(sf::Mouse::Right);
  default:
    return false;
  }
}
