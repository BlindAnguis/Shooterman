#include "Gui.h"
#include <SFML/Network.hpp>
#include <iostream>

Gui::Gui(MessageHandler* messageHandler) :
  m_messageHandler(messageHandler) {
  std::cout << "Starting GUI module..." << std::endl;
  m_guiThread = new std::thread(&Gui::render, this);
  std::cout << "Starting GUI module done" << std::endl;
}

void Gui::render()  {

  m_window = new sf::RenderWindow(sf::VideoMode(500, 500), "SFML works!");
  m_window->setFramerateLimit(60);

  while (m_window->isOpen()) {
    sf::Packet p = m_messageHandler->readGraphicsMessage();

    sf::CircleShape shape(50.f);
    shape.setPosition(200, 200);
    shape.setFillColor(sf::Color::Green);


    sf::Event event;
    while (m_window->pollEvent(event))
    {
      if (event.type == sf::Event::Closed)
        m_window->close();
    }

    m_window->clear();
    m_window->draw(shape);
    m_window->display();
  }
}

void Gui::shutDown() {
  std::cout << "Shutdown of GUI module requested..." << std::endl;
  m_running = false;
  m_guiThread->join();
  std::cout << "Shutdown of GUI module done" << std::endl;
}