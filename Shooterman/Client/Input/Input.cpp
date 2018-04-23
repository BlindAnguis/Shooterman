#include "Input.h"

#include <cstdint>

#include <iostream>

Input::Input()
{
  std::cout << "Starting Input module..." << std::endl;
  m_inputThread = new std::thread(&Input::readInput, this);
  std::cout << "Starting Input module done" << std::endl;
}

void Input::readInput() {
  std::cout << "Reading input..." << std::endl;
  std::uint32_t keyboardBitmask;
  while (m_running) {
    keyboardBitmask = 0;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
      keyboardBitmask += A_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
      keyboardBitmask += D_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
      keyboardBitmask += W_KEY;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
      keyboardBitmask += S_KEY;
    }

    if (keyboardBitmask != 0) {
      sf::Packet inputKeyPacket;
      inputKeyPacket << 0 << keyboardBitmask;
      MessageHandler::get().pushInputMessage(inputKeyPacket);
    }
    sf::sleep(sf::milliseconds(10));
  }
}

void Input::shutDown() {
  std::cout << "Shutdown of Input module requested..." << std::endl;
  m_running = false;
  m_inputThread->join();
  std::cout << "Shutdown of Input module done" << std::endl;
}