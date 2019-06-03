#pragma once

#include <queue>
#include <list>
#include <mutex>
#include <map>

#include <SFML/Network.hpp>

#include "Interface.h"
#include "../../Common/MessageHandler/Subscriber.h"
#include "../../Common/Trace.h"

class Interface;

class MessageHandler : Trace {
public:
  static MessageHandler& get() {
    static MessageHandler instance;
    return instance;
  }

  void subscribeToSystemMessages(Subscriber* newSubscriber);
  void unSubscribeToSystemMessages(Subscriber* newSubscriber);
  void pushSystemMessage(sf::Packet message);

  void subscribeToSoundMessages(Subscriber* newSubscriber);
  void unSubscribeToSoundMessages(Subscriber* newSubscriber);
  void pushSoundMessage(sf::Packet message);

  void unsubscribeAll(Subscriber* subscriber);
  void tryToGiveId(Subscriber* subscriber);

  void publishInterface(std::string name, Interface* pc);
  void unpublishInterface(std::string name);

  bool subscribeTo(std::string name, Subscriber* s);
  void unsubscribeTo(std::string name, Subscriber* s);

  std::list<std::string> getPublishedInterfaces();

private:
  MessageHandler() : mCurrentId(0) { mName = "MESSAGEHANDLER"; mDebugEnabled1 = false; }
  
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

  std::map<std::string, Interface*> mPublishedInterfacesMap;
};