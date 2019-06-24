#pragma once

#include <list>

#include "../Resources/MenuBase.h"
#include "../Resources/GuiImageButton.h"
#include "../../Map.h"

class MapEditor : public MenuBase {
public:
  MapEditor();
  ~MapEditor();

  bool checkMouse(sf::Vector2f mousePosition) override;
  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  std::list<std::shared_ptr<GuiImageButton>> mTextureButtons;
  Map mMap;
  Textures mCurrentTileId;
  sf::Sprite mCurrentSprite;
  std::string tmpMap;

  std::shared_ptr<GuiImageButton> addButton(Textures texture, std::string name);
};

