#include "LobbyMenu.h"

#include "../Resources/GuiText.h"
#include "../Resources/GuiImageButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/Messages/LobbyDataMessage.h"
#include "../../../Common/Messages/ChangeUsernameMessage.h"
#include "../../../Common/Messages/CharacterChoosenMessage.h"
#include "../../../Common/Messages/PlayableCharactersMessage.h"
#include "../../../Common/MessageHandler/Subscriber.h"

LobbyMenu::LobbyMenu(bool server) {
  mName = "CLIENT: LOBBY_MENU";
  mServer = server;
  mGuiFrame = std::make_shared<Frame>();

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Join Lobby"));

  auto allPlayerList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::VERTICAL);
  auto myPlayerList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::HORIZONTAL);
  
  auto changeUsernameButton = std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Set username", [=]() {
    while (!mSubscribedToLobby) {
      mSubscribedToLobby = MessageHandler::get().subscribeTo("ClientLobby", &mLobbySubscriber);
    }
    TRACE_INFO("Username: " << mUsername);
    ChangeUsernameMessage cum(mUsername);
    mLobbySubscriber.reverseSendMessage(cum.pack());
  });

  myPlayerList->addGuiComponent(changeUsernameButton);
  myPlayerList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::CENTER, "  "));
  mUsernameText = std::make_shared<GuiInputText>(GuiComponentPosition::CENTER, "Username");
  mUsernameText->enableReceiveInput();
  
  myPlayerList->addGuiComponent(mUsernameText);
  allPlayerList->addGuiComponent(myPlayerList);

  mPlayersList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::VERTICAL);
  allPlayerList->addGuiComponent(mPlayersList);
  mGuiFrame->addGuiComponent(allPlayerList);

  mPlayableCharactersList = std::make_shared<GuiList>(GuiComponentPosition::RIGHT, GuiListDirection::HORIZONTAL);

  mGuiFrame->addGuiComponent(mPlayableCharactersList);

  auto lobbyMenuList = std::make_shared<GuiList>(GuiComponentPosition::BOTTOM, GuiListDirection::VERTICAL);
  if (server) {
    mStartGameButton = GCF::createGameStateButton(GuiComponentPosition::CENTER, "Start Game", GAME_STATE::PLAYING, true);
    lobbyMenuList->addGuiComponent(mStartGameButton);
  }
  lobbyMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::PREVIOUS));
  mGuiFrame->addGuiComponent(lobbyMenuList);

  mSubscribedToLobby = false;
}

LobbyMenu::~LobbyMenu() { }

bool LobbyMenu::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  checkForLobbyMessages();
  if (mServer) {
    checkForServerReadyMessage();
  }
  mGuiFrame->render(window);
  return true;
}

void LobbyMenu::handleNewText(sf::Uint32 newChar) {
  if (newChar == 8) {
    // Backspace
    mUsername = mUsername.substr(0, mUsername.size() - 1);
    mUsernameText->removeChar();
  } else if (mUsername.length() < 15) {
    mUsername += newChar;
    mUsernameText->addChar(newChar);
  }
}

void LobbyMenu::init() {
  mPlayersList->clear();
  mPlayableCharactersList->clear();
  mSubscribedToLobby = false;
  if (mServer) {
    mSubscribedToServerReady = false;
    mStartGameButton->setDisabled();
  }
}

void LobbyMenu::checkForLobbyMessages() {
  if (!mSubscribedToLobby) {
    mSubscribedToLobby = MessageHandler::get().subscribeTo("ClientLobby", &mLobbySubscriber);
  }

  auto messages = mLobbySubscriber.getMessageQueue();

  sf::Packet packet;
  int messageID = 0;
  while (!messages.empty()) {
    packet = messages.front();
    messages.pop();
    packet >> messageID;
    if (messageID == PLAYER_USERNAMES) {
      TRACE_REC("PLAYER_USERNAMES");
      LobbyDataMessage ldm(packet);

      mPlayersList->clear();
      for (auto playerName : ldm.getPlayerNames()) {
        mPlayersList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, playerName));
      }

    } else if (messageID == PLAYABLE_CHARACTERS) {
      TRACE_REC("PLAYABLE_CHARACTERS");
      mPlayableCharactersList->clear();
      PlayableCharactersMessage pcm(packet);

      for (auto playerClass : pcm.getPlayerClasses()) {
        sf::Sprite image;
        std::string playerClassName = "";

        // Choose the correct image and name of player class
        if (playerClass == PlayerClass::Archer) {
          playerClassName = "Archer";
          image = GuiResourceManager::getInstance().createSprite(Textures::Tombstone); // TODO: Change to archer when added
          image.setTextureRect(sf::IntRect(0, 64 * 10 + 14, 64, 50));
          continue; // TODO: Remove when archer is added 
        } else if (playerClass == PlayerClass::Knight) {
          playerClassName = "Knight";
          image = GuiResourceManager::getInstance().createSprite(Textures::CharacterKnight);
          image.setTextureRect(sf::IntRect(0, 64 * 10 + 14, 64, 50));
        } else if (playerClass == PlayerClass::Mage) {
          playerClassName = "Mage";
          image = GuiResourceManager::getInstance().createSprite(Textures::CharacterMage);
          image.setTextureRect(sf::IntRect(0, 64 * 10 + 14, 64, 50));
        } else if (playerClass == PlayerClass::Spearman) {
          playerClassName = "Spearman";
          image = GuiResourceManager::getInstance().createSprite(Textures::CharacterSpearman);
          image.setTextureRect(sf::IntRect(0, 64 * 10 + 14, 64, 50));
        }

        // First create button
        std::shared_ptr<GuiImageButton> newImageButton = std::make_shared<GuiImageButton>(GuiComponentPosition::CENTER, playerClassName, image);
        // Then add callback after the button is created, since it is needed in the lambda
        newImageButton->setCallback([=]() {
          // Clear selection of ALL image buttons in mPlayableCharactersList
          for (int i = 0; i < mPlayableCharactersList->getNumberOfComponents(); ++i) {
            std::shared_ptr<GuiImageButton> button = std::static_pointer_cast<GuiImageButton>(mPlayableCharactersList->getComponent(i));
            button->unselect();
          }
          // Select the one that was clicked
          newImageButton->select();

          while (!mSubscribedToLobby) {
            mSubscribedToLobby = MessageHandler::get().subscribeTo("ClientLobby", &mLobbySubscriber);
          }

          CharacherChoosenMessage ccm(playerClass);
          mLobbySubscriber.reverseSendMessage(ccm.pack());
        });
        mPlayableCharactersList->addGuiComponent(newImageButton);
      }
    } else {
      TRACE_WARNING("Received unhandled message: " << messageID);
    }
  }
}


void LobbyMenu::checkForServerReadyMessage() {
  if (!mSubscribedToServerReady) {
    mSubscribedToServerReady = MessageHandler::get().subscribeTo("ClientServerReady", &mServerReadySubscriber);
  }

  auto messages = mServerReadySubscriber.getMessageQueue();

  sf::Packet serverReadyMessage;
  int messageID = 0;
  while (!messages.empty()) {
    serverReadyMessage = messages.front();
    messages.pop();
    serverReadyMessage >> messageID;
    if (messageID == SERVER_READY) {
      TRACE_REC("SERVER_READY");
      // when server is ready we activate the start game button
      mStartGameButton->setEnabled();
    }
    else {
      TRACE_WARNING("Received unhandled message: " << messageID);
    }
  }
}