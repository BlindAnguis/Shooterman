#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "GameLoop.h"
#include <iostream>
#include "../../Common/KeyBindings.h"
#include "../EntityManager/EntityManager.h"
#include "../Engine/Engine.h"
#include "../Systems/InputSystem/InputSystem.h"

GameLoop::GameLoop() {
}

void GameLoop::start() {
  std::cout << "[SERVER: GAME_LOOP] Start Server" << std::endl;
  mGameLoopThread = new std::thread(&GameLoop::gameLoop, this);
}

void GameLoop::stop() {
  std::cout << "[SERVER: GAME_LOOP] Exit server" << std::endl;
  mRunning = false;
  mGameLoopThread->join();
  delete mGameLoopThread;
  // mInputSubscriber.unsubscribe;
  std::cout << "[SERVER: GAME_LOOP] Server finished" << std::endl;
}

void GameLoop::gameLoop() {
  GameLoopState state = GameLoopState::LOBBY;
  mRunning = true;
  std::cout << "[SERVER: GAME_LOOP] Gameloop started" << std::endl;
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
        std::cout << "[SERVER: GAME_LOOP] Setting GameLoopState to SETUP_GAME" << std::endl;
        state = GameLoopState::SETUP_GAME;
      }
      break;

    case GameLoopState::SETUP_GAME:
      std::cout << "[SERVER: GAME_LOOP] Setting GameLoopState to PLAYING" << std::endl;
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
      std::cout << "[SERVER: GAME_LOOP] This state doesn't exist" << std::endl;
    }
    sf::sleep(sf::milliseconds(20));
  }  
}

void GameLoop::moveCircle(sf::CircleShape &circle, float velocityX, float velocityY) {
  sf::Vector2f position = circle.getPosition();
  std::cout << "[SERVER: GAME_LOOP] Old x: " << position.x << ", y: " << position.y << std::endl;

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
  std::cout << "[SERVER: GAME_LOOP] New x: " << newPos.x << ", y: " << newPos.y << std::endl;

  sf::Packet packet;
  packet << 33 << newPos.x << newPos.y;

  MessageHandler::get().pushSpriteListMessage(packet);
  packet.clear();
}
