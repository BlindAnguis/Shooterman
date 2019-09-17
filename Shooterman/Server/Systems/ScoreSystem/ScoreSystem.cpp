#include "ScoreSystem.h"

#include "../../../Common/MessageId.h"
#include "../../../Common/Interfaces.h"
#include "../../../Common/Messages/DeathMessage.h"

ScoreSystem::ScoreSystem(std::shared_ptr<MessageHandler> messageHandler) : mMessageHandler(messageHandler), mSubscriber("SS") {
  mSubscriber.addSignalCallback(MessageId::PLAYER_DEATH, std::bind(&ScoreSystem::handleDeathMessage, this, std::placeholders::_1));

  mMessageHandler->subscribeToWithTimeout(Interfaces::SERVER_PLAYER_DEATH, &mSubscriber);
}


ScoreSystem::~ScoreSystem() {
  mMessageHandler->unsubscribeTo(Interfaces::SERVER_PLAYER_DEATH, &mSubscriber);
}

void ScoreSystem::update()   {
  mSubscriber.handleMessages();
}

int ScoreSystem::getEntityScore(int entityId) {
  auto entityScoreIt = mEntityScores.find(entityId);
  if (entityScoreIt != mEntityScores.end()) {
    return entityScoreIt->second;
  }
  return 0;
}

void ScoreSystem::handleDeathMessage(sf::Packet packet) {
  DeathMessage dm(packet);

  if (dm.getKillerEntity() == -1) {
    // Killed by environment
    return;
  }

  TRACE_INFO("Player " << dm.getDeadEntity() << " killed by " << dm.getKillerEntity());

  auto entityScoreIt = mEntityScores.find(dm.getKillerEntity());
  if (entityScoreIt != mEntityScores.end()) {
    entityScoreIt->second += 100;
  } else {
    mEntityScores.emplace(dm.getKillerEntity(), 100);
  }
}
