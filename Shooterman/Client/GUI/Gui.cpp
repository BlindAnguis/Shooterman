#include "Gui.h"

#include "Hud/Hud.h"
#include "Resources/GuiResourceManager.h"
#include "MainMenu/MainMenu.h"
#include "LobbyMenu/LobbyMenu.h"
#include "JoinMenu/JoinMenu.h"
#include "PlayWindow/PlayWindow.h"
#include "OptionsMenu/OptionsMenu.h" 
#include "DebugMenu/DebugMenu.h"
#include "PauseMenu/PauseMenu.h"
#include "MapEditor/MapEditor.h"
#include "../../Common/Constants.h"
#include "../../Common/MessageId.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/MouseMessage.h"

Gui::Gui() {
  mName = "CLIENT: GUI";
  TRACE_INFO("Starting module...");
  mGuiThread = std::make_unique<std::thread>(&Gui::run, this);
  TRACE_INFO("Starting module done");
}

void Gui::run() {
  TRACE_FUNC_ENTER();
  MessageHandler::get().subscribeTo("ClientSystemMessage", &mSystemMessageSubscriber);
  MessageHandler::get().subscribeTo("ClientGameState", &mGameStateMessageSubscriber);
  MessageHandler::get().publishInterface("MousePosition", &mMouseInterface);

  mMenuMap.emplace(GAME_STATE::MAIN_MENU, std::list<std::shared_ptr<MenuBase>> { std::make_shared<MainMenu>() });
  mMenuMap.emplace(GAME_STATE::LOBBY, std::list<std::shared_ptr<MenuBase>> { std::make_shared<LobbyMenu>(true) });
  mMenuMap.emplace(GAME_STATE::CLIENT_LOBBY, std::list<std::shared_ptr<MenuBase>> { std::make_shared<LobbyMenu>(false) });
  mMenuMap.emplace(GAME_STATE::JOIN, std::list<std::shared_ptr<MenuBase>> { std::make_shared<JoinMenu>() });
  mMenuMap.emplace(GAME_STATE::PLAYING, std::list<std::shared_ptr<MenuBase>> { std::make_shared<PlayWindow>(), std::make_shared<Hud>() });
  mMenuMap.emplace(GAME_STATE::OPTIONS, std::list<std::shared_ptr<MenuBase>> { std::make_shared<OptionsMenu>() });
  mMenuMap.emplace(GAME_STATE::PAUSE, std::list<std::shared_ptr<MenuBase>> { std::make_shared<PauseMenu>() });
  mMenuMap.emplace(GAME_STATE::DEBUG, std::list<std::shared_ptr<MenuBase>> { std::make_shared<DebugMenu>() });
  mMenuMap.emplace(GAME_STATE::MAP_EDITOR, std::list<std::shared_ptr<MenuBase>> { std::make_shared<MapEditor>() });

  // This needs to be after the DebugMenu is created
  while (!setupDebugMessages("Client", "Gui")) {
    sf::sleep(sf::milliseconds(5));
  }

  mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024, 1024), "Shooterman");
  mWindowOpen = true;

  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mLeftButtonAlreadyPressed = false;

  render();

  teardownDebugMessages();

  MessageHandler::get().unsubscribeTo("ClientSystemMessage", &mSystemMessageSubscriber);
  MessageHandler::get().unsubscribeTo("ClientGameState", &mGameStateMessageSubscriber);
  MessageHandler::get().unpublishInterface("MousePosition");

  GuiResourceManager::getInstance().clear();
  mMenuMap.clear();
  TRACE_FUNC_EXIT();
}

void Gui::render() {
  TRACE_FUNC_ENTER();
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
      TRACE_DEBUG3("Frame length: " << currentFrameLength << " ms");
    }
  }
  mWindowOpen = false;
  TRACE_FUNC_EXIT();
}

void Gui::handleWindowEvents() {
  TRACE_FUNC_ENTER();
  sf::Event event;
  
  while (mWindow->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      sf::Packet shutdownMessage;
      shutdownMessage << SHUT_DOWN;
      Subscriber s;
      MessageHandler::get().subscribeTo("ClientSystemMessage", &s);
      s.reverseSendMessage(shutdownMessage);
      MessageHandler::get().unsubscribeTo("ClientSystemMessage", &s);
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
  TRACE_FUNC_EXIT();
}

bool Gui::renderGameState(GAME_STATE gameState) {
  TRACE_FUNC_ENTER();
  mRenderNeeded = false;

  auto it = mMenuMap.find(gameState);

  if (it == mMenuMap.end()) {
    TRACE_ERROR("GAME_STATE: " << gameState << " is not implemented");
    return mRenderNeeded;
  }

  mWindow->clear(sf::Color::White);
  sf::Vector2f mousePosition = mWindow->mapPixelToCoords(sf::Mouse::getPosition(*mWindow));
  for (auto menu : it->second) {
    if (mWindow->hasFocus()) {
      if (!mLeftButtonAlreadyPressed) {
        mLeftButtonAlreadyPressed = menu->checkMouse(mousePosition);
      }
      if (!sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
        mLeftButtonAlreadyPressed = false;
      }
    }
    mRenderNeeded = menu->render(mWindow, mousePosition);
    mInfoOverlay.render(mWindow, mousePosition);
    if (!mRenderNeeded) {
      return mRenderNeeded;
    }
  }
  TRACE_FUNC_EXIT();
  return mRenderNeeded;
}

void Gui::handleGameStateMessages() {
  TRACE_FUNC_ENTER();
  std::queue<sf::Packet> gameStateMessageQueue = mGameStateMessageSubscriber.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    int id = -1;
    gameStateMessage >> id;

    if (id == CHANGE_GAME_STATE) {
      gameStateMessageQueue.pop();

      GameStateMessage gsm(gameStateMessage);
      TRACE_REC("New Game State: " << gsm.getGameStateAsString());
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

      if (mCurrentGameState == GAME_STATE::MAIN_MENU) {
        for (auto menuList : mMenuMap) {
          for (auto menu : menuList.second) {
            menu->reset();
          }
        }
      }

    } else {
      TRACE_WARNING("Received unexpected message with ID: " << id);
    }
  }
  TRACE_FUNC_EXIT();
}

void Gui::handleSystemMessages() {
  TRACE_FUNC_ENTER();
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
  TRACE_FUNC_EXIT();
}

void Gui::shutDown() {
  TRACE_INFO("Shutdown of module requested...");
  while (mWindowOpen) {
    sf::sleep(sf::milliseconds(5));
    // Wait for GUI to close window
  }
  mGuiThread->join();
  TRACE_INFO("Shutdown of module done");
}