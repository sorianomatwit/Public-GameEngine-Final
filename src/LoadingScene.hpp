#pragma once
#include "SceneBase.hpp"
#include "LoadTrackerSystem.hpp"

class LoadingScene : public SceneBase
{
public:
    inline static int loadingCount = 0;
    inline static int loadingTotal = 0;
    inline static std::vector<Entity> sceneEntities;
    
    LoadingScene() : SceneBase
    (
        std::make_shared<LoadTrackerSystem>()
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

