#include "Interface.h"

Interface::Interface() {
  mName = "";
  mDebugEnabled1 = false;
}
Interface::~Interface() {
  TRACE_DEBUG1("Enter Destructor: " << mName);
}

void Interface::subscribe(Subscriber* newSubscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  MessageHandlerUtils::tryToGiveId(newSubscriber);
  mSubscriberList.push_back(newSubscriber);
  newSubscriber->setCallback([this](sf::Packet message) { sendMessage(message); });
  TRACE_DEBUG1("New subscriber (" << newSubscriber->getId() << ") added to " << mName);
}

void Interface::unsubscribe(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  for (auto it = mSubscriberList.begin(); it != mSubscriberList.end();) {
	  if ((*it) == NULL) {
	    it = mSubscriberList.erase(it);
      TRACE_ERROR("Removed subscriber that was null");
	  }
	  //nullcheck, if null someone forgot to remove its subsciption. Remove it and add TRACE informing about this.
	  if ((*it)->getId() == subscriber->getId()) {
      it = mSubscriberList.erase(it);
      TRACE_DEBUG1("Removed subscriber (" << subscriber->getId() << ") from " << mName);
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
  std::lock_guard<std::mutex> lockGuard(mSubscriberLock);
  return mSubscriberList;
}