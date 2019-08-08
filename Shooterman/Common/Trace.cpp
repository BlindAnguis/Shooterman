#include "Trace.h"

#include <queue>

#include <SFML/Network/Packet.hpp>

#include "MessageId.h"
#include "MessageHandler/Subscriber.h"
#include "MessageHandler/MessageHandler.h"
#include "Messages/AddDebugButtonMessage.h"
#include "Messages/ToggleDebugButtonMessage.h"
#include "Messages/RemoveDebugButtonMessage.h"

void Trace::setupDebugMessages(std::string category, std::string name) {
  if (!mDebugSubscriber) {
    mDebugSubscriber = new Subscriber();
  }
  mCategory = category;
  mButtonName = name;
  MessageHandler::get().subscribeToWithTimeout(category + "DebugMenu", mDebugSubscriber);
}

void Trace::handleDebugMessages() {
  if (!mDebugSubscriber) {
    return;
  }

  std::queue<sf::Packet> debugMessageQueue = mDebugSubscriber->getMessageQueue();
  sf::Packet debugMessage;
  while (!debugMessageQueue.empty()) {
    debugMessage = debugMessageQueue.front();
    debugMessageQueue.pop();

    int id = -1;
    debugMessage >> id;

    if (id == MessageId::TOGGLE_DEBUG_BUTTON) {
      ToggleDebugButtonMessage tdbm(debugMessage);
      switch (tdbm.getTraceLevel()) {
      case TRACE_LEVEL::DEB_1:
        mDebugEnabled1 = !mDebugEnabled1;
        break;
      case TRACE_LEVEL::DEB_2:
        mDebugEnabled2 = !mDebugEnabled2;
        break;
      case TRACE_LEVEL::DEB_3:
        mDebugEnabled3 = !mDebugEnabled3;
        break;
      case TRACE_LEVEL::DEB_4:
        mDebugEnabled4 = !mDebugEnabled4;
        break;
      case TRACE_LEVEL::SEND_REC:
        mSendRecEnabled = !mSendRecEnabled;
        break;
      case TRACE_LEVEL::FUNC:
        mFunctionEnabled = !mFunctionEnabled;
        break;
      }
    } else if (id == MessageId::SUBSCRIBE_DONE) {
      AddDebugButtonMessage debMess(mDebugSubscriber->getId(), mButtonName, mCategory);
      mDebugSubscriber->reverseSendMessage(debMess.pack());
    }
  }
}

void Trace::teardownDebugMessages() {
  if (mDebugSubscriber) {
    RemoveDebugButtonMessage rdbm(mDebugSubscriber->getId());
    sf::Packet packet = rdbm.pack();
    mDebugSubscriber->reverseSendMessage(packet);

    MessageHandler::get().unsubscribeTo(mCategory + "DebugMenu", mDebugSubscriber);

    delete mDebugSubscriber;
    mDebugSubscriber = nullptr;
  }
}