#include "MainMenu.h"

MainMenu::MainMenu() {
  mName = "GUI";
  mDebugEnabled = true;
  std::string fileName = "RobbieRocketpants.ttf";
  if (!mFont.loadFromFile(FONT_FILE_PATH + fileName)) {
    TRACE_ERROR("Could not load file " << fileName);
  }

  sf::Color textColor = sf::Color::Black;
  sf::Color textHighlightColor = sf::Color(0, 120, 0);

  mButtonList.push_back(new GUIButton("Shooterman", mFont, textColor, textColor, 72, 250, 36, NO_ACTION));

  mButtonList.push_back(new GUIButton("Host Game", mFont, textColor, textHighlightColor, 56, 250, 200, B_LOBBY));
  mButtonList.push_back(new GUIButton("Join Game", mFont, textColor, textHighlightColor, 56, 250, 260, NO_ACTION));
  mButtonList.push_back(new GUIButton("Options", mFont, textColor, textHighlightColor, 56, 250, 320, NO_ACTION));
  mButtonList.push_back(new GUIButton("Exit Game", mFont, textColor, textHighlightColor, 56, 250, 380, QUIT));
}

MainMenu::~MainMenu() {
  for (GUIButton* button : mButtonList) {
    delete button;
  }
}

BUTTON_PRESSED MainMenu::checkMouse(sf::Vector2f mousePosition) {
  BUTTON_PRESSED buttonPressed = NO_ACTION;
  for (GUIButton* button : mButtonList) {
    if (buttonPressed == NO_ACTION) {
      buttonPressed = button->checkMouse(mousePosition);
    } else {
      // Need to iterate over all buttons, so we update them
      button->checkMouse(mousePosition);
    }
  }
  return buttonPressed;
}

void MainMenu::render(sf::RenderWindow* window) {
  for (GUIButton* button : mButtonList) {
    button->render(window);
  }
}