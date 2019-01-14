#pragma once

#include <queue>
#include <mutex>
#include <string>

#include <SFML/Network.hpp>

#include "../Trace.h"

class Subscriber : protected Trace {
public:
  Subscriber() : mId(-1) {
    mName = "Subscriber without ID";
    mQueueLock = new std::mutex();
  }

  ~Subscriber() {
    delete mQueueLock;
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
      mName = "SUBSCRIBER " + std::to_string(id);
      mId = id;
    }
    else {
      TRACE_WARNING("Subscriber already have id " << mId << " cannot set it again to " << id);
    }
  }

  void setCallback(std::function<void(sf::Packet message)> callback) {
    mCallback = callback;
  }

  void reverseSendMessage(sf::Packet message) {
    if (mCallback) {
      mCallback(message);
    }
  }

private:
  int mId;
  std::queue<sf::Packet> mMessageQueue;
  std::mutex* mQueueLock;
  std::function<void(sf::Packet message)> mCallback = nullptr;
};