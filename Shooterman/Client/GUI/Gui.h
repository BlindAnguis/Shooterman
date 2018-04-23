#ifndef GUI_H
#define GUI_H

#include <thread>

#include <SFML/Graphics.hpp>

#include "../MessageHandler/MessageHandler.h"

class Gui {
public:
  Gui();

  void shutDown();

private:
  bool m_running = true;
  std::thread* m_guiThread;
  sf::RenderWindow* m_window;

  void render();

};

#endif // GUI_H