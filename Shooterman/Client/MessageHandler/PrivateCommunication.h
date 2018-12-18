#pragma once

#include <list>
#include <mutex>

#include "../../Common/Subscriber.h"
#include "../../Common/Trace.h"

class PrivateCommunication : Trace {
public:
  PrivateCommunication();
  ~PrivateCommunication();

  void subscribe(Subscriber* newSubscriber);

  void unsubscribe(Subscriber* subscriber);

  void pushMessage(sf::Packet message);

  void setMName(std::string newName);

  unsigned int getNumberOfSubscribers();

private:
  std::mutex mSubscriberLock;
  std::list<Subscriber*> mSubscriberList;
};