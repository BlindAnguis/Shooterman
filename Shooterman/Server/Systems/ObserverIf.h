#pragma once
class ObserverIf
{
public:
  virtual ~ObserverIf() {}
  virtual void update(int input) = 0;
};