#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include <list>
#include <array>

#include "GameLoop.h"
#include "../../Common/KeyBindings.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../EntityManager/EntityManager.h"
#include "../Systems/InputSystem/InputSystem.h"
#include "../Engine/Engine.h"
#include "../../Common/Messages/GameStateMessage.h"
#include "../Network/HostListener.h"

GameLoop::GameLoop() {
  mName = "SERVER: GAME_LOOP";
  mDebugEnabled = false;
}

void GameLoop::start() {
  TRACE_INFO("Start Server");
  mGameLoopThread = new std::thread(&GameLoop::gameLoop, this);
}

void GameLoop::stop() {
  TRACE_INFO("Exit Server");
  
  mNetworkSystem->shutDown();
  mRunning = false;
  mGameLoopThread->join();
  delete mGameLoopThread;
  TRACE_INFO("Server finished");

}

void GameLoop::gameLoop() {
  mNetworkSystem = std::make_shared<NetworkSystem>();
  GAME_STATE state;
  mRunning = true;
  TRACE_INFO("Gameloop started");
  sf::Packet movePacket;
  sf::CircleShape shape(50.f);
  shape.setPosition(200, 200);
  float velocityX = 5;
  float velocityY = 5;
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
  Engine world = Engine(gameMap1, mNetworkSystem);
  //world.createMap();
  /*Entity* player1 = world.createPlayer(50, 50, 5, 5, 100);
  Entity* player2 = world.createPlayer(200, 200, -2, -2, 100);*/
  //Entity* ball1 = world.createBall(100, 100, 1, 1);
  InputMessage input = -1;
  HostListener hostListener = HostListener();
  std::list<sf::TcpSocket*> clients;
  hostListener.startListening();
  sf::Clock c;
  mNetworkSystem->start();
  while (mRunning) {
    c.restart();
    state = world.getInputSystem()->getLatestGameStateMessage();

    switch (state) {
      case GAME_STATE::LOBBY:
        input = world.getInputSystem()->getLatestInput();
        //TRACE_INFO("In the LOBBY");
        //if (input == A_KEY) {
        //TRACE_INFO("Setting GameLoopState to SETUP_GAME");
        //state = GAME_STATE::SETUP_GAME;
        //}
        break;

      case GAME_STATE::SETUP_GAME:
        TRACE_INFO("Setting GameLoopState to PLAYING");
        state = GAME_STATE::PLAYING;
        //std::cout << "Game setup finished" << std::endl;
        /*
        clients = hostListener.stopListening();
        TRACE_INFO("Clients connected:")
        for (auto client : clients) {
          TRACE_INFO("Client: " << client->getLocalPort());
        }
        */
        break;

      case GAME_STATE::PLAYING:
        if (hostListener.isListening()) {
          auto tempMap = hostListener.stopListening();
          world.setConnectedClients(tempMap);
          TRACE_INFO("Clients connected:")
          for (auto client : *world.getConnectedClients()) {
            if (client.second->getSocket()) {
              TRACE_INFO("Client: " << client.second->getSocket()->getRemoteAddress());
              mNetworkSystem->addNewClientSocket(client.second->getSocket(), client.first);
            }
          }
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
        break;

      case GAME_STATE::OPTIONS:
        // TODO: add handling for options if in game
		    break;
      case GAME_STATE::PAUSE:
        break;
      default:
        TRACE_ERROR("This state doesn't exist " << state);
    }
    
    input = -1;
    int sleepTime = FRAME_LENGTH_IN_MS - c.getElapsedTime().asMilliseconds();
    if (sleepTime > 0) {
      sf::sleep(sf::milliseconds(sleepTime));
    }
    
  }  

  if (hostListener.isListening()) {
    hostListener.stopListening();
  }
}
