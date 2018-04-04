#ifndef GUI_H
#define GUI_H

#include <thread>

#include <SFML/Graphics.hpp>

#include "../MessageHandler/MessageHandler.h"

class Gui {
public:
  Gui(MessageHandler* messageHandler);

  void shutDown();

private:
  bool m_running = true;
  std::thread* m_guiThread;
  MessageHandler* m_messageHandler;
  sf::RenderWindow* m_window;

  void render();

};

#endif // GUI_H