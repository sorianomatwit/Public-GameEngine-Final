#include "GameSystems.hpp"
#include "GameOverScene.hpp"

void GameOverSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (auto id: entities)
	{
        auto entity = Entity(id);
        auto& inputData = entity.getComponent<CInput>();
        auto& menuOptions = entity.getComponent<CMenuOption>().menuOptions;

        // Reset previous menu item color
        for (auto& item : menuOptions[menuIndex].getComponent<CGui>().guiItems) {
            if (auto* textGui = std::get_if<Text>(&item)) {
                auto& textData = *textGui;
                textData.color = GRAY;
            }
        }

        if (inputData.isInputPressed(GameOverScene::MOVE_DOWN)) {
            menuIndex++;
            if (menuIndex >= menuOptions.size()) menuIndex = 0;

            auto moveSound = GameEngine::getAssets().getSound("menu_select");
            PlaySound(moveSound);
        }

        if (inputData.isInputPressed(GameOverScene::MOVE_UP)) {
            menuIndex--;
            if (menuIndex < 0) menuIndex = menuOptions.size() - 1;

            auto moveSound = GameEngine::getAssets().getSound("menu_select");
            PlaySound(moveSound);
        }

        for (auto& item : menuOptions[menuIndex].getComponent<CGui>().guiItems) {
            if (auto* textGui = std::get_if<Text>(&item)) {
                auto& textData = *textGui;
                textData.color = WHITE;
            }
        }


        if (inputData.isInputPressed(GameOverScene::SELECT)) {

            auto selectSound = GameEngine::getAssets().getSound("menu_select");
            PlaySound(selectSound);

            switch (menuIndex) {
            case 0: // start game
                GameEngine::changeScene(GAME);
                break;
            case 1: // quit game
                GameEngine::quit();
                break;
            default:
                break;
            }
        }
	}
}
