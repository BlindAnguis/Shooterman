#pragma once

#include <list>
#include <mutex>

#include "../../Common/MessageHandler/Subscriber.h"
#include "../../Common/Trace.h"

class Interface : Trace {
public:
  Interface();
  ~Interface();

  void subscribe(Subscriber* newSubscriber);

  void unsubscribe(Subscriber* subscriber);

  void pushMessage(sf::Packet message);

  void setMName(std::string newName);

  std::list<Subscriber*> getSubscribers();
  unsigned int getNumberOfSubscribers();

private:
  std::mutex mSubscriberLock;
  std::list<Subscriber*> mSubscriberList;
};