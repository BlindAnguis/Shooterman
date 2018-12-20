#include "PrivateCommunication.h"
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

PrivateCommunication::PrivateCommunication() {
  mName = "";
}
PrivateCommunication::~PrivateCommunication() {}

void PrivateCommunication::subscribe(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  //MessageHandler::get().tryToGiveId(newSubscriber);
  srand((unsigned int)(time(NULL)));
  newSubscriber->setId(rand() % 10);
  mSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to " << mName);
}

void PrivateCommunication::unsubscribe(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (auto it = mSubscriberList.begin(); it != mSubscriberList.end(); /**/) {
    if ((*it)->getId() == subscriber->getId()) {
      it = mSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << subscriber->getId() << ") from " << mName);
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

void PrivateCommunication::setMName(std::string newName) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  mName = newName;
}

unsigned int PrivateCommunication::getNumberOfSubscribers() {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  return mSubscriberList.size();
}