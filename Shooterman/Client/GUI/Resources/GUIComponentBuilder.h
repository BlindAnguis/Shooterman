#pragma once

#include <functional>
#include <string>

#include <SFML/Graphics.hpp>

#include "../../MessageHandler/MessageHandler.h"
#include "../../../Common/Trace.h"
#include "../../../Common/Messages/GameStateMessage.h"

class GUIComponent : Trace {
public:
  GUIComponent(std::string textString, std::string textFont, sf::Color textColor, sf::Color textHighlightColor, int textSize, int xPosition, int yPosition, std::function<void(void)>& callback);

  bool isPressed(sf::Vector2f mousePosition);
  void render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition);
  std::string getText();
  void setText(std::string newText);

private:
  sf::Text mComponentText;
  sf::Font mFont;
  sf::RectangleShape mBounds;
  sf::Color mTextColor;
  sf::Color mTextHighlightColor;
  std::function<void(void)> mCallback;

  bool checkMouse(sf::Vector2f mousePosition);
};

class GUIComponentBuilder : Trace {
public:
  GUIComponentBuilder() {
    mText = "";
    mTextFont = "RobbieRocketpants.ttf";
    mTextColor = sf::Color::Black;
    mTextHighlightColor = sf::Color::Black;
    mTextSize = 56;
    mXPosition = 0;
    mYPosition = 0;
    mCallback = nullptr;
  }

  GUIComponentBuilder* setText(std::string text) {
    mText = text;
    return this;
  }

  GUIComponentBuilder* setFont(std::string textFont) {
    mTextFont = textFont;
    return this;
  }

  GUIComponentBuilder* setTextColor(sf::Color textColor) {
    mTextColor = textColor;
    return this;
  }

  GUIComponentBuilder* setTextHighlightColor(sf::Color textHighlightColor) {
    mTextHighlightColor = textHighlightColor;
    return this;
  }

  GUIComponentBuilder* setTextSize(int textSize) {
    mTextSize = textSize;
    return this;
  }

  GUIComponentBuilder* setXPosition(int xPosition) {
    mXPosition = xPosition;
    return this;
  }

  GUIComponentBuilder* setYPosition(int yPosition) {
    mYPosition = yPosition;
    return this;
  }

  GUIComponentBuilder* setCallback(const std::function<void(void)>& callback) {
    mCallback = callback;
    return this;
  }

  std::shared_ptr<GUIComponent> build() {
    return std::make_shared<GUIComponent>(mText, mTextFont, mTextColor, mTextHighlightColor, mTextSize, mXPosition, mYPosition, mCallback);
  }

  static std::shared_ptr<GUIComponent> createTitle(std::string textString, int xPosition, int yPosition) {
    GUIComponentBuilder builder;
    return builder.setText(textString)->setXPosition(xPosition)->setYPosition(yPosition)->setTextSize(72)->build();
  }

  static std::shared_ptr<GUIComponent> createText(std::string textString, int xPosition, int yPosition) {
    GUIComponentBuilder builder;
    return builder.setText(textString)->setXPosition(xPosition)->setYPosition(yPosition)->setTextSize(36)->build();
  }

  static std::shared_ptr<GUIComponent> createCustomActionButton(std::string textString, int xPosition, int yPosition, const std::function<void(void)>& callback) {
    GUIComponentBuilder builder;
    return builder.setText(textString)->setTextHighlightColor(sf::Color(0, 120, 0))->setXPosition(xPosition)->setYPosition(yPosition)->setCallback(callback)->build();
  }

  static std::shared_ptr<GUIComponent> createGameStateButton(std::string textString, int xPosition, int yPosition, GAME_STATE gameState) {
    GUIComponentBuilder builder;
    auto callback = [gameState]() {
      GameStateMessage gsm(gameState);
      MessageHandler::get().pushGameStateMessage(gsm.pack());
    };
    return builder.setText(textString)->setTextHighlightColor(sf::Color(0, 120, 0))->setXPosition(xPosition)->setYPosition(yPosition)->setCallback(callback)->build();
  }

private:
  std::string mText;
  std::string mTextFont;
  sf::Color mTextColor;
  sf::Color mTextHighlightColor;
  int mTextSize;
  int mXPosition;
  int mYPosition;
  std::function<void(void)> mCallback;
};