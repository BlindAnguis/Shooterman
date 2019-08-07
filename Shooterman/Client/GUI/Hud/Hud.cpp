#include "Hud.h"

#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

Hud::Hud() {  
  mGuiFrame = std::make_shared<Frame>();
  mSubscriber.addSignalCallback(MessageId::PLAYER_DATA, std::bind(&Hud::handlePlayerData, this, std::placeholders::_1));
}

Hud::~Hud() { }

void Hud::reset() {
  mSubscribedToPlayerData = false;
  mUsernames.clear();
  mHealthBars.clear();
  mStaminaBars.clear();
  mManaBars.clear();
  mGuiFrame->clearGuiComponents();
  mSubscriber.clearMessages();
}

bool Hud::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  if (!mSubscribedToPlayerData) {
    mSubscribedToPlayerData = MessageHandler::get().subscribeTo(Interfaces::CLIENT_PLAYER_DATA, &mSubscriber);
  }

  mSubscriber.handleMessages();

  MenuBase::render(window, mousePosition);

  return true;
}

void Hud::handlePlayerData(sf::Packet& message) {
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
    mUsernames[i]->setPosition(playerData.position.x, playerData.position.y - playerData.globalBounds.height - 10);


    if (i >= mHealthBars.size()) {
      auto health = std::make_shared<GuiProgressBar>(GuiComponentPosition::FIXED_CENTER, 0.0f, 0.0f, 32.f, 6.f, sf::Color::Green);
      mHealthBars.push_back(health);
      mGuiFrame->addGuiComponent(health);
    }
    mHealthBars[i]->setMaxValue(playerData.maxHealth);
    mHealthBars[i]->setCurrentValue(playerData.currentHealth);
    mHealthBars[i]->setPosition(playerData.position.x, playerData.position.y - playerData.globalBounds.height + 5);

    if (playerData.hasMana) {
      if (mManaBars.find(i) == mManaBars.end()) {
        auto stamina = std::make_shared<GuiProgressBar>(GuiComponentPosition::FIXED_CENTER, -300.f, 300.f, 32.f, 6.f, sf::Color::Blue);
        mManaBars.emplace(i, stamina);
        mGuiFrame->addGuiComponent(stamina);
      }
      mManaBars[i]->setMaxValue(playerData.maxMana);
      mManaBars[i]->setCurrentValue(playerData.currentMana);
      mManaBars[i]->setPosition(playerData.position.x, playerData.position.y - playerData.globalBounds.height + 15);
    }

    if (playerData.hasStamina) {
      if (mStaminaBars.find(i) == mStaminaBars.end()) {
        auto mana = std::make_shared<GuiProgressBar>(GuiComponentPosition::FIXED_CENTER, -300.f, 300.f, 32.f, 6.f, sf::Color::Yellow);
        mStaminaBars.emplace(i, mana);
        mGuiFrame->addGuiComponent(mana);
      }
      mStaminaBars[i]->setMaxValue(playerData.maxStamina);
      mStaminaBars[i]->setCurrentValue(playerData.currentStamina);
      mStaminaBars[i]->setPosition(playerData.position.x, playerData.position.y - playerData.globalBounds.height + 15);
    }
  }
}
