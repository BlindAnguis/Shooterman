#include "Trace.h"

#include <queue>

#include <SFML/Network/Packet.hpp>

#include "MessageId.h"
#include "MessageHandler/Subscriber.h"
#include "MessageHandler/MessageHandler.h"
#include "Messages/AddDebugButtonMessage.h"
#include "Messages/ToggleDebugButtonMessage.h"
#include "Messages/RemoveDebugButtonMessage.h"

bool Trace::setupDebugMessages(std::string category, std::string name) {
  if (!mDebugSubscriber) {
    mDebugSubscriber = new Subscriber();
  }
  mCategory = category;
  mIsSubscribed = MessageHandler::get().subscribeTo(category + "DebugMenu", mDebugSubscriber);
  if (mIsSubscribed) {
    AddDebugButtonMessage debMess(mDebugSubscriber->getId(), name, category);
    mDebugSubscriber->reverseSendMessage(debMess.pack());
  }
  return mIsSubscribed;
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

    if (id == TOGGLE_DEBUG_BUTTON) {
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
    mDebugSubscriber = false;
  }
}