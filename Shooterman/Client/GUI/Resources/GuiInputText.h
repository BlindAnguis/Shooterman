#pragma once

#include "GuiText.h"

class GuiInputText : public GuiText {
public:
  GuiInputText(GuiComponentPosition guiComponentPosition, std::string text, int fontSize = 36, FONT font = FONT::MORRIS);

  void render(std::shared_ptr<sf::RenderWindow> window, int xPosition, int yPosition, int width, int height) override;
  bool checkMouse(sf::Vector2f mousePosition) override;

  void setText(std::string newText) override;
  void addChar(sf::Uint32 newChar);
  void removeChar();

  void enableReceiveInput();
  void disableReceiveInput();

private:
  bool mReceiveInputEnabled = false;
  bool mReceiveInput = false;
  bool mCursorEnabled = true;
  std::string mDefaultText;
  std::string mCurrentText;
  sf::Clock mCursorClock;
};