#ifndef CLIENT_MAIN_H
#define CLIENT_MAIN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "Client/Input/Input.cpp"
#include "Client/MessageHandler/MessageHandler.cpp"
#include "Client/GUI/Gui.cpp"
#include "Server/Main/GameLoop.cpp"

class ClientMain
{
public:
	ClientMain();

private:
  bool mRunning = true;
  Subscriber mSystemMessageSubscriber;

  void handleSystemMessages();
};

#endif // ifndef CLIENT_MAIN_H