#ifndef INPUT_H
#define INPUT_H

#include <thread>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "Common/KeyBindings.h"
#include "Common/MessageId.h"
#include "../MessageHandler/MessageHandler.h"


class Input {
public:
	Input();

  void shutDown();

private:
  bool mRunning = true;
  std::thread* mInputThread;
  Subscriber mSystemMessageSubscriber;

  void readInput();
  void handleSystemMessages();

};

#endif // INPUT_H