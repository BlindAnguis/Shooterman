#include "ClientMain.h"
#include "Client/Sound/Sound.cpp"

ClientMain::ClientMain() {
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Input input = Input();
  Gui gui = Gui();
  GameLoop server = GameLoop();
  //Sound sound = Sound();
  
  bool serverStarted = false;

  while (mRunning)
  {
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Return))
	  {
	  // Start server
	  if (!serverStarted)
	  {
	    server.start();
		serverStarted = true;
	  }
	}
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::F10)) 
	{
	  // Stop Server
	  if(serverStarted) 
	  {
        server.stop();
        serverStarted = false;
      }
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
      std::cout << "[ClientMain] Preparing to shut down" << std::endl;
      mRunning = false;
    }
    else {
      std::cout << "[ClientMain] Unknown system message: " << messageId << std::endl;
    }
  }
}