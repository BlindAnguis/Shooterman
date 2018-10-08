#pragma once

#include <queue>
#include <list>
#include <mutex>

#include <SFML/Network.hpp>

#include "../../Common/Subscriber.h"
#include "../../Common/Trace.h"

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

private:
  MessageHandler() : mCurrentId(0) { mName = "MESSAGEHANDLER"; }

  void tryToGiveId(Subscriber* subscriber);

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
};