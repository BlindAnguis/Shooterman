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

  void subscribeToSpriteListMessages(Subscriber* newSubscriber);
  void unSubscribeToSpriteListMessages(Subscriber* newSubscriber);
  void pushSpriteListMessage(sf::Packet message);

  void subscribeToMouseMessages(Subscriber* newSubscriber);
  void unSubscribeToMouseMessages(Subscriber* newSubscriber);
  void pushMouseMessage(sf::Packet message);

  void subscribeToSoundMessages(Subscriber* newSubscriber);
  void unSubscribeToSoundMessages(Subscriber* newSubscriber);
  void pushSoundMessage(sf::Packet message);

  void unsubscribeAll(Subscriber* subscriber);
  void tryToGiveId(Subscriber* subscriber);

  void publishInterface(std::string name, PrivateCommunication* pc) {
    std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
    pc->setMName(name);
    mPublishedComms.emplace(name, pc);
  }

  void unpublishInterface(std::string name) {
    std::lock_guard<std::mutex> lockGuard(mGameStateSubscriberLock);
    mPublishedComms.erase(name);
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
  MessageHandler() : mCurrentId(0) { mName = "MESSAGEHANDLER"; }
  
  int mCurrentId;
  std::mutex mIdGeneratorLock;
  std::mutex mSystemSubscriberLock;
  std::mutex mInputSubscriberLock;
  std::mutex mSpriteListSubscriberLock;
  std::mutex mGameStateSubscriberLock;
  std::mutex mMouseSubscriberLock;
  std::mutex mSoundSubscriberLock;
  std::list<Subscriber*> mSystemSubscriberList;
  std::list<Subscriber*> mInputSubscriberList;
  std::list<Subscriber*> mSpriteListSubscriberList;
  std::list<Subscriber*> mGameStateSubscriberList;
  std::list<Subscriber*> mMouseSubscriberList;
  std::list<Subscriber*> mSoundSubscriberList;

  std::map<std::string, PrivateCommunication*> mPublishedComms;
};