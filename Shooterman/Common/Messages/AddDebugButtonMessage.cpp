#include "AddDebugButtonMessage.h"

#include "../MessageId.h"

AddDebugButtonMessage::AddDebugButtonMessage(int subscriberId, std::string buttonText, std::string category, int secondarySubscriberId)
  : mSubscruberId(subscriberId), mButtonText(buttonText), mCategoryText(category), mSecondarySubscriberId(secondarySubscriberId) {
}

AddDebugButtonMessage::AddDebugButtonMessage(sf::Packet packet) {
  unpack(packet);
}

AddDebugButtonMessage::~AddDebugButtonMessage() {
}

sf::Packet AddDebugButtonMessage::pack() {
  sf::Packet packet;
  packet << MessageId::ADD_DEBUG_BUTTON;
  packet << mSubscruberId;
  packet << mSecondarySubscriberId;
  packet << mButtonText;
  packet << mCategoryText;
  return packet;
}

void AddDebugButtonMessage::unpack(sf::Packet packet) {
  packet >> mSubscruberId;
  packet >> mSecondarySubscriberId;
  packet >> mButtonText;
  packet >> mCategoryText;
}

int AddDebugButtonMessage::getSubscriberId() {
  return mSubscruberId;
}

int AddDebugButtonMessage::getSecondarySubscriberId() {
  return mSecondarySubscriberId;
}

std::string AddDebugButtonMessage::getButtonText() {
  return mButtonText;
}

std::string AddDebugButtonMessage::getCategoryText() {
  return mCategoryText;
}