#include "DebugMenu.h"

#include "../Resources/GuiButton.h"
#include "../../../Common/Messages/RemoveDebugButtonMessage.h"

DebugMenu::DebugMenu() {
  mName = "CLIENT: DEBUG_MENU";
  mGuiFrame = std::make_shared<Frame>();
  mGuiList = std::make_shared<GuiList>(GuiComponentPosition::TOP_LEFT, GuiListDirection::VERTICAL);
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
      AddDebugButtonMessage debMess(message);
      TRACE_DEBUG("New button added for " << debMess.getSubscriberId());
      int subscriberId = debMess.getSubscriberId();
      int secondarySubscriberId = debMess.getSecondarySubscriberId();
      auto callback = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(returnMess.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(returnMess.pack(), secondarySubscriberId);
        }
        TRACE_DEBUG("Send to " << subscriberId);
      };
      auto button = std::make_shared<GuiButton>(GuiComponentPosition::LEFT, debMess.getButtonText(), callback, 20, FONT::CLEAN);

      mSubscirberToComponentMap[subscriberId] = button;

      auto it = mCategoriesMap.find(debMess.getCategoryText());
      if (it == mCategoriesMap.end()) {
        auto expandableList = std::make_shared<GuiExpandableList>(GuiComponentPosition::TOP_LEFT, debMess.getCategoryText(), FONT::CLEAN);
        mCategoriesMap[debMess.getCategoryText()] = expandableList;
        mGuiList->addGuiComponent(expandableList);
      }
      mCategoriesMap[debMess.getCategoryText()]->addGuiComponent(button);

    } else if (messageId == REMOVE_DEBUG_BUTTON) {
      RemoveDebugButtonMessage rdbm(message);

      auto it = mCategoriesMap.begin();
      while (it != mCategoriesMap.end()) {
        it->second->removeGuiComponent(mSubscirberToComponentMap[rdbm.getSubscriberId()]);
        if (it->second->getNumberOfComponents() == 1) {
          mGuiList->removeGuiComponent(it->second);
          it = mCategoriesMap.erase(it);
        } else {
          ++it;
        }
      }
    } else {
      TRACE_WARNING("Received unhandeled message with id: " << messageId);
    }
  }
  return MenuBase::render(window, mousePosition);
}
