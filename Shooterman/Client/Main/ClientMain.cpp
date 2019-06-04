#include "ClientMain.h"

#include "../Input/Input.h"
#include "../GUI/Gui.h"
#include "../Sound/Sound.h"
#include "../Network/NetworkHandler.h"
#include "../../Common/Messages/ClientInternal/IpMessage.h"
#include "../../Server/Main/GameLoop.h"
#include "../../Common/Sounds.h"
#include "../../Common/Messages/SoundMessage.h"
#include "../SoundSystem/SoundSystem.h"

#include "SFML/Audio.hpp"

ClientMain::ClientMain() {
  mName = "CLIENT: CLIENT_MAIN";
  TRACE_INFO("Starting...");
  MessageHandler::get().publishInterface("ClientGameState", &gameStateInterface);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Input input;
  Gui gui;
  std::shared_ptr<GameLoop> server;
  NetworkHandler networkHandler;
  SoundSystem soundSystem;

  mServerStarted = false;
  bool networkHandlerStarted = false;
  mGameStateStack.push(GAME_STATE::MAIN_MENU);
  Interface pc;
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
          soundSystem.unsubscribeToSoundList();
        }
        sentIpMessage = false;
        if (!soundSystem.isBackgroundMusicPlaying(Sounds::MainMenuBackgroundSong)) {
          soundSystem.stopBackgroundMusic();
          soundSystem.setBackgroundMusic(Sounds::MainMenuBackgroundSong);
          soundSystem.startBackGroundMusic();
        }
        break; 
      }
      case GAME_STATE::LOBBY: {
        // In the lobby
        if (!mServerStarted) {
          server = std::make_shared<GameLoop>();
          server->start();
          mServerStarted = true;
        }
        if (!networkHandlerStarted) {
          MessageHandler::get().publishInterface("ClientIpList", &pc);
          networkHandler.start();
          networkHandlerStarted = true;
        }
        if (!sentIpMessage && pc.getNumberOfSubscribers() > 0) {
          IpMessage ipm(sf::IpAddress::getLocalAddress().toString(), 1337);
          pc.pushMessage(ipm.pack());
          MessageHandler::get().unpublishInterface("ClientIpList");
          sentIpMessage = true;
        }
        if (!soundSystem.isBackgroundMusicPlaying(Sounds::LobbyBackgroundSong)) {
          soundSystem.stopBackgroundMusic();
          soundSystem.setBackgroundMusic(Sounds::LobbyBackgroundSong);
          soundSystem.startBackGroundMusic();
        }
        break; 
      }
      case GAME_STATE::CLIENT_LOBBY:
      {
        if (!soundSystem.isBackgroundMusicPlaying(Sounds::LobbyBackgroundSong)) {
          soundSystem.stopBackgroundMusic();
          soundSystem.setBackgroundMusic(Sounds::LobbyBackgroundSong);
          soundSystem.startBackGroundMusic();
        }
       break;
      }
      case GAME_STATE::JOIN:
        if (!networkHandlerStarted) {
          networkHandler.start();
          networkHandlerStarted = true;
        }
        break;
	    case GAME_STATE::PLAYING: {
        if (!soundSystem.isBackgroundMusicPlaying(Sounds::PlayingBackgroundSong)) {
          soundSystem.stopBackgroundMusic();
          soundSystem.setBackgroundMusic(Sounds::PlayingBackgroundSong);
          soundSystem.startBackGroundMusic();
        }
		    break;
	      }
      case GAME_STATE::OPTIONS: {
        //TRACE_INFO("Setting GAME_STATE to GAME_STATE::OPTIONS");
		    break;
	      }
      case GAME_STATE::PAUSE: {
        break;
      }
      default:
        TRACE_ERROR("Unknown game state: " << mGameStateStack.top());
    }
    soundSystem.update();
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
    handleGameStateMessages();
    handleSystemMessages();

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
  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
  MessageHandler::get().unpublishInterface("ClientGameState");
  TRACE_INFO("Shutting down complete");
}

void ClientMain::handleSystemMessages() {
  std::queue<sf::Packet> systemMessageQueue = mSystemMessageSubscriber.getMessageQueue();
  sf::Packet systemMessage;
  while (!systemMessageQueue.empty()) {
    systemMessage = systemMessageQueue.front();
    systemMessageQueue.pop();

    auto messageId = 0;
    systemMessage >> messageId;
    switch (messageId) {
    case SHUT_DOWN:
      TRACE_INFO("Shutting down...");
      mRunning = false;
      break;
    default:
      TRACE_WARNING("Unknown system message: " << messageId);
    }
  }
}

void ClientMain::handleGameStateMessages() {
  std::queue<sf::Packet> gameStateMessageQueue = gameStateInterface.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    gameStateMessageQueue.pop();

    int id = -1;
    gameStateMessage >> id;
    if (id == CHANGE_GAME_STATE) {
      GameStateMessage gsm(gameStateMessage);

      if (gsm.getGameState() == GAME_STATE::PREVIOUS) {
        mGameStateStack.pop();
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
      gameStateInterface.pushMessage(gsm.pack());
    } else {
      TRACE_WARNING("Received unhandeled message with ID: " << id);
    }
  }
}
