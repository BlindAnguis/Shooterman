#include "Gui.h"

Gui::Gui() {
  mName = "GUI";
  TRACE_INFO("Starting module...");
  mGuiThread = new std::thread(&Gui::init, this);
  TRACE_INFO("Starting module done");
}

void Gui::init() {
  MessageHandler::get().subscribeToSpriteListMessages(&mSpriteListSubscriber);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);

  mWindow = new sf::RenderWindow(sf::VideoMode(500, 500), "Shooterman");

  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();

  render();

  delete mSpriteManager;
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
      //TRACE_DEBUG(spriteMessageQueue.size());
      render = true;
    }
    while (!spriteMessageQueue.empty()) {
      spriteMessage = spriteMessageQueue.front();
      spriteMessageQueue.pop();
      int id;
      sf::Vector2f position;
      spriteMessage >> id >> position.x >> position.y;
      //TRACE_DEBUG("ID: " << id << " Pos: " << position.x << ":" << position.y);
      sf::Sprite sprite = mSpriteManager->get(id);
      sprite.setPosition(position);
      mWindow->draw(sprite);
    }

    if (render) {
      mWindow->display();
    }
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
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
      TRACE_INFO("Closing GUI window");
      mWindow->close();
    } else {
      TRACE_WARNING(STR("Unknown system message : " << messageId));
    }
  }
}

void Gui::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  while (mWindow->isOpen()) {
    // Wait for GUI to close window
  }
  delete mWindow;
  mGuiThread->join();
  delete mGuiThread;
  TRACE_INFO("Shutdown of module done");
}