#include "InputMessage.h"

InputMessage::InputMessage() : mKeyboardBitMask(0) { }
InputMessage::InputMessage(std::uint32_t keyboardBitMask) : mKeyboardBitMask(keyboardBitMask) { }
InputMessage::InputMessage(std::uint32_t keyboardBitMask, int mouseX, int mouseY, int clientId) 
  : mKeyboardBitMask(keyboardBitMask), mMouseX(mouseX), mMouseY(mouseY), mClientId(clientId) { }
InputMessage::InputMessage(sf::Packet packet) {
  packet >> mKeyboardBitMask;
  packet >> mMouseX;
  packet >> mMouseY;
  packet >> mClientId;
}

InputMessage::~InputMessage() {}

sf::Packet InputMessage::pack() {
  sf::Packet packet;
  packet << MessageId::INPUT_KEYS;
  packet << mKeyboardBitMask;
  packet << mMouseX;
  packet << mMouseY;
  return packet;
}

void InputMessage::unpack(sf::Packet packet) {
  packet >> mClientId;
  packet >> mKeyboardBitMask;
  packet >> mMouseX;
  packet >> mMouseY;
}

std::uint32_t InputMessage::getKeyboardBitMask() {
  return mKeyboardBitMask;
}

sf::Vector2i InputMessage::getMousePosition() {
  return sf::Vector2i(mMouseX, mMouseY);
}

int InputMessage::getId() {
  return mClientId;
}

void InputMessage::setClientId(int clientId) {
  mClientId = clientId;
}