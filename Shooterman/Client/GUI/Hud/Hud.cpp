#include "Hud.h"

#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"

#include "../Resources/GUIProgressBar.h"

Hud::Hud() {  
  mHealthBar = std::make_shared<GUIProgressBar>(-300.f, 300.f, 32.f, 6.f, sf::Color::Green);
  mManaBar = std::make_shared<GUIProgressBar>(-300.f, 300.f, 32.f, 6.f, sf::Color::Blue);
  mStaminaBar = std::make_shared<GUIProgressBar>(-300.f, 300.f, 32.f, 6.f, sf::Color::Yellow);
  mComponentList.push_back(mHealthBar);
  mComponentList.push_back(mManaBar);
  mComponentList.push_back(mStaminaBar);
}

Hud::~Hud() { }

bool Hud::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  if (!mSubscribedToPlayerData) {
    mSubscribedToPlayerData = MessageHandler::get().subscribeTo("ClientPlayerData", &mPlayerDataSubscriber);
  }

  auto playerDataMessages = mPlayerDataSubscriber.getMessageQueue();

  while (playerDataMessages.size() > 0) {
    auto playerDataMessage = playerDataMessages.front();
    playerDataMessages.pop();

    int id = -1;
    playerDataMessage >> id;

    switch (id) {
    case PLAYER_DATA:
    {
      PlayerDataMessage pdm;
      pdm.unpack(playerDataMessage);
      mHealthBar->setMaxValue(pdm.getMaxHealth());
      mHealthBar->setCurrentValue(pdm.getCurrentHealth());
      mHealthBar->setPosition(pdm.getPosition().x - (pdm.getGlobalBounds().width / 4), pdm.getPosition().y - pdm.getGlobalBounds().height + 5);

      if (pdm.hasMana()) {
        mManaBar->setMaxValue(pdm.getMaxMana());
        mManaBar->setCurrentValue(pdm.getCurrentMana());
        mManaBar->setPosition(pdm.getPosition().x - (pdm.getGlobalBounds().width / 4), pdm.getPosition().y - pdm.getGlobalBounds().height + 15);
      }

      if (pdm.hasStamina()) {
        mStaminaBar->setMaxValue(pdm.getMaxStamina());
        mStaminaBar->setCurrentValue(pdm.getCurrentStamina());
        mStaminaBar->setPosition(pdm.getPosition().x - (pdm.getGlobalBounds().width / 4), pdm.getPosition().y - pdm.getGlobalBounds().height + 15);
      }
    }
      break;
    default:
      TRACE_ERROR("Unhandled message: " << id);
    }
  }

  MenuBase::render(window, mousePosition);

  return true;
}