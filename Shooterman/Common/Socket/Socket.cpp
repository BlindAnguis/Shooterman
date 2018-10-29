#include "Socket.h"

void Socket::send(std::vector<sf::Packet> packets) {
  for (auto packet : packets) {
    send(packet);
  }
}