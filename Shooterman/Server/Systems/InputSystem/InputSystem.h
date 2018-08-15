#pragma once
#include "../MovementSystem/MovementSystem.h"
#include "../ObservableIf.h"
#include "../../../Client/MessageHandler/MessageHandler.h"

class InputSystem : public ObservableIf
{
private:
  Subscriber mInputSubscriber;
public:
  /*
  static InputSystem& get() {
    static InputSystem instance;
    return instance;
  }
  */
  InputSystem();
  ~InputSystem();
	void handleInput();
  int getLatestInput();
};
