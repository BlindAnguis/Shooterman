#include "GameLoop.h"

#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>

#include "../Engine/Engine.h"
#include "../Network/HostListener.h"
#include "../../Common/Messages/ServerReadyMessage.h"
#include "../../Common/Interfaces.h"

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
      { 11,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 },
      { 11,16,16,16,16,17,17,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,17,17,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,18,18,18,18,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,18,18,18,18,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,17,17,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,18,18,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,18,18,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,18,18,18,18,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,18,18,18,18,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,17,17,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,17,17,17,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 11,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,16,11 },
      { 10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10 }
    }
  };
  std::array<std::array<Textures, 32>, 32> gameMap2 = std::array<std::array<Textures, 32>, 32>();
  for (int y = 0; y < 32; ++y) {
    for (int x = 0; x < 32; ++x) {
      gameMap2[x][y] = (Textures)gameMap1[x][y];
    }
  }

  HostListener hostListener = HostListener();
  hostListener.startListening();
  mNetworkSystem->start();
  sf::Clock c;
  // send message to client, tell it to enable the button.
  Subscriber serverReadySubscriber;
  while (!MessageHandler::get().subscribeTo(Interfaces::SERVER_SERVER_READY, &serverReadySubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  ServerReadyMessage srm;
  serverReadySubscriber.reverseSendMessage(srm.pack());
  Engine world = Engine(gameMap2);
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
          if (hostListener.hasMapData()) {
            world.setMap(hostListener.getMapData());
          }
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
