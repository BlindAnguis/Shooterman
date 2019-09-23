#include "ScoreMenu.h"

#include "../Resources/GuiButton.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/MessageId.h"
#include "../../../Common/Messages/ScoreBoardMessage.h"

ScoreMenu::ScoreMenu(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler) {
  mName = "CLIENT: PAUSE_MENU";

  mMessageHandler->subscribeTo(Interfaces::CLIENT_SCORE_BOARD, &mScoreBoardSubscriber);
  mScoreBoardSubscriber.addSignalCallback(MessageId::SCORE_BOARD, std::bind(&ScoreMenu::handleScoreBoardMessage, this, std::placeholders::_1));

  mGuiFrame = std::make_shared<Frame>();
  mScoreList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);
  mGuiFrame->addGuiComponent(mScoreList);

  auto bottomList = std::make_shared<GuiList>(GuiComponentPosition::BOTTOM, GuiListDirection::HORIZONTAL);
  mGuiFrame->addGuiComponent(bottomList);
  bottomList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Back To Main Menu", std::bind(&ScoreMenu::onMainMenuClick, this)));
  bottomList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Back To Lobby", std::bind(&ScoreMenu::onLobbyMenuClick, this)));

}

ScoreMenu::~ScoreMenu() {}

void ScoreMenu::backgroundUpdate() {
  handleDebugMessages();
  mScoreBoardSubscriber.handleMessages();
}

void ScoreMenu::handleScoreBoardMessage(sf::Packet& packet) {
  ScoreBoardMessage sbm(packet);

  mScoreList->clear();

  for (auto playerScore : sbm.getPlayerScores()) {
    auto row = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
    
    row->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, playerScore.username + ":"));
    row->addGuiComponent(std::make_shared<GuiText>(GuiComponentPosition::LEFT, std::to_string(playerScore.score)));
    
    mScoreList->addGuiComponent(row);
  }
}

void ScoreMenu::onMainMenuClick() {}

void ScoreMenu::onLobbyMenuClick() {}
