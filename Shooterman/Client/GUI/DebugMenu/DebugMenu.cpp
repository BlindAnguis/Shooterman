#include "DebugMenu.h"

DebugMenu::DebugMenu() {
  mName = "CLIENT: DEBUG_MENU";

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
     /* auto newButton = GUIComponentBuilder::createCustomActionButtonWithColorClick(debMess.getButtonText(), 250, mCurrentYPos, callback);
      newButton->setRenderBounds(true);
      mComponentList.push_back(newButton);*/
      mCurrentYPos += 50;
    }
  }
  return MenuBase::render(window, mousePosition);
}
