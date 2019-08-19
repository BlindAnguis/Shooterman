#pragma once

#include <list>

#include "../Resources/MenuBase.h"
#include "../Resources/GuiImageButton.h"
#include "../Resources/GuiInputText.h"
#include "../../Map.h"
#include "../Resources/GuiFrameComponent.h"

class MapEditor : public MenuBase {
public:
  MapEditor();
  ~MapEditor();

  void backgroundUpdate() override;
  bool checkMouse(sf::Vector2f mousePosition) override;
  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  std::list<std::shared_ptr<GuiImageButton>> mTextureButtons;
  Map mMap;
  Textures mCurrentTileId;
  sf::Sprite mCurrentSprite;
  std::string tmpMap;
  std::shared_ptr<Frame> mSavePopupFrame;
  std::shared_ptr<Frame> mLoadPopupFrame;
  std::shared_ptr<GuiInputText> mSavePopupInput;
  std::shared_ptr<GuiImageButton> addButton(Textures texture, std::string name);
  
  bool mRenderSavePopup = false;
  std::shared_ptr<GuiFrameComponent> mSavePopupBackground;

  bool mRenderLoadPopup = false;
  std::shared_ptr<GuiFrameComponent> mLoadPopupBackground;

  std::vector<std::string> getFilepaths();
  std::string filepathToFilename(std::string filepath);

  void onNewClicked();
  void onShowLoadClicked();
  void onLoadClicked(std::string filepath);
  void onLoadCancelClicked();
  void onShowSaveClicked();
  void onSaveClicked();
  void onSaveCancelClicked();
};

