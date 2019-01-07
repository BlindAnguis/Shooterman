#include "Gui.h"

Gui::Gui() {
  mName = "CLIENT: GUI";
  TRACE_INFO("Starting module...");
  mGuiThread = std::make_unique<std::thread>(&Gui::init, this);
  TRACE_INFO("Starting module done");
}

void Gui::init() {
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mGameStateMessageSubscriber);

  mMenuMap.emplace(GAME_STATE::MAIN_MENU, new MainMenu());
  mMenuMap.emplace(GAME_STATE::LOBBY, new LobbyMenu());
  mMenuMap.emplace(GAME_STATE::JOIN, new JoinMenu());
  mMenuMap.emplace(GAME_STATE::PLAYING, new PlayWindow());
  mMenuMap.emplace(GAME_STATE::OPTIONS, new OptionsMenu());

  mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024, 1024), "Shooterman");
  mWindowOpen = true;

  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mLeftButtonAlreadyPressed = false;

  render();

  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
  MessageHandler::get().unsubscribeAll(&mGameStateMessageSubscriber);
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
    if (event.type == sf::Event::TextEntered && mWindow->hasFocus()) {
      auto it = mMenuMap.find(mCurrentGameState);
      if (it != mMenuMap.end()) {
        it->second->handleNewText(event.text.unicode);
      }
    }
  }
}

void Gui::renderGameState(GAME_STATE gameState) {
  mRenderNeeded = false;

  auto it = mMenuMap.find(gameState);

  if (it == mMenuMap.end()) {
    TRACE_ERROR("GAME_STATE: " << gameState << " is not implemented");
    return;
  }

  mWindow->clear(sf::Color::White);
  sf::Vector2f mousePosition = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mWindow->hasFocus()) {
    if (!mLeftButtonAlreadyPressed) {
      mMenuMap.at(gameState)->checkMouse(mousePosition);
    }
    mLeftButtonAlreadyPressed = true;
  }
  else {
    mLeftButtonAlreadyPressed = false;
  }
  mRenderNeeded = mMenuMap.at(gameState)->render(mWindow, mousePosition);

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

    GameStateMessage gsm;
    gsm.unpack(gameStateMessage);
    if (gsm.getGameState() != mCurrentGameState) {
      // Changed game state
      auto previousMenu = mMenuMap.find(mCurrentGameState);
      if (previousMenu != mMenuMap.end()) {
        previousMenu->second->uninit();
      }

      mCurrentGameState = gsm.getGameState();
      auto newMenu = mMenuMap.find(mCurrentGameState);
      if (newMenu != mMenuMap.end()) {
        newMenu->second->init();
      }
    }
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