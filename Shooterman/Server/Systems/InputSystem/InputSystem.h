#pragma once

#include "../MovementSystem/MovementSystem.h"
#include "../ObservableIf.h"
#include "../../Player.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include ".././../../Common/Messages/GameStateMessage.h"
#include ".././../../Common/Messages/InputMessage.h"
#include "../../../Common/KeyBindings.h"
#include "../../Components/ComponentManager.h"

class InputSystem : public ObservableIf, Trace
{
private:
  bool mIsSubscribedToGameState = false;
  bool mIsSubscribedToInput = false;
  Subscriber mInputSubscriber;
  Subscriber mGameStateSubscriber;
  std::shared_ptr<MessageHandler> mMessageHandler;
  GAME_STATE mCurrentGameState;
  std::function<void(int entityId, std::uint32_t input, sf::Vector2i mousePosition)> mAttack = nullptr;
  std::shared_ptr<std::map<int, Player*>> mPlayersMap;
  std::queue<sf::Packet> getInput();

public:
  InputSystem(std::shared_ptr<MessageHandler> messageHandler);
  ~InputSystem();
	void handleInput();
  InputMessage getLatestInput();
  void setPlayers(std::shared_ptr<std::map<int, Player*>> playersMap) { mPlayersMap = playersMap; }
  void setAttackCallback(const std::function<void(int entityId, std::uint32_t input, sf::Vector2i mousePosition)>& attack) { mAttack = attack; }
  GAME_STATE getLatestGameStateMessage();
  void resetSystem();
};
