#include "Runnable.h"

void Runnable::startListenToSubscriber(Subscriber* subscirber) {
  mSubscribers.insert(subscirber);
}

void Runnable::stopListenToSubscriber(Subscriber* subscriber) {
  mSubscribers.erase(subscriber);
}

void Runnable::handleSubscribers() {
  for (Subscriber* subscriber : mSubscribers) {
    subscriber->handleMessages();
  }
}
