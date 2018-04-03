#ifndef INPUT_H
#define INPUT_H

#include <thread>
#include <SFML/System.hpp>
#include <SFML/Network.hpp>

#include "../MessageHandler/MessageHandler.h"

#define MOUSE1_KEY  1
#define MOUSE2_KEY  2
#define W_KEY       4
#define A_KEY       8
#define S_KEY       16
#define D_KEY       32

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