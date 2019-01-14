#include "Interface.h"

Interface::Interface() {
  mName = "";
  mDebugEnabled = true;
}
Interface::~Interface() {}

void Interface::subscribe(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  MessageHandler::get().tryToGiveId(newSubscriber);
  mSubscriberList.push_back(newSubscriber);
  newSubscriber->setCallback([this](sf::Packet message) { sendMessage(message); });
  TRACE_INFO("New subscriber (" << newSubscriber->getId() << ") added to " << mName);
}

void Interface::unsubscribe(Subscriber* subscriber) {
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

void Interface::pushMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (Subscriber* subscriber : mSubscriberList) {
    subscriber->sendMessage(message);
  }
}

void Interface::pushMessageTo(sf::Packet message, int subscriberId) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (Subscriber* subscriber : mSubscriberList) {
    if (subscriber->getId() == subscriberId) {
      subscriber->sendMessage(message);
    }
  }
}

void Interface::setMName(std::string newName) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  mName = newName;
}

unsigned int Interface::getNumberOfSubscribers() {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  return mSubscriberList.size();
}

std::list<Subscriber*> Interface::getSubscribers() {
  return mSubscriberList;
}