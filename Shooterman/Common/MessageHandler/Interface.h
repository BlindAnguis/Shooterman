#pragma once

#include <list>
#include <mutex>

#include "../../Common/MessageHandler/Subscriber.h"
#include "../../Common/MessageHandler/MessageHandler.h"

class Interface : public Subscriber {
public:
  Interface();
  ~Interface();

  void subscribe(Subscriber* newSubscriber);

  void unsubscribe(Subscriber* subscriber);

  void pushMessage(sf::Packet message);

  void pushMessageTo(sf::Packet message, int subscriberId);

  void setMName(std::string newName);

  std::list<Subscriber*> getSubscribers();
  unsigned int getNumberOfSubscribers();

private:
  std::mutex mSubscriberLock;
  std::list<Subscriber*> mSubscriberList;
};