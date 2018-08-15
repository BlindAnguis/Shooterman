#pragma once
#include <vector>
#include "./ObserverIf.h"

class ObservableIf
{
public:
  virtual ~ObservableIf();
  virtual void attach(ObserverIf* subscriber);
  virtual void detach(ObserverIf* subscriber);
  virtual void notify(int message);
private:
  std::vector<ObserverIf*> mObservers;
};