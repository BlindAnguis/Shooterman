#pragma once
#include <vector>
#include "./ObserverIf.h"
#include "../../Common/Messages/InputMessage.h"

class ObservableIf
{
public:
  virtual ~ObservableIf();
  virtual void attach(ObserverIf* subscriber);
  virtual void detach(ObserverIf* subscriber);
  virtual void notify(InputMessage message, int ID);
  virtual void notify(int message, int ID);
private:
  std::vector<ObserverIf*> mObservers;
};