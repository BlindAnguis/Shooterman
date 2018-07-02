#include "ClientMain.h"

ClientMain::ClientMain() {
  mName = "CLIENTMAIN";
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Input input = Input();
  Gui gui = Gui();
  GameLoop server = GameLoop();
  //Sound sound = Sound();
  
  bool serverStarted = false;

  while (mRunning) {
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return)) {
	    // Start server
	    if (!serverStarted) {
	      server.start();
		  serverStarted = true;
	    }
	  }
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::F10)) {
	    // Stop Server
	    if(serverStarted) {
          server.stop();
          serverStarted = false;
      }
    }
	  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
      TRACE_DEBUG("Escape pressed");
	  }
    sf::sleep(sf::milliseconds(20));
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
      TRACE_INFO("Preparing to shut down");
      mRunning = false;
    } else {
      TRACE_WARNING("Unknown system message: " << messageId);
    }
  }
}