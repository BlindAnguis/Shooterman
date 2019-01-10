#pragma once
#include "../MovementSystem/MovementSystem.h"
#include "../ObservableIf.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include ".././../../Common/Messages/GameStateMessage.h"
#include ".././../../Common/Messages/InputMessage.h"
#include "../../../Common/KeyBindings.h"

class InputSystem : public ObservableIf, Trace
{
private:
  Subscriber mInputSubscriber;
  Subscriber mGameStateSubscriber;
  GAME_STATE mCurrentGameState;
  std::function<void(int entityId, std::uint32_t input, sf::Vector2i mousePosition)> mAttack = nullptr;
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
  InputMessage getLatestInput();
  void setPlayers(std::shared_ptr<std::map<int, std::pair<sf::TcpSocket*, Entity*>>> playersMap) { mPlayersMap = playersMap; }
  void setAttackCallback(const std::function<void(int entityId, std::uint32_t input, sf::Vector2i mousePosition)>& attack) { mAttack = attack; }
  GAME_STATE getLatestGameStateMessage();
};
