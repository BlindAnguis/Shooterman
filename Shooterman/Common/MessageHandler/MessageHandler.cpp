#include "MessageHandler.h"

#include "../Messages/SubscribeDoneMessage.h"
#include "../Messages/SubscribeTimeoutMessage.h"

MessageHandler::MessageHandler() : mCurrentId(0), mRunning(true) {
  mName = "MESSAGEHANDLER";
  mMessageHandlerThread = std::make_unique<std::thread>(&MessageHandler::start, this);
}

MessageHandler::~MessageHandler() { }

void MessageHandler::start() {
  while (mRunning) {
    handleNewSubscribers();

    sf::sleep(sf::milliseconds(5));
  }
}

void MessageHandler::handleNewSubscribers() {
  std::lock_guard<std::mutex> lockGuard(mMutex);
  auto subscriberIt = mPendingSubscribers.begin();
  while (subscriberIt != mPendingSubscribers.end()) {
    auto interfaceIt = mPublishedInterfacesMap.find(subscriberIt->interfaceName);
    if (interfaceIt != mPublishedInterfacesMap.end()) {
      // Interface is published, subscribe to it
      interfaceIt->second->subscribe(subscriberIt->subscriber);
      SubscribeDoneMessage sdm;
      subscriberIt->subscriber->sendMessage(sdm.pack());
      subscriberIt = mPendingSubscribers.erase(subscriberIt);
    } else {
      // Interface is not yet published
      if (subscriberIt->timer->getElapsedTime() >= sf::milliseconds(subscriberIt->timeoutLength)) {
        // Subscribe timeout before interface is published
        SuscribeTimeoutMessage stm;
        subscriberIt->subscriber->sendMessage(stm.pack());
        subscriberIt = mPendingSubscribers.erase(subscriberIt);
      } else {
        // Let the subscriber wait until interface is published
        ++subscriberIt;
      }
    }
  }
}

void MessageHandler::tryToGiveId(Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mIdGeneratorLock);
  if (subscriber->getId() == -1) {
    subscriber->setId(mCurrentId);
    mCurrentId++;
  }
}

void MessageHandler::publishInterface(std::string name, Interface* interface) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto it = mPublishedInterfacesMap.find(name);
  if (it == mPublishedInterfacesMap.end()) {
    interface->setMName(name);
    mPublishedInterfacesMap.emplace(name, interface);
    TRACE_DEBUG1("Interface: " << name << " published");
  } else {
    TRACE_ERROR("Interface: " << name << " is already published");
  }
}

void MessageHandler::unpublishInterface(std::string name) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto interface = mPublishedInterfacesMap.find(name);
  if (interface != mPublishedInterfacesMap.end()) {
    while (interface->second->getSubscribers().size() > 0) {
      interface->second->unsubscribe(interface->second->getSubscribers().front());
    }
    mPublishedInterfacesMap.erase(interface);
    TRACE_DEBUG1("Interface: " << name << " unpublished");
  }
}

bool MessageHandler::subscribeTo(std::string name, Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto it = mPublishedInterfacesMap.find(name);
  if (it != mPublishedInterfacesMap.end()) {
    it->second->subscribe(subscriber);
    return true;
  }
  return false;
}

void MessageHandler::subscribeToWithTimeout(std::string interfaceName, Subscriber* subscriber, int timeoutLength) {
  std::lock_guard<std::mutex> lockGuard(mMutex);
  NewSubscriber newSubscriber;
  newSubscriber.interfaceName = interfaceName;
  newSubscriber.subscriber = subscriber;
  newSubscriber.timer = std::make_shared<sf::Clock>();
  newSubscriber.timeoutLength = timeoutLength;
  mPendingSubscribers.emplace_back(newSubscriber);
}

void MessageHandler::unsubscribeTo(std::string name, Subscriber* subscriber) {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  auto it = mPublishedInterfacesMap.find(name);
  if (it != mPublishedInterfacesMap.end()) {
    it->second->unsubscribe(subscriber);
  } else {
    TRACE_WARNING("Could not find " << name);
  }
}

std::list<std::string> MessageHandler::getPublishedInterfaces() {
  std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
  std::list<std::string> interfaceList;

  for (auto publishedInterface : mPublishedInterfacesMap) {
    std::string interfaceInfo = publishedInterface.first +  " (" + std::to_string(publishedInterface.second->getSubscribers().size()) +")";
    interfaceList.push_back(interfaceInfo);
  }

  return interfaceList;
}

void MessageHandler::shutdown() {
  mRunning = false;
  mMessageHandlerThread->join();
}
