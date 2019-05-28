#include "DebugMenu.h"

#include "../Resources/GuiToggleButton.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"
#include "../../../Common/Messages/ToggleDebugButtonMessage.h"
#include "../../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../../Common/MessageHandler/Interface.h"

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
      TRACE_DEBUG1("New button added for " << debMess.getSubscriberId());
      int subscriberId = debMess.getSubscriberId();
      int secondarySubscriberId = debMess.getSecondarySubscriberId();

      auto list = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::HORIZONTAL);
      auto text = std::make_shared<GuiText>(GuiComponentPosition::LEFT, debMess.getButtonText() + ":");

      auto callback1 = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        ToggleDebugButtonMessage tdbm(TRACE_LEVEL::DEB_1);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(tdbm.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
        }
        TRACE_SEND("Send to " << subscriberId);
      };
      auto button1 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb1", callback1);

      auto callback2 = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        ToggleDebugButtonMessage tdbm(TRACE_LEVEL::DEB_2);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(tdbm.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
        }
        TRACE_SEND("Send to " << subscriberId);
      };
      auto button2 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb2", callback2);

      auto callback3 = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        ToggleDebugButtonMessage tdbm(TRACE_LEVEL::DEB_3);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(tdbm.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
        }
        TRACE_SEND("Send to " << subscriberId);
      };
      auto button3 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb3", callback3);

      auto callback4 = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        ToggleDebugButtonMessage tdbm(TRACE_LEVEL::DEB_4);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(tdbm.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
        }
        TRACE_SEND("Send to " << subscriberId);
      };
      auto button4 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb4", callback4);

      auto callbackSR = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        ToggleDebugButtonMessage tdbm(TRACE_LEVEL::SEND_REC);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(tdbm.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
        }
        TRACE_SEND("Send to " << subscriberId);
      };
      auto buttonSR = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Send/Rec", callbackSR);

      auto callbackF = [=]() {
        AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
        ToggleDebugButtonMessage tdbm(TRACE_LEVEL::FUNC);
        if (secondarySubscriberId == -1) {
          mIf.pushMessageTo(tdbm.pack(), subscriberId);
        } else {
          mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
        }
        TRACE_SEND("Send to " << subscriberId);
      };
      auto buttonF = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Fucntion", callbackF);

      list->addGuiComponent(text);
      list->addGuiComponent(button1);
      list->addGuiComponent(button2);
      list->addGuiComponent(button3);
      list->addGuiComponent(button4);
      list->addGuiComponent(buttonSR);
      list->addGuiComponent(buttonF);

      mSubscirberToComponentMap[subscriberId] = list;

      auto it = mCategoriesMap.find(debMess.getCategoryText());
      if (it == mCategoriesMap.end()) {
        auto expandableList = std::make_shared<GuiExpandableList>(GuiComponentPosition::TOP_LEFT, debMess.getCategoryText());
        mCategoriesMap[debMess.getCategoryText()] = expandableList;
        mGuiList->addGuiComponent(expandableList);
      }
      mCategoriesMap[debMess.getCategoryText()]->addGuiComponent(list);

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
