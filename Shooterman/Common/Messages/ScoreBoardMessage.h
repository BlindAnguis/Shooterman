#pragma once
#include "Message.h"
#include "../MessageId.h"

struct PlayerScore {
  std::string username;
  int score;
};

class ScoreBoardMessage : public Message {
public:
  ScoreBoardMessage();
  ScoreBoardMessage(sf::Packet packet);
  ~ScoreBoardMessage();

  void unpack(sf::Packet packet) override;
  sf::Packet pack() override;

  void addPlayerScore(PlayerScore playerScore);
  std::vector<PlayerScore> getPlayerScores();

private:
  std::vector<PlayerScore> mPlayerScores;
};

