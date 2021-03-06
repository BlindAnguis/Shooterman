#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiExpandableList.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common/MessageHandler/Interface.h"
#include "../../../Common/Messages/ToggleDebugButtonMessage.h"

class DebugMenu : public MenuBase {
public:
  DebugMenu(std::shared_ptr<MessageHandler> messageHandler);
  ~DebugMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Interface mIf;
  std::shared_ptr<MessageHandler> mMessageHandler;
  std::shared_ptr<GuiList> mGuiList;
  std::map<std::string, std::shared_ptr<GuiExpandableList>> mCategoriesMap;
  std::map<int, std::shared_ptr<GuiComponent>> mSubscirberToComponentMap;
  sf::Clock mInterfaceFetchTimer;

  void handleAddDebugButtonMessage(sf::Packet& message);
  void handleRemoveDebugButtonMessage(sf::Packet& message);
  
  void onToggleButton(TRACE_LEVEL traceLevel, int subscriberId, int secondarySubscriberId);
};

