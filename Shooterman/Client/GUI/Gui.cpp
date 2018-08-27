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
  MessageHandler::get().subscribeToGameStateMessages(&mSystemMessageSubscriber);

  mWindow = new sf::RenderWindow(sf::VideoMode(500, 500), "Shooterman");
  mWindowOpen = true;

  mSpriteManager = new SpriteManager();
  mSpriteManager->loadSprites();

  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mLeftButtonAlreadyPressed = false;

  render();

  delete mWindow;
  delete mSpriteManager;
}

void Gui::render() {
  while (mWindow != nullptr && mWindow->isOpen()) {
    handleWindowEvents(mWindow);

    renderGameState(mWindow, mCurrentGameState);
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));

    handleSystemMessages();
  }
  mWindowOpen = false;
}

void Gui::handleWindowEvents(sf::RenderWindow* window) {
  sf::Event event;
  
  while (window->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      sf::Packet shutdownMessage;
      shutdownMessage << SHUT_DOWN;
      MessageHandler::get().pushSystemMessage(shutdownMessage);
    }
  }
}

void Gui::renderGameState(sf::RenderWindow* window, GAME_STATE gameState) {
  mRenderNeeded = false;

  switch (gameState) {
  case GAME_STATE::MAIN_MENU:
    mainMenu();
    break;
  case GAME_STATE::LOBBY:
    lobbyMenu();
    break;
  case GAME_STATE::PLAYING:
    playing();
    break;
  default:
    TRACE_ERROR("GAME_STATE: " << gameState << " is not implemented");
    break;
  }

  if (mRenderNeeded) {
    window->display();
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
    case CHANGE_GAME_STATE: {
      GameStateMessage gsm;
      gsm.unpack(systemMessage);
      mCurrentGameState = gsm.getGameState();
      break; }
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
  delete mGuiThread;
  TRACE_INFO("Shutdown of module done");
}

void Gui::mainMenu() {
  mWindow->clear(sf::Color::White);
  BUTTON_PRESSED buttonPressed = mMainMenu.checkMouse(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)));
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (!mLeftButtonAlreadyPressed) {
      switch (buttonPressed) {
      case BUTTON_PRESSED::B_LOBBY: {
        // Start the game loop
        GameStateMessage gsm(GAME_STATE::LOBBY);
        MessageHandler::get().pushGameStateMessage(gsm.pack());
        break; }
      case BUTTON_PRESSED::QUIT: {
        sf::Packet shutdownMessage;
        shutdownMessage << SHUT_DOWN;
        MessageHandler::get().pushSystemMessage(shutdownMessage);
        break; }
      case BUTTON_PRESSED::NO_ACTION:
      default:
        // Do nothing;
        break;
      }
    }
    mLeftButtonAlreadyPressed = true;
  } else {
    mLeftButtonAlreadyPressed = false;
  }
  mMainMenu.render(mWindow);
  mRenderNeeded = true;
}

void Gui::lobbyMenu() {
  mWindow->clear(sf::Color::White);
  BUTTON_PRESSED buttonPressed = mLobbyMenu.checkMouse(mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow)));
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
    if (!mLeftButtonAlreadyPressed) {
      switch (buttonPressed) {
      case BUTTON_PRESSED::START: {
        GameStateMessage gsm(GAME_STATE::PLAYING);
        MessageHandler::get().pushGameStateMessage(gsm.pack());
        break; }
      case BUTTON_PRESSED::BACK: {
        GameStateMessage gsm(GAME_STATE::MAIN_MENU);
        MessageHandler::get().pushGameStateMessage(gsm.pack());
        break; }
      case BUTTON_PRESSED::NO_ACTION:
      default:
        // Do nothing;
        break;
      }
    }
    mLeftButtonAlreadyPressed = true;
  } else {
    mLeftButtonAlreadyPressed = false;
  }
  mLobbyMenu.render(mWindow);
  mRenderNeeded = true;
}

void Gui::playing() {
  std::queue<sf::Packet> spriteMessageQueue = mSpriteListSubscriber.getMessageQueue();
  sf::Packet spriteMessage;
  if (!spriteMessageQueue.empty()) {
    mWindow->clear(sf::Color::White);
  }
  while (!spriteMessageQueue.empty()) {
    spriteMessage = spriteMessageQueue.front();
    int messageID;
    spriteMessage >> messageID;
    if (messageID == SPRITE_LIST) {
      spriteMessageQueue.pop();
      SpriteMessage sm;
      sm.unpack(spriteMessage);
      std::pair<int, sf::Vector2f> spriteData = sm.getSpriteData();
      while (spriteData.first != -1) {
        //TRACE_DEBUG(spriteData.first);
        sf::Sprite sprite = mSpriteManager->get(spriteData.first);
        sprite.setPosition(spriteData.second);
        mWindow->draw(sprite);
        spriteData = sm.getSpriteData();
        mRenderNeeded = true;
      }
    }
  }
}
