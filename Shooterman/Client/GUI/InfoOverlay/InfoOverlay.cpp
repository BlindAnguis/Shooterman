#include "InfoOverlay.h"

#include "../Resources/GuiList.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/InfoMessage.h"

InfoOverlay::InfoOverlay() {
  mName = "CLIENT: INFO_OVERLAY";
  mDebugEnabled1 = true;
  mInfoMessageInterface.addSignalCallback(MessageId::INFO_MESSAGE, std::bind(&InfoOverlay::handleInfoMessage, this, std::placeholders::_1));

  publishInfoMessagesInterface();

  mGuiFrame = std::make_shared<Frame>();
  mTextBox = GCF::createTextBox(GuiComponentPosition::BOTTOM_LEFT, "Test infobox");
  mGuiFrame->addGuiComponent(mTextBox);

  setupDebugMessages("Client", "Join Menu");
}

InfoOverlay::~InfoOverlay() { }

void InfoOverlay::backgroundUpdate() {
  handleDebugMessages();
}

bool InfoOverlay::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  mInfoMessageInterface.handleMessages();
  if (mClock.getElapsedTime() < sf::seconds((float)mNumberOfSecToShowMsg)) {
    mGuiFrame->render(window);
  }
  return true;
}

void InfoOverlay::publishInfoMessagesInterface() {
  TRACE_FUNC_ENTER();
  TRACE_INFO("publishing infomessageInterface");
  MessageHandler::get().publishInterface(Interfaces::INFO_MESSAGE, &mInfoMessageInterface);
  TRACE_FUNC_EXIT();
}

void InfoOverlay::handleInfoMessage(sf::Packet& message) {
  TRACE_FUNC_ENTER();
  InfoMessage msg(message);
  mTextBox->setText(msg.getMessage());
  TRACE_REC("INFO_MESSAGE: " << msg.getMessage() << ", will be shown for: " << msg.getMsgDuration() << "s.");
  mNumberOfSecToShowMsg = msg.getMsgDuration();
  mClock.restart();
  TRACE_FUNC_EXIT();
}