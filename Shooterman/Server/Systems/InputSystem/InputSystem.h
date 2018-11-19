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
  std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> mPlayersMap;
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
  void setPlayers(std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> playersMap) { mPlayersMap = playersMap; }
  GAME_STATE getLatestGameStateMessage();
};
