#pragma once
#include "Message.h"
#include "../MessageId.h"

class AddDebugButtonMessage : Message {
public:
  AddDebugButtonMessage(int subscriberId, std::string buttonText) : mSubscruberId(subscriberId), mButtonText(buttonText), mCallbackId(0) {}
  AddDebugButtonMessage(int subscriberId, std::string buttonText, int callbackId) : mSubscruberId(subscriberId), mButtonText(buttonText), mCallbackId(callbackId) {}
  AddDebugButtonMessage() : mSubscruberId(-1), mButtonText(""), mCallbackId(0) {}
  ~AddDebugButtonMessage() {}

  sf::Packet pack() {
    sf::Packet packet;
    packet << ADD_DEBUG_BUTTON;
    packet << mSubscruberId;
    packet << mButtonText;
    packet << mCallbackId;
    return packet;
  }

  void unpack(sf::Packet packet) {
    packet >> mSubscruberId;
    packet >> mButtonText;
    packet >> mCallbackId;
  }

  int getSubscriberId() {
    return mSubscruberId;
  }

  std::string getButtonText() {
    return mButtonText;
  }

  int getCallbackId() {
    return mCallbackId;
  }

private:
  int mSubscruberId;
  int mCallbackId;
  std::string mButtonText;
};