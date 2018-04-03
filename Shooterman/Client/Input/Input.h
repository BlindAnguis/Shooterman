#ifndef INPUT_H
#define INPUT_H

#include <thread>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "../../Common/KeyBindings.h"
#include "../MessageHandler/MessageHandler.h"


class Input {
public:
	Input(MessageHandler* messageHandler);

  void shutDown();

private:
  bool m_running = true;
  std::thread* m_inputThread;
  MessageHandler* m_messageHandler;

  void readInput();

};

#endif // INPUT_H