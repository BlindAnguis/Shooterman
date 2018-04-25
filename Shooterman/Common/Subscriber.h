#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include <queue>
#include <mutex>
#include <iostream>

#include <SFML/Network.hpp>

//#include "Client/MessageHandler/MessageHandler.cpp"

class Subscriber {
public:
  Subscriber() : mId(-1) {
    mQueueLock = new std::mutex();
  }

  ~Subscriber() {
    delete mQueueLock;
    //MessageHandler::get().unsubscribeAll(this);
  }

  void sendMessage(sf::Packet message) {
    std::lock_guard<std::mutex> lockGuard(*mQueueLock);
    mMessageQueue.push(message);
  }

  std::queue<sf::Packet> getMessageQueue() {
    std::lock_guard<std::mutex> lockGuard(*mQueueLock);
    std::queue<sf::Packet> returnMessageQueue = mMessageQueue;
    std::queue<sf::Packet> empty;
    std::swap(mMessageQueue, empty);
    return returnMessageQueue;
  }

  int getId() { return mId; }

  void setId(int id) {
    if (mId == -1) {
      mId = id;
    } else {
      std::cout << "[SUBSCRIBER] WARNING! Subscriber already have id " << mId << " cannot set it again to " << id << std::endl;
    }
  }

private:
  int mId;
  std::queue<sf::Packet> mMessageQueue;
  std::mutex* mQueueLock;
};

#endif /* SUBSCRIBER_H_ */