#pragma once

#include "Message.h"

class AddDebugButtonMessage : Message {
public:
  AddDebugButtonMessage(int subscriberId, std::string buttonText, std::string category, int secondarySubscriberId = -1);
  AddDebugButtonMessage(sf::Packet packet);
  ~AddDebugButtonMessage();

  sf::Packet pack();
  void unpack(sf::Packet packet);

  int getSubscriberId();
  int getSecondarySubscriberId();

  std::string getButtonText();
  std::string getCategoryText();

private:
  int mSubscruberId;
  int mSecondarySubscriberId;
  std::string mButtonText;
  std::string mCategoryText;
};