#include "LobbyMenu.h"

#include <string>
#include <iostream>
#include <filesystem>
#include <fstream>

#include "../Resources/GuiText.h"
#include "../Resources/GuiImageButton.h"
#include "../Resources/GuiComponentFactory.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/Messages/LobbyDataMessage.h"
#include "../../../Common/Messages/ChangeUsernameMessage.h"
#include "../../../Common/Messages/CharacterChoosenMessage.h"
#include "../../../Common/Messages/PlayableCharactersMessage.h"
#include "../../../Common/Messages/MapMessage.h"
#include "../../../Common/MessageHandler/Subscriber.h"

LobbyMenu::LobbyMenu(bool server) {
  mName = "CLIENT: LOBBY_MENU";
  mServer = server;

  mServerReadySubscriber.addSignalCallback(MessageId::SERVER_READY, std::bind(&LobbyMenu::handleServerReadyMessage, this, std::placeholders::_1));
  mLobbySubscriber.addSignalCallback(MessageId::PLAYER_USERNAMES, std::bind(&LobbyMenu::handlePlayerUsernameMessage, this, std::placeholders::_1));
  mLobbySubscriber.addSignalCallback(MessageId::PLAYABLE_CHARACTERS, std::bind(&LobbyMenu::handlePlayableCharactersMessage, this, std::placeholders::_1));

  mGuiFrame = std::make_shared<Frame>();

  mGuiFrame->addGuiComponent(GCF::createHeader(GuiComponentPosition::TOP, "Join Lobby"));

  auto allPlayerList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::VERTICAL);
  auto myPlayerList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::HORIZONTAL);
  
  auto changeUsernameButton = std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Set username", [=]() {
    while (!mSubscribedToLobby) {
      mSubscribedToLobby = MessageHandler::get().subscribeTo(Interfaces::CLIENT_LOBBY, &mLobbySubscriber);
    }
    ChangeUsernameMessage cum(mUsernameText->getText());
    mLobbySubscriber.reverseSendMessage(cum.pack());
  });

  myPlayerList->addGuiComponent(changeUsernameButton);
  myPlayerList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::CENTER, "  "));
  mUsernameText = std::make_shared<GuiInputText>(GuiComponentPosition::CENTER, "Username", [=]() {
    while (!mSubscribedToLobby) {
      mSubscribedToLobby = MessageHandler::get().subscribeTo(Interfaces::CLIENT_LOBBY, &mLobbySubscriber);
    }
    ChangeUsernameMessage cum(mUsernameText->getText());
    mLobbySubscriber.reverseSendMessage(cum.pack());
  });
  mUsernameText->enableReceiveInput();
  addTextListener(mUsernameText);
  
  myPlayerList->addGuiComponent(mUsernameText);
  allPlayerList->addGuiComponent(myPlayerList);

  mPlayersList = std::make_shared<GuiList>(GuiComponentPosition::LEFT, GuiListDirection::VERTICAL);
  allPlayerList->addGuiComponent(mPlayersList);
  mGuiFrame->addGuiComponent(allPlayerList);

  auto rightList = std::make_shared<GuiList>(GuiComponentPosition::RIGHT, GuiListDirection::VERTICAL);
  mPlayableCharactersList = std::make_shared<GuiList>(GuiComponentPosition::RIGHT, GuiListDirection::HORIZONTAL);
  rightList->addGuiComponent(mPlayableCharactersList);
  rightList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, "Levels:", 50));

  if (server) {
    mFileList = std::make_shared<GuiList>(GuiComponentPosition::RIGHT, GuiListDirection::VERTICAL);
    rightList->addGuiComponent(mFileList);
  }

  mGuiFrame->addGuiComponent(rightList);


  auto lobbyMenuList = std::make_shared<GuiList>(GuiComponentPosition::BOTTOM, GuiListDirection::VERTICAL);
  if (server) {
    mStartGameButton = GCF::createGameStateButton(GuiComponentPosition::CENTER, "Start Game", GAME_STATE::PLAYING, true);
    lobbyMenuList->addGuiComponent(mStartGameButton);
  }
  lobbyMenuList->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Back", GAME_STATE::MAIN_MENU));
  mGuiFrame->addGuiComponent(lobbyMenuList);

  mLevelDirectories.emplace_back("Levels/UserCreated/");
  mLevelDirectories.emplace_back("Levels/DefaultLevels/");

  mSubscribedToLobby = false;
}

LobbyMenu::~LobbyMenu() { }

