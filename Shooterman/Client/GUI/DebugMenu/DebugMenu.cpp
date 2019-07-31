#include "DebugMenu.h"

#include "../Resources/GuiToggleButton.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/AddDebugButtonMessage.h"
#include "../../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../../Common/MessageHandler/Interface.h"

DebugMenu::DebugMenu() {
  mName = "CLIENT: DEBUG_MENU";
  mIf.addSignalCallback(MessageId::ADD_DEBUG_BUTTON, std::bind(&DebugMenu::handleAddDebugButtonMessage, this, std::placeholders::_1));
  mIf.addSignalCallback(MessageId::REMOVE_DEBUG_BUTTON, std::bind(&DebugMenu::handleRemoveDebugButtonMessage, this, std::placeholders::_1));

  mGuiFrame = std::make_shared<Frame>();
  mGuiList = std::make_shared<GuiList>(GuiComponentPosition::TOP_LEFT, GuiListDirection::VERTICAL);
  mGuiFrame->addGuiComponent(mGuiList);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_DEBUG_MENU, &mIf);
  mInterfaceFetchTimer.restart();

  auto expandableList = std::make_shared<GuiExpandableList>(GuiComponentPosition::TOP_LEFT, "Interfaces");
  mCategoriesMap["Interfaces"] = expandableList;
  mGuiList->addGuiComponent(expandableList);
}

DebugMenu::~DebugMenu() {
  MessageHandler::get().unpublishInterface("ClientDebugMenu");
}

bool DebugMenu::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  mIf.handleMessages();

  if (mInterfaceFetchTimer.getElapsedTime() > sf::milliseconds(1000)) {
    mInterfaceFetchTimer.restart();

    if (mCategoriesMap["Interfaces"]->getNumberOfComponents() <= 1) {
      mCategoriesMap["Interfaces"]->addGuiComponent(mGuiList);
    } else {
      mCategoriesMap["Interfaces"]->clear();
    }

    for (std::string interfaceInfo : MessageHandler::get().getPublishedInterfaces()) {
      mCategoriesMap["Interfaces"]->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, interfaceInfo, 20));
    }
  }

  return MenuBase::render(window, mousePosition);
}

void DebugMenu::handleAddDebugButtonMessage(sf::Packet message) {
  AddDebugButtonMessage debMess(message);
  TRACE_DEBUG1("New button added for " << debMess.getSubscriberId());
  int subscriberId = debMess.getSubscriberId();
  int secondarySubscriberId = debMess.getSecondarySubscriberId();

  auto list = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::HORIZONTAL);
  auto text = std::make_shared<GuiText>(GuiComponentPosition::LEFT, debMess.getButtonText() + ":");

  auto button1 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb1", std::bind(&DebugMenu::onToggleButton, this, TRACE_LEVEL::DEB_1, subscriberId, secondarySubscriberId));
  auto button2 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb2", std::bind(&DebugMenu::onToggleButton, this, TRACE_LEVEL::DEB_2, subscriberId, secondarySubscriberId));
  auto button3 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb3", std::bind(&DebugMenu::onToggleButton, this, TRACE_LEVEL::DEB_3, subscriberId, secondarySubscriberId));
  auto button4 = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Deb4", std::bind(&DebugMenu::onToggleButton, this, TRACE_LEVEL::DEB_4, subscriberId, secondarySubscriberId));
  auto buttonSR = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Send/Rec", std::bind(&DebugMenu::onToggleButton, this, TRACE_LEVEL::SEND_REC, subscriberId, secondarySubscriberId));
  auto buttonF = std::make_shared<GuiToggleButton>(GuiComponentPosition::LEFT, "Fucntion", std::bind(&DebugMenu::onToggleButton, this, TRACE_LEVEL::FUNC, subscriberId, secondarySubscriberId));

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
}

void DebugMenu::handleRemoveDebugButtonMessage(sf::Packet message) {
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
}

void DebugMenu::onToggleButton(TRACE_LEVEL traceLevel, int subscriberId, int secondarySubscriberId) {
  AddDebugButtonMessage returnMess(subscriberId, "", "", secondarySubscriberId);
  ToggleDebugButtonMessage tdbm(traceLevel);
  if (secondarySubscriberId == -1) {
    mIf.pushMessageTo(tdbm.pack(), subscriberId);
  } else {
    mIf.pushMessageTo(tdbm.pack(), secondarySubscriberId);
  }
  TRACE_SEND("Send to " << subscriberId);
}
