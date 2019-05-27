#pragma once

#include "../Resources/MenuBase.h"
#include "../Resources/GuiExpandableList.h"
#include "../../../Common/MessageHandler/MessageHandler.h"

class DebugMenu : public MenuBase {
public:
  DebugMenu();
  ~DebugMenu();

  bool render(std::shared_ptr<sf::RenderWindow> window, sf::Vector2f mousePosition) override;

private:
  Interface mIf;
  std::shared_ptr<GuiList> mGuiList;
  std::map<std::string, std::shared_ptr<GuiExpandableList>> mCategoriesMap;
  std::map<int, std::shared_ptr<GuiComponent>> mSubscirberToComponentMap;
};

