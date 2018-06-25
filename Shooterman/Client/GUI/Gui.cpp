#include "Gui.h"
#include <iostream>

Gui::Gui() {
  std::cout << "[GUI] Starting module..." << std::endl;
  mGuiThread = new std::thread(&Gui::init, this);
  std::cout << "[GUI] Starting module done" << std::endl;
}

void Gui::init() {
  MessageHandler::get().subscribeToSpriteListMessages(&mSpriteListSubscriber);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);

  mWindow = new sf::RenderWindow(sf::VideoMode(500, 500), "SFML works!");

  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();

  render();
}

void Gui::render() {
  while (mWindow->isOpen()) {

    sf::Event event;
    while (mWindow->pollEvent(event)) {
      if (event.type == sf::Event::Closed) {
        sf::Packet shutdownMessage;
        shutdownMessage << SHUT_DOWN;
        MessageHandler::get().pushSystemMessage(shutdownMessage);
      }
    }
    bool render = false;
    std::queue<sf::Packet> spriteMessageQueue = mSpriteListSubscriber.getMessageQueue();
    sf::Packet spriteMessage;
    if (!spriteMessageQueue.empty()) {
      mWindow->clear(sf::Color::White);
      std::cout << spriteMessageQueue.size() << std::endl;
      render = true;
    }
    while (!spriteMessageQueue.empty()) {
      spriteMessage = spriteMessageQueue.front();
      spriteMessageQueue.pop();
      int id;
      sf::Vector2f position;
      spriteMessage >> id >> position.x >> position.y;
      //std::cout << "ID: " << id << " Pos: " << position.x << ":" << position.y << std::endl;
      sf::Sprite sprite = mSpriteManager->get(id);
      sprite.setPosition(position);
      mWindow->draw(sprite);
    }

    if (render) {
      mWindow->display();
    }
    sf::sleep(sf::milliseconds(10));
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
  delete mSpriteManager;
  delete mWindow;
  mGuiThread->join();
  delete mGuiThread;
  std::cout << "[GUI] Shutdown of module done" << std::endl;
}