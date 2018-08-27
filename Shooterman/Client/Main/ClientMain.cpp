#include "ClientMain.h"

ClientMain::ClientMain() {
  mName = "CLIENTMAIN";
  MessageHandler::get().subscribeToSystemMessages(&mSystemMessageSubscriber);
  Input input = Input();
  Gui gui = Gui();
  GameLoop server = GameLoop();
  //Sound sound = Sound();

  mServerStarted = false;
  mCurrentGameState = GAME_STATE::STATE_MAIN_MENU;

  while (mRunning) {
    switch (mCurrentGameState) {
    case GAME_STATE::STATE_MAIN_MENU: {
      // Stop Server
      if (mServerStarted) {
        server.stop();
        mServerStarted = false;
      }
      break; }
    case GAME_STATE::STATE_LOBBY:
      // In the lobby
      break;
    case GAME_STATE::STATE_PLAYING: {
      // Start server
      if (!mServerStarted) {
        server.start();
        mServerStarted = true;
      }
      break; }
    default:
      TRACE_ERROR("Unknown game state: " << mCurrentGameState);
    }
    sf::sleep(sf::milliseconds(FRAME_LENGTH_IN_MS));
    handleSystemMessages();
  }
  input.shutDown();
  gui.shutDown();
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
    case CHANGE_GAME_STATE: {
      GameStateMessage gsm;
      gsm.unpack(systemMessage);
      mCurrentGameState = gsm.getGameState();
      break; }
    default:
      TRACE_WARNING("Unknown system message: " << messageId);
    }
  }
}