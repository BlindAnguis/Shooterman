#include "PrivateCommunication.h"

PrivateCommunication::PrivateCommunication() {}
PrivateCommunication::~PrivateCommunication() {}

void PrivateCommunication::subscribe(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  //MessageHandler::get().tryToGiveId(newSubscriber);
  newSubscriber->setId(100);
  mSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to game state list");
}

void PrivateCommunication::unsubscribe(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (auto it = mSubscriberList.begin(); it != mSubscriberList.end(); /**/) {
    if ((*it)->getId() == subscriber->getId()) {
      it = mSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << subscriber->getId() << ") from to game state list");
    } else {
      ++it;
    }
  }
}

void PrivateCommunication::pushMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (Subscriber* subscriber : mSubscriberList) {
    subscriber->sendMessage(message);
  }
}