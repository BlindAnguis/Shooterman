#include <SFML\System.hpp>
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
      std::cout << "In the lobby" << std::endl;

      if (input == A_KEY) {
        state = GameLoopState::SETUP_GAME;
      }
      break;

    case GameLoopState::SETUP_GAME:
      //std::cout << "Setting up the game" << std::endl;
      state = GameLoopState::PLAYING;
      //std::cout << "Game setup finished" << std::endl;
      break;

    case GameLoopState::PLAYING:
      std::cout << "Playing the game" << std::endl;
      if (input == D_KEY) {
        state = GameLoopState::GAME_OVER;
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
    sf::sleep(sf::milliseconds(20));
  }  
}
