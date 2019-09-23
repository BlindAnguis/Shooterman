#pragma once

#include "../../Components/ComponentManager.h"
#include "../../Components/Components.h"
#include "../../../Common/Trace.h"
#include "../../../Common/MessageHandler/MessageHandler.h"
#include "../../../Common//MessageHandler/Subscriber.h"

class ScoreSystem : public Trace {
public:
  ScoreSystem(std::shared_ptr<MessageHandler> messageHandler);
  ~ScoreSystem();

  void update();

  int getEntityScore(int entityId);

private:
  std::shared_ptr<MessageHandler> mMessageHandler;
  Subscriber mSubscriber;
  std::map<int, int> mEntityScores;

  void handleDeathMessage(sf::Packet packet);
};

