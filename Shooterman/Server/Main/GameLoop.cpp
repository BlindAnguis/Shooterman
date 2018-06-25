#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "Server/Main/GameLoop.h"
#include <iostream>
#include "Common/KeyBindings.h"

GameLoop::GameLoop() {
}

void GameLoop::start() {
  std::cout << "Start Server" << std::endl;
  mGameLoopThread = new std::thread(&GameLoop::gameLoop, this);
}

void GameLoop::stop() {
  std::cout << "Exit server" << std::endl;
  mRunning = false;
  mGameLoopThread->join();
  delete mGameLoopThread;
  // mInputSubscriber.unsubscribe;
  std::cout << "Server finished" << std::endl;
}

void GameLoop::gameLoop() {
  GameLoopState state = GameLoopState::LOBBY;
  mRunning = true;
  std::cout << "Gameloop started" << std::endl;
  MessageHandler::get().subscribeToInputMessages(&mInputSubscriber);
  int input = 0;
  int messageId;
  sf::Packet movePacket;
  sf::CircleShape shape(50.f);
  shape.setPosition(200, 200);
  float velocityX = 5;
  float velocityY = 5;

  while (mRunning) {
    //sf::Packet inputMessage = mMessageHandler->readInputMessage();
    std::queue<sf::Packet> inputMessagesQueue = mInputSubscriber.getMessageQueue();
    sf::Packet inputMessage;
    if (!inputMessagesQueue.empty()) {
      inputMessage = inputMessagesQueue.front();
      inputMessagesQueue.pop();

      inputMessage >> messageId;
      std::cout << "Message id: " << messageId << std::endl;
      
      inputMessage >> input;
      std::cout << "input: " << input << std::endl;
    }
    
    switch (state) {
    case GameLoopState::LOBBY:
      //std::cout << "In the lobby" << std::endl;

      if (input == A_KEY) {
        state = GameLoopState::SETUP_GAME;
      }
      break;

    case GameLoopState::SETUP_GAME:
      //std::cout << "Setting up the game" << std::endl;
      shape.setFillColor(sf::Color::Green);
      movePacket << 33 << shape.getPosition().x << shape.getPosition().y;
      MessageHandler::get().pushSpriteListMessage(movePacket);
      movePacket.clear();
      state = GameLoopState::PLAYING;
      //std::cout << "Game setup finished" << std::endl;
      break;

    case GameLoopState::PLAYING:
      //std::cout << "Playing the game" << std::endl;
      if (input == D_KEY) {
        moveCircle(shape, velocityX, 0);
      }

      if (input == A_KEY) {
        moveCircle(shape, -velocityX, 0);
      }

      if (input == W_KEY) {
        moveCircle(shape, 0, -velocityY);
      }

      if (input == S_KEY) {
        moveCircle(shape, 0, velocityY);
      }

	  if (input == A_S_KEY) {
		  moveCircle(shape, -velocityX, velocityY);
	  }

	  if (input == A_W_KEY) {
		  moveCircle(shape, -velocityX, -velocityY);
	  }

	  if (input == D_S_KEY) {
		  moveCircle(shape, velocityX, velocityY);
	  }

	  if (input == D_W_KEY) {
		  moveCircle(shape, velocityX, -velocityY);
	  }

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
      std::cout << "This state doesn't exist" << std::endl;
    }
    input = 0;
    sf::sleep(sf::milliseconds(20));
  }  
}

void GameLoop::moveCircle(sf::CircleShape &circle, float velocityX, float velocityY) {
  sf::Vector2f position = circle.getPosition();
  std::cout << "Old x: " << position.x << ", y: " << position.y << std::endl;

  if ((position.x + velocityX) < 0) {
    position.x = 0;
  } else if ((position.x + velocityX) > 400) {
    position.x = 400;
  } else {
    position.x += velocityX;
  }

  if ((position.y + velocityY) < 0) {
    position.y = 0;
  }
  else if ((position.y + velocityY) > 400) {
    position.y = 400;
  }
  else {
    position.y += velocityY;
  }

  circle.setPosition(position.x, position.y);
  sf::Vector2f newPos = circle.getPosition();
  std::cout << "New x: " << newPos.x << ", y: " << newPos.y << std::endl;

  sf::Packet packet;
  packet << 33 << newPos.x << newPos.y;

  MessageHandler::get().pushSpriteListMessage(packet);

  std::cout << "Send packet for moving circle to the left" << std::endl;
  std::cout << "x: " << newPos.x << ", y: " << newPos.y << std::endl;
  packet.clear();
}
