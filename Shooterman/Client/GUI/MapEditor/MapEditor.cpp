#include "MapEditor.h"

#include <iostream>
#include <fstream>

#include <SFML/Window/Mouse.hpp>

#include "../Resources/GuiList.h"
#include "../Resources/GuiExpandableList.h"
#include "../Resources/GuiComponentFactory.h"

MapEditor::MapEditor() {
  mName = "CLIENT: MAP_EDITOR";
  mMap.clear();
  mMap.createDefaultMap();
  mGuiFrame = std::make_shared<Frame>();
  
  auto toolbar = std::make_shared<GuiList>(GuiComponentPosition::RIGHT, GuiListDirection::VERTICAL);

  auto filesList1 = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  filesList1->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "New", [=]() {
    mMap.clear();
    mMap.createDefaultMap();
  }));
  filesList1->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Load", [=]() {
    std::ifstream saveFile;
    saveFile.open("Levels/UserCreated/test.level");
    std::string data;
    std::string line;
    if (saveFile.is_open()) {
      while (!saveFile.eof()) {
        std::getline(saveFile, line);
        data += line;
        data += "\n";
      }
      mMap.fromString(data);
    } else {
      TRACE_ERROR("Failed to open file");
    }
    saveFile.close();
  }));
  toolbar->addGuiComponent(filesList1);

  auto filesList2 = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  filesList2->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Save", [=]() {
    // Open new popup
    mRenderSavePopup = true;
  }));

  filesList2->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Button", [=]() {
    TRACE_INFO("Why are you pressing this button?");
  }));
  toolbar->addGuiComponent(filesList2);

  auto floorList = std::make_shared<GuiExpandableList>(GuiComponentPosition::CENTER, "Floors");
  floorList->addGuiComponent(addButton(Textures::FloorWhole, "Floor"));
  floorList->addGuiComponent(addButton(Textures::FloorCracked, "Cracked"));
  toolbar->addGuiComponent(floorList);

  auto trapList = std::make_shared<GuiExpandableList>(GuiComponentPosition::CENTER, "Traps");
  trapList->addGuiComponent(addButton(Textures::FloorSpikes, "Spikes"));
  toolbar->addGuiComponent(trapList);

  auto wallList = std::make_shared<GuiExpandableList>(GuiComponentPosition::CENTER, "Walls");
  wallList->addGuiComponent(addButton(Textures::HorizontalWall1, "Horizontal"));
  wallList->addGuiComponent(addButton(Textures::VerticalWall1, "Vertical"));
  toolbar->addGuiComponent(wallList);


  toolbar->addGuiComponent(GCF::createGameStateButton(GuiComponentPosition::CENTER, "Close Editor", GAME_STATE::PREVIOUS));
  mGuiFrame->addGuiComponent(toolbar);

  // Create save popup
  mPopupFrame = std::make_shared<Frame>();
  auto saveHeadline = std::make_shared<GuiText>(GuiComponentPosition::TOP, "Save map");
  mPopupFrame->addGuiComponent(saveHeadline);
  auto filesList3 = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  mSavePopupInput = std::make_shared<GuiInputText>(GuiComponentPosition::CENTER, "Filename");
  mSavePopupInput->enableReceiveInput();
  filesList3->addGuiComponent(mSavePopupInput);
  filesList3->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Save", [=]() {
    std::ofstream saveFile;
    std::string mapPath("Levels/UserCreated/" + mSavePopupInput->getText() + ".level");
    saveFile.open(mapPath);
    if (saveFile.is_open()) {
      saveFile << mMap.toString();
    }
    else {
      TRACE_ERROR("Failed to open file: " << mapPath);
    }
    saveFile.close();
    mRenderSavePopup = false;
  }));
  mPopupFrame->addGuiComponent(filesList3);
}

MapEditor::~MapEditor() { }

bool MapEditor::checkMouse(sf::Vector2f mousePosition) {
  if (mRenderSavePopup && mPopupFrame->checkMouse(mousePosition)) {
    return true;
  }

  if (mGuiFrame->checkMouse(mousePosition)) {
    return true;
  }

  if (sf::Mouse::isButtonPressed(sf::Mouse::Left) &&
    mousePosition.x > 0 && mousePosition.x < 1024 &&
    mousePosition.y > 0 && mousePosition.y < 1024) {
    mMap.setTile((int)(mousePosition.x / 32), (int)(mousePosition.y / 32), mCurrentTileId, mCurrentSprite);
  }
  if (sf::Mouse::isButtonPressed(sf::Mouse::Right) &&
    mousePosition.x > 0 && mousePosition.x < 1024 &&
    mousePosition.y > 0 && mousePosition.y < 1024) {
    mMap.setTile((int)(mousePosition.x / 32), (int)(mousePosition.y / 32), Textures::Unknown, sf::Sprite());
  }
  return false;
}

bool MapEditor::render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) {
  for (int x = 0; x < 32; ++x) {
    for (int y = 0; y < 32; ++y) {
      sf::Sprite sprite = mMap.getTile(x, y).second;
      sprite.setPosition((float)(x * 32), (float)(y * 32));
      window->draw(sprite);
    }
  }

  mGuiFrame->render(window);
  if (mRenderSavePopup) {
    mPopupFrame->render(window);
  }
  return true;
}

std::shared_ptr<GuiImageButton> MapEditor::addButton(Textures texture, std::string name) {
  sf::Sprite image = GuiResourceManager::getInstance().createSprite(texture);
  auto button = std::make_shared<GuiImageButton>(GuiComponentPosition::CENTER, name, image);
  mTextureButtons.push_back(button);
  button->setCallback([=]() {
    for (auto button : mTextureButtons) {
      button->unselect();
    }
    button->select();
    mCurrentTileId = texture;
    mCurrentSprite = image;
  });

  return button;
}

void MapEditor::handleNewText(sf::Uint32 newChar) {
  if (newChar == 8) {
    // Backspace
    mSavePopupInput->removeChar();
  }
  else if (mSavePopupInput->getText().length() < 15) {
    mSavePopupInput->addChar(newChar);
  }
}
