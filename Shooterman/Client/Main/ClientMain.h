#ifndef CLIENT_MAIN_H
#define CLIENT_MAIN_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <stdlib.h>
#include <time.h>

#include "../Input/Input.h"
#include "../MessageHandler/MessageHandler.h"
#include "../GUI/Gui.h"
#include "../../Server/Main/GameLoop.h"
#include "../Sound/Sound.h"

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