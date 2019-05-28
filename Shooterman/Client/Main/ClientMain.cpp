#include "ClientMain.h"

#include "../Input/Input.h"
#include "../GUI/Gui.h"
#include "../Sound/Sound.h"
#include "../Network/NetworkHandler.h"
#include "../../Common/Messages/ClientInternal/IpMessage.h"
#include "../../Server/Main/GameLoop.h"
#include "../../Common/Sounds.h"
#include "../../Common/Messages/SoundMessage.h"

#include "SFML/Audio.hpp"

ClientMain::ClientMain() {
  mName = "CLIENT: CLIENT_MAIN";
  TRACE_INFO("Starting...");
  MessageHandler::get().publishInterface("ClientGameState", &gameStateInterface);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Subscriber soundSubcription;
  Input input;
  Gui gui;
  std::shared_ptr<GameLoop> server;
  NetworkHandler networkHandler;
  //Sound sound = Sound();
  std::map<Sounds, sf::Sound> soundMap;
  sf::SoundBuffer slashLong1Buffer;
  if (!slashLong1Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/swish_2.wav")) {
    TRACE_ERROR("Couldn't load Client/Resources/Sounds/SoundEffects/swish_2.wav");
  }
  else {
    TRACE_INFO("Loaded Client/Resources/Sounds/SoundEffects/swish_2.wav");
  }
  soundMap.emplace(Sounds::SlashLong1, sf::Sound(slashLong1Buffer));

  sf::SoundBuffer slashLong2Buffer;
  if (!slashLong2Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/swish_4.wav")) {
    TRACE_ERROR("Couldn't load Client/Resources/Sounds/SoundEffects/swish_4.wav");
  }
  else {
    TRACE_INFO("Loaded Client/Resources/Sounds/SoundEffects/swish_4.wav");
  }
  soundMap.emplace(Sounds::SlashLong2, sf::Sound(slashLong2Buffer));

  sf::SoundBuffer hit1Buffer;
  if (!hit1Buffer.loadFromFile("Client/Resources/Sounds/SoundEffects/fall.wav")) {
    TRACE_ERROR("Couldn't load Client/Resources/Sounds/SoundEffects/fall.wav");
  }
  else {
    TRACE_INFO("Loaded Client/Resources/Sounds/SoundEffects/fall.wav");
  }
  soundMap.emplace(Sounds::Hit1, sf::Sound(hit1Buffer));

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
        }
        sentIpMessage = false;
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
          while (!MessageHandler::get().subscribeTo("ClientSoundList", &soundSubcription)) {
            sf::sleep(sf::milliseconds(5));
          }
          TRACE_INFO("Subscribed to sounds");
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
	    case GAME_STATE::PLAYING: {
        auto sounds = soundSubcription.getMessageQueue();
        while (!sounds.empty()) {
          auto sound = sounds.front();
          sounds.pop();
          SoundMessage sm;
          sm.unpack(sound);
          for (auto i = 0; i < sm.getSize(); i++) {
            soundMap.at(sm.getSound(i)).play();
          }
        }

        /*
        if (counter == 60) {
          soundMap.at(Sounds::SlashLong1).play();
          soundMap.at(Sounds::SlashLong2).play();
        }
        if (counter >= 120) {
          soundMap.at(Sounds::SlashLong2).play();
          counter = 0;
        }
        counter++;*/
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
      GameStateMessage gsm;
      gsm.unpack(gameStateMessage);

      if (gsm.getGameState() == GAME_STATE::PREVIOUS) {
        mGameStateStack.pop();
      } else if (gsm.getGameState() == GAME_STATE::MAIN_MENU) {
        mGameStateStack = std::stack<GAME_STATE>();
        mGameStateStack.push(GAME_STATE::MAIN_MENU);
      } else if (mGameStateStack.top() != gsm.getGameState()) {
        mGameStateStack.push(gsm.getGameState());
      }

      gsm = GameStateMessage(mGameStateStack.top());
      gameStateInterface.pushMessage(gsm.pack());
    } else {
      TRACE_WARNING("Received unhandeled message with ID: " << id);
    }
  }
}