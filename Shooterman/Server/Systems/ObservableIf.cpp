#include "./ObservableIf.h"

ObservableIf::~ObservableIf() {
}
void ObservableIf::attach(ObserverIf* subscriber) {
  mObservers.push_back(subscriber);
}

void ObservableIf::detach(ObserverIf* subscriber) {
  // TODO: Remove subscriper from vector.
}

void ObservableIf::notify(InputMessage message, int ID) {
  for (auto subscriber : mObservers) {
    subscriber->update(message, ID);
  }
}

void ObservableIf::notify(int message, int ID) {
  for (auto subscriber : mObservers) {
    subscriber->update(message, ID);
  }
}
