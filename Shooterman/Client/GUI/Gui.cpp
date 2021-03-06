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
#include "ScoreMenu/ScoreMenu.h"
#include "../../Common/Constants.h"
#include "../../Common/Interfaces.h"
#include "../../Common/MessageId.h"
#include "../../Common/Messages/AddDebugButtonMessage.h"
#include "../../Common/Messages/RemoveDebugButtonMessage.h"
#include "../../Common/Messages/SpriteMessage.h"
#include "../../Common/Messages/MouseMessage.h"

Gui::Gui(std::shared_ptr<MessageHandler> messageHandler) 
  : mMessageHandler(messageHandler), 
  mInfoOverlay(std::make_shared<InfoOverlay>(messageHandler)),
  mSubscriber("GUI") {
  mName = "CLIENT: GUI";
}

void Gui::start() {
  TRACE_INFO("Starting module...");
  mSubscriber.addSignalCallback(MessageId::SHUT_DOWN, std::bind(&Gui::handleShutdownMessage, this, std::placeholders::_1));
  mSubscriber.addSignalCallback(MessageId::CHANGE_GAME_STATE, std::bind(&Gui::handleChangedGameStateMessage, this, std::placeholders::_1));

  mMessageHandler->subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSubscriber);
  mMessageHandler->subscribeTo(Interfaces::CLIENT_GAME_STATE, &mSubscriber);
  mMessageHandler->publishInterface(Interfaces::MOUSE_POSITION, &mMouseInterface);

  mMenuMap.emplace(GAME_STATE::DEBUG, std::list<std::shared_ptr<MenuBase>> { std::make_shared<DebugMenu>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::MAIN_MENU, std::list<std::shared_ptr<MenuBase>> { std::make_shared<MainMenu>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::LOBBY, std::list<std::shared_ptr<MenuBase>> { std::make_shared<LobbyMenu>(true, mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::CLIENT_LOBBY, std::list<std::shared_ptr<MenuBase>> { std::make_shared<LobbyMenu>(false, mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::JOIN, std::list<std::shared_ptr<MenuBase>> { std::make_shared<JoinMenu>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::PLAYING, std::list<std::shared_ptr<MenuBase>> { std::make_shared<PlayWindow>(mMessageHandler), std::make_shared<Hud>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::OPTIONS, std::list<std::shared_ptr<MenuBase>> { std::make_shared<OptionsMenu>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::PAUSE, std::list<std::shared_ptr<MenuBase>> { std::make_shared<PauseMenu>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::MAP_EDITOR, std::list<std::shared_ptr<MenuBase>> { std::make_shared<MapEditor>(mMessageHandler) });
  mMenuMap.emplace(GAME_STATE::SCORE_BOARD, std::list<std::shared_ptr<MenuBase>> { std::make_shared<ScoreMenu>(mMessageHandler) });

  // This needs to be after the DebugMenu is created
  setupDebugMessages("Client", "Gui", mMessageHandler);

  mWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(1024, 1024), "Shooterman");
  mWindowOpen = true;

  mCurrentGameState = GAME_STATE::MAIN_MENU;
  mLeftButtonAlreadyPressed = false;

  startListenToSubscriber(&mSubscriber);

  TRACE_INFO("Starting module done");
}

void Gui::run() {
  if (!mWindow || !mWindow->isOpen()) {
    return;
  }

  for (auto menues : mMenuMap) {
    for (auto menu : menues.second) {
      menu->backgroundUpdate();
    }
  }

  mRenderClock.restart();
  handleWindowEvents();

  bool renderNeeded = false;
  renderNeeded = renderGameState(mCurrentGameState);

  if (mRenderNeeded) {
    mWindow->display();
  }

  handleDebugMessages();

  auto currentFrameLength = mRenderClock.getElapsedTime().asMilliseconds();
  if (currentFrameLength < FRAME_LENGTH_IN_MS) {
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS - currentFrameLength));
  } else {
    // No need to sleep, frame took to long
    TRACE_DEBUG3("Frame length: " << currentFrameLength << " ms");
  }
}

void Gui::stop() {
  TRACE_INFO("Shutdown of module requested...");
  teardownDebugMessages();

  stopListenToSubscriber(&mSubscriber);

  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &mSubscriber);
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_GAME_STATE, &mSubscriber);
  mMessageHandler->unpublishInterface(Interfaces::MOUSE_POSITION);

  GuiResourceManager::getInstance().clear();
  mMenuMap.clear();

  mWindow->close();

  TRACE_INFO("Shutdown of module done");
}

void Gui::handleWindowEvents() {
  TRACE_FUNC_ENTER();
  sf::Event event;
  
  while (mWindow->pollEvent(event)) {
    if (event.type == sf::Event::Closed) {
      sf::Packet shutdownMessage;
      shutdownMessage << MessageId::SHUT_DOWN;
      Subscriber s("GUI");
      mMessageHandler->subscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
      s.reverseSendMessage(std::move(shutdownMessage));
      mMessageHandler->unsubscribeTo(Interfaces::CLIENT_SYSTEM_MESSAGE, &s);
    }
    if (event.type == sf::Event::MouseMoved) {
      sf::Vector2f offset = (sf::Vector2f)mWindow->getSize() - sf::Vector2f(1024, 1024);
      offset.x /= 2;
      offset.y /= 2;
      auto res = (sf::Vector2f)sf::Mouse::getPosition(*mWindow) - offset;
      MouseMessage mm((sf::Vector2i)res);
      mMouseInterface.pushMessage(std::move(mm.pack()));
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
    mInfoOverlay->render(mWindow, mousePosition);
    if (!mRenderNeeded) {
      return mRenderNeeded;
    }
  }
  TRACE_FUNC_EXIT();
  return mRenderNeeded;
}

void Gui::handleShutdownMessage(sf::Packet& message) {
  TRACE_INFO("Closing GUI window");
  mWindow->close();
}

void Gui::handleChangedGameStateMessage(sf::Packet& message) {
  GameStateMessage gsm(message);
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
}