#pragma once
class ObserverIf
{
public:
  virtual ~ObserverIf() {}
  virtual void update(int input, int ID) = 0;
};