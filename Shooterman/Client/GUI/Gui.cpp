#include "Gui.h"

#include "Hud/Hud.h"
#include "Resources/FontManager.h"
#include "MainMenu/MainMenu.h"
#include "LobbyMenu/LobbyMenu.h"
#include "JoinMenu/JoinMenu.h"
#include "PlayWindow/PlayWindow.h"
#include "OptionsMenu/OptionsMenu.h" 
#include "DebugMenu/DebugMenu.h"
#include "PauseMenu/PauseMenu.h"
#include "../../Common/Constants.h"
#include "../../Common/MessageId.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/MouseMessage.h"

Gui::Gui() {
  mName = "CLIENT: GUI";
  TRACE_INFO("Starting module...");
  mGuiThread = std::make_unique<std::thread>(&Gui::init, this);
  TRACE_INFO("Starting module done");
}

void Gui::init() {
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  MessageHandler::get().subscribeTo("ClientGameState", &mGameStateMessageSubscriber);
  MessageHandler::get().publishInterface("MousePosition", &mMouseInterface);

  Interface pc;

  mMenuMap.emplace(GAME_STATE::MAIN_MENU, std::vector<MenuBase*> { new MainMenu() });
  mMenuMap.emplace(GAME_STATE::LOBBY, std::vector<MenuBase*> { new LobbyMenu(true) });
  mMenuMap.emplace(GAME_STATE::CLIENT_LOBBY, std::vector<MenuBase*> { new LobbyMenu(false) });
  mMenuMap.emplace(GAME_STATE::JOIN, std::vector<MenuBase*> { new JoinMenu() });
  mMenuMap.emplace(GAME_STATE::PLAYING, std::vector<MenuBase*> { new PlayWindow(), new Hud() });
  mMenuMap.emplace(GAME_STATE::OPTIONS, std::vector<MenuBase*> { new OptionsMenu() });
  mMenuMap.emplace(GAME_STATE::PAUSE, std::vector<MenuBase*> { new PauseMenu() });
  mMenuMap.emplace(GAME_STATE::DEBUG, std::vector<MenuBase*> { new DebugMenu() });

  // This needs to be after the DebugMenu is created
  while (!MessageHandler::get().subscribeTo("ClientDebugMenu", &mDebugSubscriber)) {
    sf::sleep(sf::milliseconds(5));
  }
  AddDebugButtonMessage debMess(mDebugSubscriber.getId(), "GUI debug traces", "Client");
  mDebugSubscriber.reverseSendMessage(debMess.pack());

  mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024, 1024), "Shooterman");
  mWindowOpen = true;

  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mLeftButtonAlreadyPressed = false;

  render();

  RemoveDebugButtonMessage rdbm(mDebugSubscriber.getId());
  sf::Packet packet = rdbm.pack();
  mDebugSubscriber.reverseSendMessage(packet);

  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
  MessageHandler::get().unsubscribeTo("ClientGameState", &mGameStateMessageSubscriber);
  MessageHandler::get().unsubscribeTo("ClientDebugMenu", &mDebugSubscriber);
  MessageHandler::get().unpublishInterface("MousePosition");

  FontManager::getInstance().clear();
}

void Gui::render() {
  while (mWindow != nullptr && mWindow->isOpen()) {
    mRenderClock.restart();
    handleWindowEvents();
    
    bool renderNeeded = false;
    renderNeeded = renderGameState(mCurrentGameState);

    if (mRenderNeeded) {
      mWindow->display();
    }

    handleGameStateMessages();
    handleSystemMessages();
    handleDebugMessages();

    auto currentFrameLength = mRenderClock.getElapsedTime().asMilliseconds();
    if (currentFrameLength < FRAME_LENGTH_IN_MS) {
      sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS - currentFrameLength));
    } else {
      // No need to sleep, frame took to long
    }
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
      mMouseInterface.pushMessage(mm.pack());
    }
    if (event.type == sf::Event::Resized) {
      sf::FloatRect visibleArea(0, 0, (float)event.size.width, (float)event.size.height);
      mWindow->setView(sf::View(visibleArea));
    }
    if (event.type == sf::Event::TextEntered && mWindow->hasFocus()) {
      auto it = mMenuMap.find(mCurrentGameState);
      if (it != mMenuMap.end()) {
        for (auto menu : it->second) {
          menu->handleNewText(event.text.unicode);
        }
      }
    }
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::F1)) {
    if (!mF1KeyAlreadyPressed) {
      if (mCurrentGameState == GAME_STATE::DEBUG) {
        mCurrentGameState = mPreviousGameState;
      } else {
        mPreviousGameState = mCurrentGameState;
        mCurrentGameState = GAME_STATE::DEBUG;
      }
      mF1KeyAlreadyPressed = true;
    }
  } else {
    mF1KeyAlreadyPressed = false;
  }
}

bool Gui::renderGameState(GAME_STATE gameState) {
  mRenderNeeded = false;

  auto it = mMenuMap.find(gameState);

  if (it == mMenuMap.end()) {
    TRACE_ERROR("GAME_STATE: " << gameState << " is not implemented");
    return mRenderNeeded;
  }

  mWindow->clear(sf::Color::White);
  sf::Vector2f mousePosition = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
  for (auto menu : it->second) {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && mWindow->hasFocus()) {
      if (!mLeftButtonAlreadyPressed) {
        menu->checkMouse(mousePosition);
      }
      mLeftButtonAlreadyPressed = true;
    } else {
      mLeftButtonAlreadyPressed = false;
    }
    mRenderNeeded = menu->render(mWindow, mousePosition);

    if (!mRenderNeeded) {
      return mRenderNeeded;
    }
  }
  return mRenderNeeded;
}

void Gui::handleGameStateMessages() {
  std::queue<sf::Packet> gameStateMessageQueue = mGameStateMessageSubscriber.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    int id = -1;
    gameStateMessage >> id;

    if (id == CHANGE_GAME_STATE) {
      gameStateMessageQueue.pop();

      GameStateMessage gsm;
      gsm.unpack(gameStateMessage);
      if (gsm.getGameState() != mCurrentGameState) {
        // Changed game state
        auto previousMenu = mMenuMap.find(mCurrentGameState);
        if (previousMenu != mMenuMap.end()) {
          for (auto menu : previousMenu->second) {
            menu->uninit();
          }
        }

        mPreviousGameState = mCurrentGameState;
        mCurrentGameState = gsm.getGameState();
        auto newMenu = mMenuMap.find(mCurrentGameState);
        if (newMenu != mMenuMap.end()) {
          for (auto menu : newMenu->second) {
            menu->init();
          }
        }

        mWindow->clear(sf::Color::White);
        mWindow->display();
      }
    } else {
      TRACE_WARNING("Received unexpected message with ID: " << id);
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

void Gui::handleDebugMessages() {
  std::queue<sf::Packet> debugMessageQueue = mDebugSubscriber.getMessageQueue();
  sf::Packet debugMessage;
  while (!debugMessageQueue.empty()) {
    debugMessage = debugMessageQueue.front();
    debugMessageQueue.pop();
    TRACE_DEBUG("Toggle debug traces");
    mDebugEnabled = !mDebugEnabled;
    TRACE_DEBUG("Toggle debug traces");
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