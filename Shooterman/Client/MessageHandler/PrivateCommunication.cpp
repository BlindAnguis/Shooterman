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
  srand(time(NULL));
  newSubscriber->setId(rand() % 10);
  mSubscriberList.push_back(newSubscriber);
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to list");
}

void PrivateCommunication::unsubscribe(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (auto it = mSubscriberList.begin(); it != mSubscriberList.end(); /**/) {
    if ((*it)->getId() == subscriber->getId()) {
      it = mSubscriberList.erase(it);
      TRACE_INFO("Removed subscriber (" << subscriber->getId() << ") from to list");
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
  mName = newName;
}