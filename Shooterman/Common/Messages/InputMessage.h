#pragma once
#include "Message.h"
#include "../MessageId.h"

class InputMessage : Message {
public:
  InputMessage() : mKeyboardBitMask(0) {}
  InputMessage(std::uint32_t keyboardBitMask) : mKeyboardBitMask(keyboardBitMask) {}
  InputMessage(std::uint32_t keyboardBitMask, int mouseX, int mouseY, int clientId = -1) : mKeyboardBitMask(keyboardBitMask), mMouseX(mouseX), mMouseY(mouseY), mClientId(clientId) {}
  InputMessage(sf::Packet packet) {
    packet >> mKeyboardBitMask;
    packet >> mMouseX;
    packet >> mMouseY;
    packet >> mClientId;
  }

  ~InputMessage() {}

  sf::Packet pack() {
    sf::Packet packet;
    packet << INPUT_KEYS;
    packet << mKeyboardBitMask;
    packet << mMouseX;
    packet << mMouseY;
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mClientId;
    packet >> mKeyboardBitMask;
    packet >> mMouseX;
    packet >> mMouseY;
  }

  std::uint32_t getKeyboardBitMask() {
    return mKeyboardBitMask;
  }

  sf::Vector2i getMousePosition() {
    return sf::Vector2i(mMouseX, mMouseY);
  }

  int getId() {
    return mClientId;
  }

  void setClientId(int clientId) {
    mClientId = clientId;
  }

private:
  std::string mName = "TRACE";
  int mClientId;
  std::uint32_t mKeyboardBitMask;
  int mMouseX;
  int mMouseY;
};