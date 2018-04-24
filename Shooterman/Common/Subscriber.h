#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include <queue>
#include <mutex>

#include <SFML/Network.hpp>

class Subscriber {
public:
  Subscriber() {
    mQueueLock = new std::mutex();;
  }

  ~Subscriber() {
    delete mQueueLock;
  }

  void sendMessage(sf::Packet message) {
    mQueueLock->lock();
    mMessageQueue.push(message);
    mQueueLock->unlock();
  }

  std::queue<sf::Packet> getMessageQueue() {
    mQueueLock->lock();
    std::queue<sf::Packet> returnMessageQueue = mMessageQueue;
    std::queue<sf::Packet> empty;
    std::swap(mMessageQueue, empty);
    mQueueLock->unlock();
    return returnMessageQueue;
  }

private:
  std::queue<sf::Packet> mMessageQueue;
  std::mutex* mQueueLock;
};

#endif /* SUBSCRIBER_H_ */