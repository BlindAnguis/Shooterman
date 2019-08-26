#pragma once

#include <queue>
#include <list>
#include <mutex>
#include <map>

#include <SFML/Network.hpp>

#include "../../Common/MessageHandler/Subscriber.h"

class Interface;

class MessageHandler {
public:
  virtual ~MessageHandler() {}

  virtual void publishInterface(std::string name, Interface* interface) = 0;
  virtual void unpublishInterface(std::string name) = 0;

  virtual bool subscribeTo(std::string name, Subscriber* subscriber) = 0;
  virtual void subscribeToWithTimeout(std::string interfaceName, Subscriber* subscriber, int timeoutLength = 5000) = 0;
  virtual void unsubscribeTo(std::string name, Subscriber* subscriber) = 0;

  virtual std::list<std::string> getPublishedInterfaces() = 0;
};

class MessageHandlerUtils {
public:
  static void tryToGiveId(Subscriber* subscriber) {
    static std::mutex mIdGeneratorLock;
    std::lock_guard<std::mutex> lockGuard(mIdGeneratorLock);
    static int currentId = 0;
    if (subscriber->getId() == -1) {
      subscriber->setId(currentId);
      currentId++;
    }
  }
};