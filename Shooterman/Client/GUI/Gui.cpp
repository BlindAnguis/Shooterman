#include "Gui.h"

Gui::Gui() {
  mName = "GUI";
  TRACE_INFO("Starting module...");
  mGuiThread = std::make_unique<std::thread>(&Gui::init, this);
  TRACE_INFO("Starting module done");
}

void Gui::init() {
  MessageHandler::get().subscribeToSpriteListMessages(&mSpriteListSubscriber);
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mGameStateMessageSubscriber);

  mMenuMap.emplace(GAME_STATE::MAIN_MENU, new MainMenu());
  mMenuMap.emplace(GAME_STATE::LOBBY, new LobbyMenu());
  mMenuMap.emplace(GAME_STATE::JOIN, new JoinMenu());

  mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(500, 500), "Shooterman");
  mWindowOpen = true;

  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();

  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mLeftButtonAlreadyPressed = false;

  render();

  delete mSpriteManager;

  MessageHandler::get().unsubscribeAll(&mSpriteListSubscriber);
  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
}

void Gui::render() {
  while (mWindow != nullptr && mWindow->isOpen()) {
    handleWindowEvents();

    renderGameState(mCurrentGameState);
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));

    handleGameStateMessages();
    handleSystemMessages();
  }
  mWindowOpen = false;
}

void Gui::handleWindowEvents() {
  sf::Event event;
  
  while (mWindow->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      sf::Packet shutdownMessage;
      shutdownMessage << SHUT_DOWN;
      MessageHandler::get().pushSystemMessage(shutdownMessage);
    }
    if (event.type == sf::Event::MouseMoved) {
      MouseMessage mm(sf::Mouse::getPosition(*mWindow));
      MessageHandler::get().pushMouseMessage(mm.pack());
    }
    if (event.type == sf::Event::Resized) {
      sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
      mWindow->setView(sf::View(visibleArea));
    }
  }
}

void Gui::renderGameState(GAME_STATE gameState) {
  mRenderNeeded = false;

  switch (gameState) {
  case GAME_STATE::MAIN_MENU:
  case GAME_STATE::JOIN:
  case GAME_STATE::LOBBY: {
    mWindow->clear(sf::Color::White);
    sf::Vector2f mousePosition = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
      if (!mLeftButtonAlreadyPressed) {
        mMenuMap.at(gameState)->checkMouse(mousePosition);
      }
      mLeftButtonAlreadyPressed = true;
    } else {
      mLeftButtonAlreadyPressed = false;
    }
    mMenuMap.at(gameState)->render(mWindow, mousePosition);
    mRenderNeeded = true;
    break;
  }
  case GAME_STATE::PLAYING:
    playing();
    break;
  default:
    TRACE_ERROR("GAME_STATE: " << gameState << " is not implemented");
    break;
  }

  if (mRenderNeeded) {
    mWindow->display();
  }
}

void Gui::handleGameStateMessages() {
  std::queue<sf::Packet> gameStateMessageQueue = mGameStateMessageSubscriber.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    gameStateMessageQueue.pop();

    //auto messageId = 0;
    //gameStateMessage >> messageId;
    GameStateMessage gsm;
    gsm.unpack(gameStateMessage);
    mCurrentGameState = gsm.getGameState();
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
    switch (messageId) {
    case SHUT_DOWN:
      TRACE_INFO("Closing GUI window");
      mWindow->close();
      break;
    default:
      TRACE_WARNING("Unknown system message : " << messageId);
    }
  }
}

void Gui::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  while (mWindowOpen) {
    // Wait for GUI to close window
  }
  mGuiThread->join();
  TRACE_INFO("Shutdown of module done");
}

void Gui::playing() {
  std::queue<sf::Packet> spriteMessageQueue = mSpriteListSubscriber.getMessageQueue();
  sf::Packet spriteMessage;
  if (!spriteMessageQueue.empty()) {
    mWindow->clear(sf::Color::White);
    mRenderNeeded = true;
  }

  while (!spriteMessageQueue.empty()) {
    spriteMessage = spriteMessageQueue.front();
    int messageID;
    spriteMessage >> messageID;
    if (messageID == SPRITE_LIST) {
      spriteMessageQueue.pop();
      SpriteMessage sm;
      sm.unpack(spriteMessage);
      int position = sm.getSize() - 1;
      std::pair<int, sf::Vector2f> spriteData = sm.getSpriteData(position);
      TRACE_DEBUG("SpriteID: " << spriteData.first);
      while (spriteData.first != -1) {
        TRACE_DEBUG(spriteData.first);
        sf::Sprite sprite = mSpriteManager->get(spriteData.first);
        sprite.setPosition(spriteData.second);
        mWindow->draw(sprite);
        position--;
        spriteData = sm.getSpriteData(position);
      }
    }
    else {

      TRACE_DEBUG("Found no message");
    }
  }
}
