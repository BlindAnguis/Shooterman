#pragma once
#include "../MovementSystem/MovementSystem.h"
#include "../ObservableIf.h"
#include "../../../Client/MessageHandler/MessageHandler.h"
#include ".././../../Common/Messages/GameStateMessage.h"

class InputSystem : public ObservableIf
{
private:
  Subscriber mInputSubscriber;
  Subscriber mGameStateSubscriber;
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
  GAME_STATE getLatestGameStateMessage();
};
