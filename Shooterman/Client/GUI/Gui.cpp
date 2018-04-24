#include "Gui.h"
#include <iostream>

Gui::Gui() {
  std::cout << "[GUI] Starting module..." << std::endl;
  mGuiThread = new std::thread(&Gui::render, this);
  std::cout << "[GUI] Starting module done" << std::endl;
}

void Gui::render()  {
  MessageHandler::get().subscribeToSpriteListMessages(&mSpriteListSubscriber);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);

  mWindow = new sf::RenderWindow(sf::VideoMode(500, 500), "SFML works!");

  sf::CircleShape shape(50.f);
  shape.setPosition(200, 200);
  shape.setFillColor(sf::Color::Green);

  while (mWindow->isOpen()) {

    sf::Event event;
    while (mWindow->pollEvent(event))
    {
      if (event.type == sf::Event::Closed) {
        sf::Packet shutdownMessage;
        shutdownMessage << SHUT_DOWN;
        MessageHandler::get().pushSystemMessage(shutdownMessage);
      }
    }
    std::queue<sf::Packet> spriteMessageQueue = mSpriteListSubscriber.getMessageQueue();
    sf::Packet spriteMessage;
    while (!spriteMessageQueue.empty()) {
      spriteMessage = spriteMessageQueue.front();
      spriteMessageQueue.pop();
      int id;
      sf::Vector2f pos;
      spriteMessage >> id >> pos.x >> pos.y;
      std::cout << "ID: " << id << " Pos: " << pos.x << ":" << pos.y << std::endl;
      shape.setPosition(pos);
    }

    mWindow->clear();
    mWindow->draw(shape);
    mWindow->display();
    sf::sleep(sf::milliseconds(20));
    handleSystemMessages();
  }
}

void Gui::handleSystemMessages() {
  std::queue<sf::Packet> systemMessageQueue = mSystemMessageSubscriber.getMessageQueue();
  sf::Packet systemMessage;
  while (!systemMessageQueue.empty()) {
    systemMessage = systemMessageQueue.front();
    systemMessageQueue.pop();

    auto messageId = 0;
    systemMessage >> messageId;
    if (messageId == SHUT_DOWN) {
      std::cout << "[GUI] Preparing to shut down" << std::endl;
      mWindow->close();
    } else {
      std::cout << "[GUI] Unknown system message: " << messageId << std::endl;
    }
  }
}

void Gui::shutDown() {
  std::cout << "[GUI] Shutdown of module requested..." << std::endl;
  while (mWindow->isOpen()) {
    // Wait for GUI to close window
  }
  delete mWindow;
  mGuiThread->join();
  delete mGuiThread;
  std::cout << "[GUI] Shutdown of module done" << std::endl;
}