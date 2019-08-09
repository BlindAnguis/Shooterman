#pragma once

#include "../Resources/MenuBase.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../Resources/GuiText.h"

class InfoOverlay : public MenuBase {
public:
  InfoOverlay();
  ~InfoOverlay();

  void backgroundUpdate() override;
  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private: 
  sf::Clock mClock;
  Interface mInfoMessageInterface;
  std::shared_ptr<GuiText> mTextBox;
  int mNumberOfSecToShowMsg;

  void publishInfoMessagesInterface();
  void handleInfoMessage(sf::Packet& message);
};

