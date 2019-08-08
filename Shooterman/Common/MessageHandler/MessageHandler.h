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

struct NewSubscriber {
  std::string interfaceName;
  Subscriber* subscriber;
  std::shared_ptr<sf::Clock> timer;
  int timeoutLength;
};

class MessageHandler : Trace {
public:
  static MessageHandler& get() {
    static MessageHandler instance;
    return instance;
  }

  void tryToGiveId(Subscriber* subscriber);

  void publishInterface(std::string name, Interface* interface);
  void unpublishInterface(std::string name);

  bool subscribeTo(std::string name, Subscriber* subscriber);
  void subscribeToWithTimeout(std::string interfaceName, Subscriber* subscriber, int timeoutLength = 5000);
  void unsubscribeTo(std::string name, Subscriber* subscriber);

  std::list<std::string> getPublishedInterfaces();

  void shutdown();

private:
  bool mRunning;
  int mCurrentId;
  std::mutex mIdGeneratorLock;
  std::mutex mGameStateSubscriberLock;
  std::mutex mMutex;

  std::unique_ptr<std::thread> mMessageHandlerThread;

  std::map<std::string, Interface*> mPublishedInterfacesMap;
  std::list<NewSubscriber> mPendingSubscribers;

  MessageHandler();
  ~MessageHandler();

  void start();
  void handleNewSubscribers();
};