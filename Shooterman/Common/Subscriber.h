#ifndef SUBSCRIBER_H_
#define SUBSCRIBER_H_

#include <queue>
#include <mutex>

#include <SFML/Network.hpp>

class Subscriber {
public:
  Subscriber() {
    m_queueLock = new std::mutex();;
  }

  ~Subscriber() {
    delete m_queueLock;
  }

  void sendMessage(sf::Packet message) {
    m_queueLock->lock();
    m_messageQueue.push(message);
    m_queueLock->unlock();
  }

  std::queue<sf::Packet> getMessageQueue() {
    m_queueLock->lock();
    std::queue<sf::Packet> returnMessageQueue = m_messageQueue;
    std::queue<sf::Packet> empty;
    std::swap(m_messageQueue, empty);
    m_queueLock->unlock();
    return returnMessageQueue;
  }

private:
  std::queue<sf::Packet> m_messageQueue;
  std::mutex* m_queueLock;
};

#endif /* SUBSCRIBER_H_ */