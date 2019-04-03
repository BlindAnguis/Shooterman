#include "Hud.h"

#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"

Hud::Hud() {
  mHealthText = GUIComponentBuilder::createText("Health:", 150, 50, sf::Color::White);
  mComponentList.push_back(mHealthText);

  mManaText = GUIComponentBuilder::createText("Mana: ", 750, 50, sf::Color::White);
  mComponentList.push_back(mManaText);
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
      mHealthText->setText("Health: " + std::to_string(pdm.getCurrentHealth()));
      auto currentMana = pdm.getCurrentMana();
      if (currentMana > -1) {
        mManaText->setText("Mana: " + std::to_string(pdm.getCurrentMana()));
      } else { // If Mana is -1 (or less) it means the Player doesn't have mana.
        mManaText->setText("");
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