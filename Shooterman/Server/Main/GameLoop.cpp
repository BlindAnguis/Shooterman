#include "GameLoop.h"

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include "../Engine/Engine.h"
#include "../Network/HostListener.h"
#include "../../Common/Messages/ServerReadyMessage.h"

GameLoop::GameLoop() {
  mName = "SERVER: GAME_LOOP";
  mDebugEnabled1 = false;
}

void GameLoop::start() {
  TRACE_INFO("Start Server");
  mGameLoopThread = new std::thread(&GameLoop::gameLoop, this);
}

void GameLoop::stop() {
  TRACE_INFO("Exit Server");
  cleanUp();
  TRACE_INFO("Server finished");
}

void GameLoop::gameLoop() {
  mNetworkSystem = &NetworkSystem::get();
  GAME_STATE state;
  mRunning = true;
  TRACE_INFO("Gameloop started");
  std::array<std::array<int, 32>, 32> gameMap1 =
  {
    {
      { 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 },
      { 3,4,4,4,4,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,6,6,6,6,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,6,6,6,6,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,6,6,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,6,6,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,6,6,6,6,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,6,6,6,6,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,5,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 3,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,3 },
      { 1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1 }
    }
  };

  std::array<std::array<int, 16>, 16> gameMap2 =
  {
    {
      { 1,2,2,2,0,0,0,0,0,0,0,0,2,2,2,1 },
      { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
      { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 },
      { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
      { 3,0,0,0,0,0,0,0,0,0,0,0,0,0,0,3 },
      { 1,2,2,2,0,0,0,0,0,0,0,0,2,2,2,1 }
    }
  };

  std::array<std::array<int, 16>, 16> gameMap3 = {};

  HostListener hostListener = HostListener();
  hostListener.startListening();
  mNetworkSystem->start();
  sf::Clock c;
  // send message to client, tell it to enable the button.
  Subscriber serverReadySubscriber;
  while (!MessageHandler::get().subscribeTo("ServerServerReady", &serverReadySubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  ServerReadyMessage srm;
  serverReadySubscriber.reverseSendMessage(srm.pack());
  Engine world = Engine(gameMap1);
  while (mRunning) {
    c.restart();
    state = InputSystem::get().getLatestGameStateMessage();

    switch (state) {
      case GAME_STATE::LOBBY:
        //
        break;

      case GAME_STATE::SETUP_GAME:
        state = GAME_STATE::PLAYING;
        break;

      case GAME_STATE::PLAYING:
        if (hostListener.isListening()) {
          auto tempMap = hostListener.stopListening();
          world.setConnectedClients(tempMap);
          world.createPlayers();
          world.createMap();
        }
        world.update();
        world.collectPlayerData();
        break;

      case GAME_STATE::GAME_OVER:
        state = GAME_STATE::EXIT;
        break;

      case GAME_STATE::EXIT:
        state = GAME_STATE::LOBBY;
        break;

      case GAME_STATE::MAIN_MENU:
        if (hostListener.isListening()) {
          hostListener.stopListening();
        }
        world.shutDown();
        mNetworkSystem->shutDown();
        break;

      case GAME_STATE::OPTIONS:
        // TODO: add handling for options if in game
		    break;
      case GAME_STATE::PAUSE:
        break;
      default:
        TRACE_ERROR("This state doesn't exist " << state);
    }
    
    int sleepTime = FRAME_LENGTH_IN_MS - c.getElapsedTime().asMilliseconds();
    if (sleepTime > 0) {
      sf::sleep(sf::milliseconds(sleepTime));
    }
  }  

  if (hostListener.isListening()) {
    hostListener.stopListening();
  }
  InputSystem::get().resetSystem();
  world.shutDown();
  mNetworkSystem->shutDown();
}

void GameLoop::cleanUp() {
  mRunning = false;
  mGameLoopThread->join();
  delete mGameLoopThread;
}
