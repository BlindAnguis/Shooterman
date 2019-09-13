#pragma once

#include <queue>
#include <mutex>
#include <map>

#include <SFML/Network/Packet.hpp>

#include "../Trace.h"

class Subscriber : protected Trace {
public:
  Subscriber(std::string name = "");
  ~Subscriber();

  void sendMessage(sf::Packet message);
  std::queue<sf::Packet> getMessageQueue();

  int getId();
  void setId(int id);

  void setCallback(std::function<void(sf::Packet message)> callback);

  void addSignalCallback(int signalId, std::function<void(sf::Packet& message)> callback);

  void handleMessages();
  void clearMessages();

  void reverseSendMessage(sf::Packet message);

private:
  int mId;
  std::queue<sf::Packet> mMessageQueue;
  std::mutex* mQueueLock;
  std::function<void(sf::Packet message)> mCallback = nullptr;
  std::map<int, std::function<void(sf::Packet& message)>> mMessageFunctionMap;
};