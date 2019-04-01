#include "Hud.h"

#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/PlayerDataMessage.h"

Hud::Hud() {
  auto test = GUIComponentBuilder::createText("Health: ", 50, 50);
  mComponentList.push_back(test);
  mHealthText = GUIComponentBuilder::createText("Health: ", 50 + test->getWidth(), 50);
  mComponentList.push_back(mHealthText);
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
      mHealthText->setText(std::to_string(pdm.getCurrentHealth()));
    }
      break;
    default:
      TRACE_ERROR("Unhandled message: " << id);
    }
  }

  MenuBase::render(window, mousePosition);

  return true;
}