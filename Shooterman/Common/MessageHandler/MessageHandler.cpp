#include "MessageHandler.h"

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