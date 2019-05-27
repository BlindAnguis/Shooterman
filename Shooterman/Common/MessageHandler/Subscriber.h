#pragma once

#include <queue>
#include <mutex>

#include <SFML/Network/Packet.hpp>

#include "../Trace.h"

class Subscriber : protected Trace {
public:
  Subscriber();
  ~Subscriber();

  void sendMessage(sf::Packet message);
  std::queue<sf::Packet> getMessageQueue();

  int getId();
  void setId(int id);

  void setCallback(std::function<void(sf::Packet message)> callback);

  void reverseSendMessage(sf::Packet message);

private:
  int mId;
  std::queue<sf::Packet> mMessageQueue;
  std::mutex* mQueueLock;
  std::function<void(sf::Packet message)> mCallback = nullptr;
};