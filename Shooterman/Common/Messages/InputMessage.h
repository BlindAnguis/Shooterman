#pragma once
#include "Message.h"
#include "../MessageId.h"

class InputMessage : Message {
public:
  InputMessage() : mKeyboardBitMask(0) {}
  InputMessage(std::uint32_t keyboardBitMask) : mKeyboardBitMask(keyboardBitMask) {}
  InputMessage(std::uint32_t keyboardBitMask, int clientId) : mKeyboardBitMask(keyboardBitMask), mClientId(clientId) {}
  InputMessage(sf::Packet packet) {
    packet >> mClientId;
    packet >> mKeyboardBitMask;
  }

  ~InputMessage() {}

  sf::Packet pack() {
    sf::Packet packet;
    packet << mClientId;
    packet << mKeyboardBitMask;
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mClientId;
    packet >> mKeyboardBitMask;
  }

  std::uint32_t getKeyboardBitMask() {
    return mKeyboardBitMask;
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
};