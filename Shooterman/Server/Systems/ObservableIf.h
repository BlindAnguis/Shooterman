#pragma once
#include <vector>
#include "./ObserverIf.h"

class ObservableIf
{
public:
  virtual ~ObservableIf();
  virtual void attach(ObserverIf* subscriber);
  virtual void detach(ObserverIf* subscriber);
  virtual void notify(int message, int ID);
private:
  std::vector<ObserverIf*> mObservers;
};