#pragma once
#include <vector>
#include "./ObserverIf.h"
#include "../../Common/Messages/InputMessage.h"

class ObservableIf
{
public:
  virtual ~ObservableIf();
  virtual void attach(std::shared_ptr<ObserverIf> subscriber);
  virtual void detach(std::shared_ptr<ObserverIf> subscriber);
  virtual void notify(InputMessage message);
  virtual void notify(int message, int ID);
protected:
  std::vector<std::shared_ptr<ObserverIf>> mObservers;
};