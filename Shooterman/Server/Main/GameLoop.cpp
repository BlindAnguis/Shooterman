#include <SFML\System.hpp>
#include <SFML\Graphics.hpp>
#include "GameLoop.h"
#include "../../Common/KeyBindings.h"

GameLoop::GameLoop() {
  mName = "GAMELOOP";
}

void GameLoop::start() {
  TRACE_INFO("Start Server");
  mGameLoopThread = new std::thread(&GameLoop::gameLoop, this);
}

void GameLoop::stop() {
  TRACE_INFO("Exit Server");
  mRunning = false;
  mGameLoopThread->join();
  delete mGameLoopThread;
  // mInputSubscriber.unsubscribe;
  TRACE_INFO("Server finished");
}

void GameLoop::gameLoop() {
  GameLoopState state = GameLoopState::LOBBY;
  mRunning = true;
  TRACE_INFO("Gameloop started");
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
      TRACE_DEBUG(STR("Message id: " << messageId));
      
      inputMessage >> input;
      TRACE_DEBUG(STR("input: " << input));
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
      TRACE_ERROR("This state doesn't exist");
    }
    input = 0;
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
  }  
}

void GameLoop::moveCircle(sf::CircleShape &circle, float velocityX, float velocityY) {
  sf::Vector2f position = circle.getPosition();
  TRACE_DEBUG(STR("Old x: " << position.x << ", y: " << position.y));

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
  TRACE_DEBUG(STR("New x: " << newPos.x << ", y: " << newPos.y));

  sf::Packet packet;
  packet << 33 << newPos.x << newPos.y;

  MessageHandler::get().pushSpriteListMessage(packet);

  TRACE_DEBUG("Send packet for moving circle to the left");
  TRACE_DEBUG(STR("x: " << newPos.x << ", y: " << newPos.y));
  packet.clear();
}
