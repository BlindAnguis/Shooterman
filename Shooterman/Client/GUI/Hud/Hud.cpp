#include "Hud.h"

#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

Hud::Hud(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mGuiFrame = std::make_shared<Frame>();
  mSubscriber.addSignalCallback(MessageId::PLAYER_DATA, std::bind(&Hud::handlePlayerData, this, std::placeholders::_1));
  mMessageHandler->subscribeToWithTimeout(Interfaces::CLIENT_PLAYER_DATA, &mSubscriber);
  mScoreList = std::make_shared<GuiList>(GuiComponentPosition::TOP_LEFT, GuiListDirection::VERTICAL);
  mGuiFrame->addGuiComponent(mScoreList);
}

Hud::~Hud() {
  mMessageHandler->unsubscribeTo(Interfaces::CLIENT_PLAYER_DATA, &mSubscriber);
}

void Hud::reset() {
  mUsernames.clear();
  mHealthBars.clear();
  mStaminaBars.clear();
  mManaBars.clear();
  mScores.clear();
  mScoreList->clear();
  mGuiFrame->clearGuiComponents();
  mGuiFrame->addGuiComponent(mScoreList);
  mSubscriber.clearMessages();
}

bool Hud::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  mWindow = window;
  mSubscriber.handleMessages();

  MenuBase::render(window, mousePosition);

  return true;
}

void Hud::handlePlayerData(sf::Packet& message) {
  sf::Vector2f windowSize = (sf::Vector2f)mWindow->getSize();
  sf::Vector2f offset = windowSize - sf::Vector2f(1024, 1024);
  offset.x /= 2;
  offset.y /= 2;
  PlayerDataMessage pdm(message);
  for (unsigned int i = 0; i < pdm.getNumberOfPlayerData(); ++i) {
    PlayerData playerData = pdm.getPlayerData(i);

    if (i >= mUsernames.size()) {
      auto username = std::make_shared<GuiText>(GuiComponentPosition::FIXED_CENTER, "", 18);
      username->setOutlineColor(sf::Color::Black);
      mUsernames.push_back(username);
      mGuiFrame->addGuiComponent(username);
    }
    mUsernames[i]->setText(playerData.username);
    mUsernames[i]->setPosition(playerData.position.x + offset.x, playerData.position.y - playerData.globalBounds.height - 10 + offset.y);


    if (i >= mHealthBars.size()) {
      auto health = std::make_shared<GuiProgressBar>(GuiComponentPosition::FIXED_CENTER, 0.0f, 0.0f, 32.f, 6.f, sf::Color::Green);
      mHealthBars.push_back(health);
      mGuiFrame->addGuiComponent(health);
    }
    mHealthBars[i]->setMaxValue(playerData.maxHealth);
    mHealthBars[i]->setCurrentValue(playerData.currentHealth);
    mHealthBars[i]->setPosition(playerData.position.x + offset.x, playerData.position.y - playerData.globalBounds.height + 5 + offset.y);

    if (playerData.hasMana) {
      if (mManaBars.find(i) == mManaBars.end()) {
        auto stamina = std::make_shared<GuiProgressBar>(GuiComponentPosition::FIXED_CENTER, -300.f, 300.f, 32.f, 6.f, sf::Color::Blue);
        mManaBars.emplace(i, stamina);
        mGuiFrame->addGuiComponent(stamina);
      }
      mManaBars[i]->setMaxValue(playerData.maxMana);
      mManaBars[i]->setCurrentValue(playerData.currentMana);
      mManaBars[i]->setPosition(playerData.position.x + offset.x, playerData.position.y - playerData.globalBounds.height + 15 + offset.y);
    }

    if (playerData.hasStamina) {
      if (mStaminaBars.find(i) == mStaminaBars.end()) {
        auto mana = std::make_shared<GuiProgressBar>(GuiComponentPosition::FIXED_CENTER, -300.f, 300.f, 32.f, 6.f, sf::Color::Yellow);
        mStaminaBars.emplace(i, mana);
        mGuiFrame->addGuiComponent(mana);
      }
      mStaminaBars[i]->setMaxValue(playerData.maxStamina);
      mStaminaBars[i]->setCurrentValue(playerData.currentStamina);
      mStaminaBars[i]->setPosition(playerData.position.x + offset.x, playerData.position.y - playerData.globalBounds.height + 15 + offset.y);
    }

    if (i >= mScores.size()) {
      auto score = std::make_shared<GuiText>(GuiComponentPosition::LEFT, "");
      mScores.push_back(score);
      mScoreList->addGuiComponent(score);
    }
    mScores[i]->setText(playerData.username + "\n" + std::to_string(playerData.score));
  }
}
