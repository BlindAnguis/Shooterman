#include "Gui.h"

#include <iostream>

Gui::Gui(MessageHandler* messageHandler) :
  m_messageHandler(messageHandler) {
  std::cout << "Starting GUI module..." << std::endl;
  m_guiThread = new std::thread(&Gui::render, this);
  std::cout << "Starting GUI module done" << std::endl;
}

void Gui::render() {

}

void Gui::shutDown() {
  std::cout << "Shutdown of GUI module requested..." << std::endl;
  m_running = false;
  m_guiThread->join();
  std::cout << "Shutdown of GUI module done" << std::endl;
}