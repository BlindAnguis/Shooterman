#include "MapEditor.h"

#include <iostream>
#include <fstream>
#include <filesystem>

#include <SFML/Window/Mouse.hpp>

#include "../Resources/GuiList.h"
#include "../Resources/GuiExpandableList.h"
#include "../Resources/GuiComponentFactory.h"

MapEditor::MapEditor() {
  mName = "CLIENT: MAP_EDITOR";
  mMap.clear();
  mMap.createDefaultMap();
  mGuiFrame = std::make_shared<Frame>();
  mSavePopupBackground = std::make_shared<GuiFrameComponent>(GuiComponentPosition::CENTER, sf::RectangleShape(sf::Vector2f(520, 520)));
  auto toolbar = std::make_shared<GuiList>(GuiComponentPosition::RIGHT, GuiListDirection::VERTICAL);

  auto filesList1 = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  filesList1->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "New", std::bind(&MapEditor::onNewClicked, this)));
  filesList1->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Load", std::bind(&MapEditor::onShowLoadClicked, this)));
  toolbar->addGuiComponent(filesList1);

  auto filesList2 = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  filesList2->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Save", std::bind(&MapEditor::onShowSaveClicked, this)));
  filesList2->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Button", [=]() {TRACE_INFO("Why are you pressing this button?");}));
  toolbar->addGuiComponent(filesList2);

  auto floorList = std::make_shared<GuiExpandableList>(GuiComponentPosition::CENTER, "Floors");
  floorList->addGuiComponent(addButton(Textures::SpawnPoint, "Spawn Point"));
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
  mSavePopupFrame = std::make_shared<Frame>();
  mSavePopupFrame->addGuiComponent(mSavePopupBackground);
  auto saveHeadline = std::make_shared<GuiText>(GuiComponentPosition::TOP, "Save map");
  mSavePopupBackground->addGuiComponent(saveHeadline);
  auto filesList3 = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::HORIZONTAL);
  mSavePopupInput = std::make_shared<GuiInputText>(GuiComponentPosition::CENTER, "Filename");
  mSavePopupInput->enableReceiveInput();
  addTextListener(mSavePopupInput);
  filesList3->addGuiComponent(mSavePopupInput);
  filesList3->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, "Save", std::bind(&MapEditor::onSaveClicked, this)));
  mSavePopupBackground->addGuiComponent(filesList3);

  // Create load popup
  mLoadPopupFrame = std::make_shared<Frame>();
  mLoadPopupBackground = std::make_shared<GuiFrameComponent>(GuiComponentPosition::CENTER, sf::RectangleShape(sf::Vector2f(520, 520)));
  mLoadPopupFrame->addGuiComponent(mLoadPopupBackground);

  setupDebugMessages("Client", "Map Editor");
}

MapEditor::~MapEditor() { }

void MapEditor::backgroundUpdate() {
  handleDebugMessages();
}

bool MapEditor::checkMouse(sf::Vector2f mousePosition) {
  if (mRenderSavePopup) {
    if (mSavePopupBackground->isMouseClickInsideFrame(mousePosition)) {
      if (mSavePopupFrame->checkMouse(mousePosition)) {
        return true;
      }
    } else {
      mRenderSavePopup = false;
    }
    return false;
  }

  if (mRenderLoadPopup) {
    if (mLoadPopupBackground->isMouseClickInsideFrame(mousePosition)) {
      if (mLoadPopupFrame->checkMouse(mousePosition)) {
        return true;
      }
    } else {
      mRenderLoadPopup = false;
    }
    return false;
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
    mSavePopupFrame->render(window);
  }
  if (mRenderLoadPopup) {
    mLoadPopupFrame->render(window);
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

std::vector<std::string> MapEditor::getFilepaths() {
  std::vector<std::string> levelDirectories;
  levelDirectories.emplace_back("Levels/UserCreated/");
  levelDirectories.emplace_back("Levels/DefaultLevels/");

  std::vector<std::string> filepaths;

  for (auto levelDirectory : levelDirectories) {
    for (const auto& entry : std::filesystem::directory_iterator(levelDirectory)) {
      if (entry.path().string().find(".level") != std::string::npos) {
        std::string pathAsString = entry.path().string();
        int pos = pathAsString.find_last_of("/") + 1;
        auto fileName = pathAsString.substr(pos, pathAsString.length() - pos);
        pos = fileName.find_last_of(".");
        fileName = fileName.substr(0, pos);
        filepaths.emplace_back(pathAsString);
      }
    }
  }
  return filepaths;
}

void MapEditor::onNewClicked() {
  mMap.clear();
  mMap.createDefaultMap();
}

void MapEditor::onShowLoadClicked() {
  mLoadPopupBackground->clear();

  auto loadHeadline = std::make_shared<GuiText>(GuiComponentPosition::TOP, "Load map");
  mLoadPopupBackground->addGuiComponent(loadHeadline);
  auto filesList = std::make_shared<GuiList>(GuiComponentPosition::CENTER, GuiListDirection::VERTICAL);
  mLoadPopupBackground->addGuiComponent(filesList);

  for (auto filepath : getFilepaths()) {
    int pos = filepath.find_last_of("/") + 1;
    auto fileName = filepath.substr(pos, filepath.length() - pos);
    pos = fileName.find_last_of(".");
    fileName = fileName.substr(0, pos);
    filesList->addGuiComponent(std::make_shared<GuiButton>(GuiComponentPosition::CENTER, fileName, std::bind(&MapEditor::onLoadClicked, this, filepath)));
  }
  mRenderLoadPopup = true;
}

void MapEditor::onLoadClicked(std::string filepath) {
  std::ifstream saveFile;
  saveFile.open(filepath);
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
  mRenderLoadPopup = false;
}

void MapEditor::onShowSaveClicked() {
  // Open new popup
  mRenderSavePopup = true;
}

void MapEditor::onSaveClicked() {
  std::ofstream saveFile;
  std::string mapPath("Levels/UserCreated/" + mSavePopupInput->getText() + ".level");
  saveFile.open(mapPath);
  if (saveFile.is_open()) {
    saveFile << mMap.toString();
  } else {
    TRACE_ERROR("Failed to open file: " << mapPath);
  }
  saveFile.close();
  mRenderSavePopup = false;
}
