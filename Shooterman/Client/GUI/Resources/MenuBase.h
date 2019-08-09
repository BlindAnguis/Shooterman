#pragma once

#include <list>

#include <SFML/Graphics/RenderWindow.hpp>

#include "GuiFrame.h"
#include "GuiInputText.h"
#include "../../../Common/Trace.h"

class MenuBase : public Trace {
public:
  virtual ~MenuBase();
  virtual void init();
  virtual void uninit();
  virtual void reset();
  virtual void backgroundUpdate();
  virtual bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);
  virtual bool checkMouse(sf::Vector2f mousePosition);
  void handleNewText(sf::Uint32 newChar);
  
protected:
  std::shared_ptr<Frame> mGuiFrame;

  void addTextListener(std::shared_ptr<GuiInputText> textListener);

private:
  std::list<std::shared_ptr<GuiInputText>> mTextListeners;
};