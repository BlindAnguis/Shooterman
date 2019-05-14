#include "ClientMain.h"

ClientMain::ClientMain() {
  mName = "CLIENT: CLIENT_MAIN";
  TRACE_INFO("Starting...");
  MessageHandler::get().publishInterface("GameState", &gameStateInterface);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Input input;
  Gui gui;
  GameLoop server;
  NetworkHandler networkHandler;
  //Sound sound = Sound();


  mServerStarted = false;
  bool networkHandlerStarted = false;
  mGameStateStack.push(GAME_STATE::MAIN_MENU);
  Interface pc;
  bool sentIpMessage = false;

  TRACE_INFO("Starting complete");

  while (mRunning) {
    switch (mGameStateStack.top()) {
      case GAME_STATE::MAIN_MENU: {
        // Stop Server
        if (mServerStarted) {
          server.stop();
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
          server.start();
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

        break; 
      }
      case GAME_STATE::JOIN:
        if (!networkHandlerStarted) {
          networkHandler.start();
          networkHandlerStarted = true;
        }
        break;
	    case GAME_STATE::PLAYING: {
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
  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
  MessageHandler::get().unpublishInterface("GameState");
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

    GameStateMessage gsm;
    gsm.unpack(gameStateMessage);
	if (gsm.getGameState() == GAME_STATE::PREVIOUS) {
	  mGameStateStack.pop();
	}
	else if (gsm.getGameState() == GAME_STATE::MAIN_MENU) {
		mGameStateStack = std::stack<GAME_STATE>();
		mGameStateStack.push(GAME_STATE::MAIN_MENU);
	} 
	else {
	  mGameStateStack.push(gsm.getGameState());
	}
	gsm = GameStateMessage(mGameStateStack.top());
	gameStateInterface.pushMessage(gsm.pack());
  }
}