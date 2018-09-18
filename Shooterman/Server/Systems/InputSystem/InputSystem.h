#pragma once
#include "../MovementSystem/MovementSystem.h"
#include "../ObservableIf.h"
#include "../../../Client/MessageHandler/MessageHandler.h"
#include ".././../../Common/Messages/GameStateMessage.h"

class InputSystem : public ObservableIf, Trace
{
private:
  Subscriber mInputSubscriber;
  Subscriber mGameStateSubscriber;
  GAME_STATE mCurrentGameState;
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
