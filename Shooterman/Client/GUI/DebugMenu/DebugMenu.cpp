#include "DebugMenu.h"

#include "../Resources/GuiButton.h"

DebugMenu::DebugMenu() {
  mName = "CLIENT: DEBUG_MENU";
  mGuiFrame = std::make_shared<Frame>();
  mGuiList = std::make_shared<GuiList>(GuiComponentPosition::BOTTOM_RIGHT, GuiListDirection::VERTICAL);
  mGuiFrame->addGuiComponent(mGuiList);
  MessageHandler::get().publishInterface("ClientDebugMenu", &mIf);
}

DebugMenu::~DebugMenu() {
  MessageHandler::get().unpublishInterface("ClientDebugMenu");
}

bool DebugMenu::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  std::queue<sf::Packet> messageQueue = mIf.getMessageQueue();
  while (!messageQueue.empty()) {
    int messageId;
    auto message = messageQueue.front();
    messageQueue.pop();
    message >> messageId;

    if (messageId == ADD_DEBUG_BUTTON) {
      AddDebugButtonMessage debMess;
      debMess.unpack(message);
      TRACE_DEBUG("New button added for " << debMess.getSubscriberId());
      int subscriberId = debMess.getSubscriberId();
      int callbackId = debMess.getCallbackId();
      auto callback = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", callbackId);
        mIf.pushMessageTo(returnMess.pack(), subscriberId);
        TRACE_DEBUG("Send to " << subscriberId);
      };

      auto newButton = std::make_shared<GuiButton>(GuiComponentPosition::RIGHT, debMess.getButtonText(), callback);
      mGuiList->addGuiComponent(newButton);
    }
  }
  return MenuBase::render(window, mousePosition);
}
