#pragma once
#include "SceneBase.hpp"
#include "OpeningSceneSystem.hpp"

class OpeningScene : public SceneBase
{
public:
    inline static int loadingCount = 0;
    inline static int loadingTotal = 0;
    inline static std::vector<Entity> sceneEntities;

    OpeningScene() : SceneBase
    (
        std::make_shared<OpeningSceneSystem>()
    ) {};
    void init() override;
    void drawDebug() override;
    void clear() {
        //Features[0]->isActive = false;
        for (auto e : sceneEntities)
        {
            e.destroy();
        }
        sceneEntities.clear();
    }
};