bool LobbyMenu::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  if (!mSubscribedToLobby) {
    mSubscribedToLobby = MessageHandler::get().subscribeTo(Interfaces::CLIENT_LOBBY, &mLobbySubscriber);
  }
  mLobbySubscriber.handleMessages();

  if (mServer) {
    if (!mSubscribedToServerReady) {
      mSubscribedToServerReady = MessageHandler::get().subscribeTo(Interfaces::CLIENT_SERVER_READY, &mServerReadySubscriber);
    }
    mServerReadySubscriber.handleMessages();
  }
  mGuiFrame->render(window);
  return true;
}

void LobbyMenu::init() {
  mPlayersList->clear();
  mPlayableCharactersList->clear();
  mSubscribedToLobby = false;
  if (mServer) {
    mSubscribedToServerReady = false;
    mStartGameButton->setDisabled();
  }

  if (mServer) {
    mFileList->clear();
    mFileButtonList.clear();
    for (auto levelDirectory : mLevelDirectories) {
      for (const auto& entry : std::filesystem::directory_iterator(levelDirectory)) {
        if (entry.path().string().find(".level") != std::string::npos) {
          std::string pathAsString = entry.path().string();
          int pos = pathAsString.find_last_of("/") + 1;
          auto fileName = pathAsString.substr(pos, pathAsString.length() - pos);
          pos = fileName.find_last_of(".");
          fileName = fileName.substr(0, pos);
          auto newButton = std::make_shared<GuiButton>(GuiComponentPosition::CENTER, fileName);
          mFileButtonList.push_back(newButton);
          newButton->setCallback([=]() {
            for (auto button : mFileButtonList) {
              button->unselect();
            }
            newButton->select();

            // Load file
            std::ifstream saveFile;
            std::string saveFileName(levelDirectory + newButton->getText() + ".level");
            saveFile.open(saveFileName);
            std::string data;
            std::string line;
            if (saveFile.is_open()) {
              while (!saveFile.eof()) {
                std::getline(saveFile, line);
                data += line;
                data += "\n";
              }

              MapMessage mm(data);
              mLobbySubscriber.reverseSendMessage(mm.pack());

            }
            else {
              TRACE_ERROR("Failed to open file: " << saveFileName);
            }
            saveFile.close();
          });
          mFileList->addGuiComponent(newButton);
        }
      }
    }
  }
}

void LobbyMenu::handlePlayerUsernameMessage(sf::Packet message) {
  TRACE_REC("PLAYER_USERNAMES");
  LobbyDataMessage ldm(message);

  mPlayersList->clear();
  for (auto playerName : ldm.getPlayerNames()) {
    mPlayersList->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, playerName));
  }
}

void LobbyMenu::handlePlayableCharactersMessage(sf::Packet message) {
  TRACE_REC("PLAYABLE_CHARACTERS");
  mPlayableCharactersList->clear();
  PlayableCharactersMessage pcm(message);

  for (auto playerClass : pcm.getPlayerClasses()) {
    sf::Sprite image;
    std::string playerClassName = "";

    // Choose the correct image and name of player class
    if (playerClass == PlayerClass::Archer) {
      playerClassName = "Archer";
      image = GuiResourceManager::getInstance().createSprite(Textures::CharacterArcher);
      image.setTextureRect(sf::IntRect(0, 64 * 10 + 14, 64, 50));
    } else if (playerClass == PlayerClass::Knight) {
      playerClassName = "Knight";
      image = GuiResourceManager::getInstance().createSprite(Textures::CharacterKnight);
    } else if (playerClass == PlayerClass::Mage) {
      playerClassName = "Mage";
      image = GuiResourceManager::getInstance().createSprite(Textures::CharacterMage);
    } else if (playerClass == PlayerClass::Spearman) {
      playerClassName = "Spearman";
      image = GuiResourceManager::getInstance().createSprite(Textures::CharacterSpearman);
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
        mSubscribedToLobby = MessageHandler::get().subscribeTo(Interfaces::CLIENT_LOBBY, &mLobbySubscriber);
      }

      CharacherChoosenMessage ccm(playerClass);
      mLobbySubscriber.reverseSendMessage(ccm.pack());
    });
    mPlayableCharactersList->addGuiComponent(newImageButton);
  }
}

void LobbyMenu::handleServerReadyMessage(sf::Packet message) {
  TRACE_REC("SERVER_READY");
  // when server is ready we activate the start game button
  mStartGameButton->setEnabled();
}
