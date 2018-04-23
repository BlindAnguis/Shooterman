#include "ClientMain.h"

ClientMain::ClientMain() {
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Input input = Input();
  Gui gui = Gui();
  GameLoop server = GameLoop();
  bool serverStarted = false;

  while (mRunning)
  {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
    {
      // Start server
      if (!serverStarted) {
        server.start();
        serverStarted = true;
      }
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
      // Stop Server
      if (serverStarted) {
        server.stop();
        serverStarted = false;
      }
    }
    handleSystemMessages();
  }
  input.shutDown();
  gui.shutDown();
}

void ClientMain::handleSystemMessages() {
  std::queue<sf::Packet> systemMessageQueue = mSystemMessageSubscriber.getMessageQueue();
  sf::Packet systemMessage;
  while (!systemMessageQueue.empty()) {
    systemMessage = systemMessageQueue.front();
    systemMessageQueue.pop();

    auto messageId = 0;
    systemMessage >> messageId;
    if (messageId == SHUT_DOWN) {
      std::cout << "[ClientMain] Preparing to shut down" << std::endl;
      mRunning = false;
    }
    else {
      std::cout << "[ClientMain] Unknown system message: " << messageId << std::endl;
    }
  }
}