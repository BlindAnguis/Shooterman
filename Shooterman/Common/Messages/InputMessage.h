#pragma once
#include "Message.h"
#include "../MessageId.h"

class InputMessage : Message {
public:
  InputMessage();
  InputMessage(std::uint32_t keyboardBitMask);
  InputMessage(std::uint32_t keyboardBitMask, int mouseX, int mouseY, int clientId = -1);
  InputMessage(sf::Packet packet);
  ~InputMessage();

  sf::Packet pack();
  void unpack(sf::Packet packet);

  std::uint32_t getKeyboardBitMask();
  sf::Vector2i getMousePosition();
  int getId();

  void setClientId(int clientId);

private:
  int mClientId;
  std::uint32_t mKeyboardBitMask;
  int mMouseX;
  int mMouseY;
};