#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "GameLoop.h"
#include "../../Common/KeyBindings.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../EntityManager/EntityManager.h"
#include "../Engine/Engine.h"
#include "../Systems/InputSystem/InputSystem.h"

GameLoop::GameLoop() {
  mName = "GAMELOOP";
  mDebugEnabled = false;
}

void GameLoop::start() {
  TRACE_INFO("[SERVER: GAME_LOOP] Start Server");
  mGameLoopThread = new std::thread(&GameLoop::gameLoop, this);
}

void GameLoop::stop() {
  TRACE_INFO("[SERVER: GAME_LOOP] Exit Server");
  mRunning = false;
  mGameLoopThread->join();
  delete mGameLoopThread;
  TRACE_INFO("[SERVER: GAME_LOOP] Server finished");
}

void GameLoop::gameLoop() {
  GameLoopState state = GameLoopState::LOBBY;
  mRunning = true;
  TRACE_INFO("[SERVER: GAME_LOOP] Gameloop started");
  sf::Packet movePacket;
  sf::CircleShape shape(50.f);
  shape.setPosition(200, 200);
  float velocityX = 5;
  float velocityY = 5;
  Engine world = Engine();
  Entity* player1 = world.createPlayer(50, 50, 5, 5, 100);
  Entity* player2 = world.createPlayer(200, 200, -2, -2, 100);
  Entity* ball1 = world.createBall(100, 100, 1, 1);
  int input = -1;

  while (mRunning) {
    switch (state) {
    case GameLoopState::LOBBY:
      input = world.getInputSystem()->getLatestInput();
      if (input == A_KEY) {
        TRACE_INFO("[SERVER: GAME_LOOP] Setting GameLoopState to SETUP_GAME");
        state = GameLoopState::SETUP_GAME;
      }
      break;

    case GameLoopState::SETUP_GAME:
      TRACE_INFO("[SERVER: GAME_LOOP] Setting GameLoopState to PLAYING");
      state = GameLoopState::PLAYING;
      //std::cout << "Game setup finished" << std::endl;
      break;

    case GameLoopState::PLAYING:
      world.update();
      break;

    case GameLoopState::GAME_OVER:
      state = GameLoopState::EXIT;
      break;

    case GameLoopState::EXIT:
      state = GameLoopState::LOBBY;
      break;

    default:
      TRACE_ERROR("[SERVER: GAME_LOOP] This state doesn't exist");
    }
    input = -1;
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }  
}
