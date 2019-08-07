#include "ClientMain.h"

#include "../Input/Input.h"
#include "../GUI/Gui.h"
#include "../Network/NetworkHandler.h"
#include "../../Common/Messages/ClientInternal/IpMessage.h"
#include "../../Server/Main/GameLoop.h"
#include "../../Common/Interfaces.h"
#include "../../Common/Sounds.h"
#include "../../Common/Messages/SoundMessage.h"
#include "../SoundSystem/SoundSystem.h"

#include "SFML/Audio.hpp"

ClientMain::ClientMain() {
  mName = "CLIENT: CLIENT_MAIN";
  TRACE_INFO("Starting...");

  mSystemMessageInterface.addSignalCallback(MessageId::SHUT_DOWN, std::bind(&ClientMain::handleShutdownMessage, this, std::placeholders::_1));
  mGameStateInterface.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&ClientMain::handleChangeGameStateMessage, this, std::placeholders::_1));

  MessageHandler::get().publishInterface(Interfaces::CLIENT_GAME_STATE, &mGameStateInterface);
  MessageHandler::get().publishInterface(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSystemMessageInterface);
  Input input;
  Gui gui;
  std::shared_ptr<GameLoop> server;
  NetworkHandler networkHandler;
  SoundSystem soundSystem;

  mServerStarted = false;
  bool networkHandlerStarted = false;
  mGameStateStack.push(GAME_STATE::MAIN_MENU);
  Interface ipInterface;
  bool sentIpMessage = false;

  TRACE_INFO("Starting complete");
  int counter = 0;

  while (mRunning) {
    switch (mGameStateStack.top()) {
      case GAME_STATE::MAIN_MENU: {
        // Stop Server
        if (mServerStarted) {
          server->stop();
          server = nullptr;
          mServerStarted = false;
        }
        if (networkHandlerStarted) {
          networkHandler.shutDown();
          networkHandlerStarted = false;
        }
        sentIpMessage = false;
        break; 
      }
      case GAME_STATE::LOBBY:
      {
        if (!mServerStarted) {
          server = std::make_shared<GameLoop>();
          server->start();
          mServerStarted = true;
        }
        if (!networkHandlerStarted) {
          MessageHandler::get().publishInterface(Interfaces::CLIENT_IP_LIST, &ipInterface);
          networkHandler.start();
          networkHandlerStarted = true;
        }
        if (!sentIpMessage && ipInterface.getNumberOfSubscribers() > 0) {
          IpMessage ipm(sf::IpAddress::getLocalAddress().toString(), 1337);
          ipInterface.pushMessage(ipm.pack());
          MessageHandler::get().unpublishInterface(Interfaces::CLIENT_IP_LIST);
          sentIpMessage = true;
        }
        break;
      }
      case GAME_STATE::CLIENT_LOBBY:
       break;
      case GAME_STATE::JOIN:
        if (!networkHandlerStarted) {
          networkHandler.start();
          networkHandlerStarted = true;
        }
        break;
	    case GAME_STATE::PLAYING:
		    break;
      case GAME_STATE::OPTIONS:
		    break;
      case GAME_STATE::PAUSE:
        break;
      case GAME_STATE::MAP_EDITOR:
        break;
      default:
        TRACE_ERROR("Unknown game state: " << mGameStateStack.top());
    }
    soundSystem.update();
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
    mGameStateInterface.handleMessages();
    mSystemMessageInterface.handleMessages();

  }
  input.shutDown();
  gui.shutDown();
  if (networkHandlerStarted) {
    networkHandler.shutDown();
  }
  if (mServerStarted) {
    server->stop();
    server = nullptr;
  }

  MessageHandler::get().unpublishInterface("ClientGameState");
  TRACE_INFO("Shutting down complete");
}

void ClientMain::handleChangeGameStateMessage(sf::Packet& message) {
  GameStateMessage gsm(message);

  if (gsm.getGameState() == GAME_STATE::PREVIOUS) {
    if (mGameStateStack.size() > 1) {
      mGameStateStack.pop();
    }
  } else if (gsm.getGameState() == GAME_STATE::MAIN_MENU) {
    mGameStateStack = std::stack<GAME_STATE>();
    mGameStateStack.push(GAME_STATE::MAIN_MENU);
  } else if (mGameStateStack.top() != gsm.getGameState()) {
    mGameStateStack.push(gsm.getGameState());
  } else {
    TRACE_DEBUG1("Already in game state " << gsm.getGameStateAsString());
    return;
  }

  gsm = GameStateMessage(mGameStateStack.top());
  TRACE_INFO(gsm.getGameStateAsString());
  mGameStateInterface.pushMessage(gsm.pack());
}

void ClientMain::handleShutdownMessage(sf::Packet& message) {
  sf::Packet packet;
  packet << MessageId::SHUT_DOWN;
  mSystemMessageInterface.pushMessage(packet);

  TRACE_INFO("Shutting down...");
  mRunning = false;
}
