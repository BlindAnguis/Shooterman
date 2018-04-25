#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <queue>
#include <list>
#include <mutex>

#include <SFML/Network.hpp>

#include "Common/Subscriber.h"

class MessageHandler {
public:
  static MessageHandler& get() {
    static MessageHandler instance;
    return instance;
  }

  void subscribeToSystemMessages(Subscriber* newSubscriber);
  void unSubscribeToSystemMessages(Subscriber* newSubscriber);
  void pushSystemMessage(sf::Packet message);

  void subscribeToInputMessages(Subscriber* newSubscriber);
  void unSubscribeToInputMessages(Subscriber* newSubscriber);
  void pushInputMessage(sf::Packet message);

  void subscribeToSpriteListMessages(Subscriber* newSubscriber);
  void unSubscribeToSpriteListMessages(Subscriber* newSubscriber);
  void pushSpriteListMessage(sf::Packet message);

  void unsubscribeAll(Subscriber* subscriber);

private:
  MessageHandler() : mCurrentId(0) {}

  void tryToGiveId(Subscriber* subscriber);

  int mCurrentId;
  std::mutex mIdGeneratorLock;
  std::mutex mSystemSubscriberLock;
  std::mutex mInputSubscriberLock;
  std::mutex mSpriteListSubscriberLock;
  std::list<Subscriber*> mSystemSubscriberList;
  std::list<Subscriber*> mInputSubscriberList;
  std::list<Subscriber*> mSpriteListSubscriberList;
};
#endif // ifndef MESSAGE_HANDLER_H