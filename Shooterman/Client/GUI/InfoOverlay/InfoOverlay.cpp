#include "InfoOverlay.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/InfoMessage.h"

InfoOverlay::InfoOverlay() {
  mName = "CLIENT: INFO_OVERLAY";
  mDebugEnabled1 = true;
  publishInfoMessagesInterface();
  mGuiFrame = std::make_shared<Frame>();

  mTextBox = GCF::createTextBox(GuiComponentPosition::BOTTOM_LEFT, "Test infobox");
  mGuiFrame->addGuiComponent(mTextBox);
}

InfoOverlay::~InfoOverlay() { }

void InfoOverlay::publishInfoMessagesInterface() {
  TRACE_FUNC_ENTER();
  TRACE_INFO("publishing infomessageInterface");
  MessageHandler::get().publishInterface("InfoMessage", &mInfoMessageInterface);
  TRACE_FUNC_EXIT();
}

void InfoOverlay::handleInfoMessages() {
  TRACE_FUNC_ENTER();
  std::queue<sf::Packet> infoMessageQueue = mInfoMessageInterface.getMessageQueue();
  sf::Packet infoMessage;
  while (!infoMessageQueue.empty()) {
    infoMessage = infoMessageQueue.front();
    infoMessageQueue.pop();

    int id = -1;
    infoMessage >> id;

    if (id == INFO_MESSAGE) {
      InfoMessage msg(infoMessage);
      mTextBox->setText(msg.getMessage());
      mNumberOfSecToShowMsg = msg.getMsgDuration();
      mClock.restart();
	  
    }
    else {
      TRACE_WARNING("Received unexpected message with ID : " << id);
    }
  }
  TRACE_FUNC_EXIT();
}

bool InfoOverlay::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  handleInfoMessages();
  if (mClock.getElapsedTime() < sf::seconds((float)mNumberOfSecToShowMsg)) {
    mGuiFrame->render(window);
  }
  return true;
}