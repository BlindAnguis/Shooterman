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
    SpriteMessage sm;

    switch (state) {
    case GameLoopState::LOBBY:
      input = world.getInputSystem()->getLatestInput();
      if (input == A_KEY) {
        TRACE_INFO("[SERVER: GAME_LOOP] Setting GameLoopState to SETUP_GAME");
        state = GameLoopState::SETUP_GAME;
      }
      break;

    case GameLoopState::SETUP_GAME:
      //std::cout << "Setting up the game" << std::endl;
      shape.setFillColor(sf::Color::Green);
      sm.addSpriteData(33, shape.getPosition());
      movePacket = sm.pack();
      MessageHandler::get().pushSpriteListMessage(movePacket);
      movePacket.clear();
      TRACE_INFO("[SERVER: GAME_LOOP] Setting GameLoopState to PLAYING");
      state = GameLoopState::PLAYING;
      //std::cout << "Game setup finished" << std::endl;
      break;

    case GameLoopState::PLAYING:
      world.update();
      break;

    case GameLoopState::GAME_OVER:
      //std::cout << "Enter the game over screen" << std::endl;
      state = GameLoopState::EXIT;
      //std::cout << "Exit the game over screen" << std::endl;
      break;

    case GameLoopState::EXIT:
      //std::cout << "The game should exit" << std::endl;
      state = GameLoopState::LOBBY;
      //std::cout << "Exiting the game" << std::endl;
      break;

    default:
      TRACE_ERROR("[SERVER: GAME_LOOP] This state doesn't exist");
    }
    input = 0;
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }  
}

void GameLoop::moveCircle(sf::CircleShape &circle, float velocityX, float velocityY) {
  sf::Vector2f position = circle.getPosition();
  TRACE_DEBUG("[SERVER: GAME_LOOP] Old x: " << position.x << ", y: " << position.y);

  if ((position.x + velocityX) < 0) {
    position.x = 0;
  } else if ((position.x + velocityX) > 450) {
    position.x = 450;
  } else {
    position.x += velocityX;
  }

  if ((position.y + velocityY) < 0) {
    position.y = 0;
  }
  else if ((position.y + velocityY) > 450) {
    position.y = 450;
  }
  else {
    position.y += velocityY;
  }

  circle.setPosition(position.x, position.y);
  sf::Vector2f newPos = circle.getPosition();
  TRACE_DEBUG("[SERVER: GAME_LOOP] New x: " << newPos.x << ", y: " << newPos.y);

  sf::Packet packet;
  //packet << 33 << newPos.x << newPos.y;
  SpriteMessage sm;
  sm.addSpriteData(33, newPos);
  packet = sm.pack();
  MessageHandler::get().pushSpriteListMessage(packet);
  TRACE_DEBUG("[SERVER: GAME_LOOP] Send packet for moving circle to the left");
  TRACE_DEBUG("[SERVER: GAME_LOOP] x: " << newPos.x << ", y: " << newPos.y);
  packet.clear();
}
