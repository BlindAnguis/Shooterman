#include "ClientMain.h"

ClientMain::ClientMain() {
  mName = "CLIENTMAIN";
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  MessageHandler::get().subscribeToGameStateMessages(&mGameStateMessageSubscriber);
  Input input = Input();
  Gui gui = Gui();
  GameLoop server = GameLoop();
  //Sound sound = Sound();

  mServerStarted = false;
  mCurrentGameState = GAME_STATE::MAIN_MENU;

  while (mRunning) {
    switch (mCurrentGameState) {
    case GAME_STATE::MAIN_MENU: {
      // Stop Server
      if (mServerStarted) {
        server.stop();
        mServerStarted = false;
      }
      break; }
    case GAME_STATE::LOBBY: {
      // In the lobby
      if (!mServerStarted) {
        server.start();
        mServerStarted = true;
      }
      break; }
    case GAME_STATE::PLAYING: {
      // Start server
      break; }
    default:
      TRACE_ERROR("Unknown game state: " << mCurrentGameState);
    }
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
    handleGameStateMessages();
    handleSystemMessages();
  }
  input.shutDown();
  gui.shutDown();
  MessageHandler::get().unsubscribeAll(&mSystemMessageSubscriber);
}

void ClientMain::handleSystemMessages() {
  std::queue<sf::Packet> systemMessageQueue = mSystemMessageSubscriber.getMessageQueue();
  sf::Packet systemMessage;
  while (!systemMessageQueue.empty()) {
    systemMessage = systemMessageQueue.front();
    systemMessageQueue.pop();

    auto messageId = 0;
    systemMessage >> messageId;
    switch (messageId) {
    case SHUT_DOWN:
      TRACE_INFO("Preparing to shut down");
      mRunning = false;
      break;
    default:
      TRACE_WARNING("Unknown system message: " << messageId);
    }
  }
}

void ClientMain::handleGameStateMessages() {
  std::queue<sf::Packet> gameStateMessageQueue = mGameStateMessageSubscriber.getMessageQueue();
  sf::Packet gameStateMessage;
  while (!gameStateMessageQueue.empty()) {
    gameStateMessage = gameStateMessageQueue.front();
    gameStateMessageQueue.pop();

    GameStateMessage gsm;
    gsm.unpack(gameStateMessage);
    mCurrentGameState = gsm.getGameState();
  }
}