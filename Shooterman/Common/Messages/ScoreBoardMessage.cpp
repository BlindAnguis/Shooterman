#include "ScoreBoardMessage.h"



ScoreBoardMessage::ScoreBoardMessage() {}

ScoreBoardMessage::ScoreBoardMessage(sf::Packet packet) {
  unpack(packet);
}


ScoreBoardMessage::~ScoreBoardMessage() {}

void ScoreBoardMessage::unpack(sf::Packet packet) {
  int nrOfPlayerScores = 0;
  packet >> nrOfPlayerScores;

  for (int i = 0; i < nrOfPlayerScores; ++i) {
    PlayerScore playerScore;
    packet >> playerScore.username;
    packet >> playerScore.score;
    mPlayerScores.push_back(playerScore);
  }
}

sf::Packet ScoreBoardMessage::pack() {
  sf::Packet packet;
  packet << MessageId::SCORE_BOARD;
  packet << mPlayerScores.size();

  for (auto playerScore : mPlayerScores) {
    packet << playerScore.username;
    packet << playerScore.score;
  }

  return packet;
}

void ScoreBoardMessage::addPlayerScore(PlayerScore playerScore) {
  mPlayerScores.push_back(playerScore);
}

std::vector<PlayerScore> ScoreBoardMessage::getPlayerScores() {
  return mPlayerScores;
}
