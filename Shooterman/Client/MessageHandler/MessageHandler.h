#pragma once

#include <queue>
#include <list>
#include <mutex>
#include <map>

#include <SFML/Network.hpp>

#include "../../Client/MessageHandler/PrivateCommunication.h"
#include "../../Common/Subscriber.h"
#include "../../Common/Trace.h"

class PrivateCommunication;

class MessageHandler : Trace {
public:
  static MessageHandler& get() {
    static MessageHandler instance;
    return instance;
  }

  void subscribeToGameStateMessages(Subscriber* newSubscriber);
  void unSubscribeToGameStateMessages(Subscriber* newSubscriber);
  void pushGameStateMessage(sf::Packet message);

  void subscribeToSystemMessages(Subscriber* newSubscriber);
  void unSubscribeToSystemMessages(Subscriber* newSubscriber);
  void pushSystemMessage(sf::Packet message);

  void subscribeToInputMessages(Subscriber* newSubscriber);
  void unSubscribeToInputMessages(Subscriber* newSubscriber);
  void pushInputMessage(sf::Packet message);

  void subscribeToSoundMessages(Subscriber* newSubscriber);
  void unSubscribeToSoundMessages(Subscriber* newSubscriber);
  void pushSoundMessage(sf::Packet message);

  void unsubscribeAll(Subscriber* subscriber);
  void tryToGiveId(Subscriber* subscriber);

  void publishInterface(std::string name, PrivateCommunication* pc) {
    std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
    auto it = mPublishedComms.find(name);
    if (it == mPublishedComms.end()) {
      pc->setMName(name);
      mPublishedComms.emplace(name, pc);
      TRACE_DEBUG("Interface: " << name << " published");
    } else {
      TRACE_WARNING("Interface: " << name << " is already published");
    }
  }

  void unpublishInterface(std::string name) {
    std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
    auto interface = mPublishedComms.find(name);
    if (interface != mPublishedComms.end()) {
      while (interface->second->getSubscribers().size() > 0) {
        interface->second->unsubscribe(interface->second->getSubscribers().front());
      }
      mPublishedComms.erase(interface);
    }
  }

  bool subscribeTo(std::string name, Subscriber* s) {
    std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
    auto it = mPublishedComms.find(name);
    if (it != mPublishedComms.end()) {
      it->second->subscribe(s);
      return true;
    }
    return false;
  }

  void unsubscribeTo(std::string name, Subscriber* s) {
    std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
    auto it = mPublishedComms.find(name);
    if (it != mPublishedComms.end()) {
      it->second->unsubscribe(s);
    } else {
      TRACE_INFO("Could not find " << name);
    }
  }

private:
  MessageHandler() : mCurrentId(0) { mName = "MESSAGEHANDLER"; mDebugEnabled = true; }
  
  int mCurrentId;
  std::mutex mIdGeneratorLock;
  std::mutex mSystemSubscriberLock;
  std::mutex mInputSubscriberLock;
  std::mutex mGameStateSubscriberLock;
  std::mutex mSoundSubscriberLock;
  std::list<Subscriber*> mSystemSubscriberList;
  std::list<Subscriber*> mInputSubscriberList;
  std::list<Subscriber*> mGameStateSubscriberList;
  std::list<Subscriber*> mSoundSubscriberList;

  std::map<std::string, PrivateCommunication*> mPublishedComms;
};