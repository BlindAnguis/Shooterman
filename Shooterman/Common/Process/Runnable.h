#pragma once

#include <set>

#include "../MessageHandler/Subscriber.h"

class Runnable {
public:
  virtual ~Runnable() {};

  virtual void start() = 0;
  virtual void run() = 0;
  virtual void stop() = 0;

  void handleSubscribers();

protected:
  void startListenToSubscriber(Subscriber* subscirber);
  void stopListenToSubscriber(Subscriber* subscriber);

private:
  std::set<Subscriber*> mSubscribers;
};