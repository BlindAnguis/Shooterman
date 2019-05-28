#include "Subscriber.h"

Subscriber::Subscriber() : mId(-1) {
  mName = "Subscriber without ID";
  mQueueLock = new std::mutex();
}

Subscriber::~Subscriber() {
  delete mQueueLock;
  TRACE_DEBUG1("Enter Destructor " << mName);
}

void Subscriber::sendMessage(sf::Packet message) {
  std::lock_guard<std::mutex> lockGuard(*mQueueLock);
  mMessageQueue.push(message);
}

std::queue<sf::Packet> Subscriber::getMessageQueue() {
  std::lock_guard<std::mutex> lockGuard(*mQueueLock);
  std::queue<sf::Packet> returnMessageQueue = mMessageQueue;
  std::queue<sf::Packet> empty;
  std::swap(mMessageQueue, empty);
  return returnMessageQueue;
}

int Subscriber::getId() { 
  return mId; 
}

void Subscriber::setId(int id) {
  if (mId == -1) {
    mName = "SUBSCRIBER " + std::to_string(id);
    mId = id;
  } else {
    TRACE_WARNING("Subscriber already have id " << mId << " cannot set it again to " << id);
  }
}

void Subscriber::setCallback(std::function<void(sf::Packet message)> callback) {
  mCallback = callback;
}

void Subscriber::reverseSendMessage(sf::Packet message) {
  if (mCallback) {
    mCallback(message);
  }
}