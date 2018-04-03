#ifndef MESSAGE_HANDLER_H
#define MESSAGE_HANDLER_H

#include <queue>
#include <mutex>

#include <SFML/Network.hpp>


class MessageHandler {
public:
	MessageHandler();

	void publishInputMessage(sf::Packet message);
  sf::Packet readInputMessage();

  void publishGraphicsMessage(sf::Packet message);
  sf::Packet readGraphicsMessage();

private:
  std::mutex m_inputQueueMutex;
  std::queue<sf::Packet> m_inputQueue;
  std::mutex m_graphicsQueueMutex;
  std::queue<sf::Packet> m_graphicsQueue;
};
#endif // ifndef MESSAGE_HANDLER_H