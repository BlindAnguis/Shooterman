#include "./ObservableIf.h"

ObservableIf::~ObservableIf() {
}
void ObservableIf::attach(std::shared_ptr<ObserverIf> subscriber) {
  mObservers.push_back(subscriber);
}

void ObservableIf::detach(std::shared_ptr<ObserverIf> subscriber) {
  // TODO: Remove subscriper from vector.
}

void ObservableIf::notify(InputMessage message) {
  for (auto subscriber : mObservers) {
    subscriber->update(message);
  }
}

void ObservableIf::notify(int message, int ID) {
  for (auto subscriber : mObservers) {
    subscriber->update(message, ID);
  }
}
