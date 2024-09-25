#include "GameSystems.hpp"
#include "GameEngine.hpp"

class GameEngine;

void PlayerUpdateGuiSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	Vec2 topLeftCameraPos = Vec2(GameEngine::mainCamera.target.x - GameEngine::windowWidth * 0.5f, GameEngine::mainCamera.target.y - GameEngine::windowHeight * 0.5f);

	for (auto id : entities)
	{
		auto entity = Entity(id);
		auto playerData = entity.getComponent<CPlayer>();
		auto& guiData = entity.getComponent<CGui>();

		int k = 0;
		for (int i = 0; i < guiData.guiItems.size();i++)
		{
			if (i <= playerData.ShieldIconOverlay) {
				auto allPowerUps = CPowerUp().allPowerUps;
				auto& displayData = std::get<Display>(guiData.guiItems[i]);

				k = (i % 3 == 0 && i != 0) ? k + 1 : k;
				displayData.origin = topLeftCameraPos;
				float xMargin = GameEngine::windowWidth - displayData.animation.size.x * 0.5f - 20.0f;
				float yMargin = GameEngine::windowHeight * (0.35f) - displayData.animation.size.y * 0.5f + k * (displayData.animation.size.y * 0.7f + 10);
				displayData.offset = Vec2(xMargin, yMargin);

				float timerPercent, fullHeight, newHeight;
				switch (i) {
				case playerData.AmmoIcon:
					displayData.animation = (playerData.currentAmmo == MAIL) ?
						Animation(GameEngine::getAssets().getAnimation("Mail_Icon"), Vec2(0.85f, 0.85f)) :
						Animation(GameEngine::getAssets().getAnimation("Dog_treat_icon"), Vec2(0.85f, 0.85f));
					break;
				case playerData.TripleIcon:
					displayData.animation = (playerData.currentAmmo == MAIL) ?
						Animation(GameEngine::getAssets().getAnimation("Triple_M_cannon"), Vec2(0.85f, 0.85f)) :
						Animation(GameEngine::getAssets().getAnimation("Triple_T_cannon"), Vec2(0.85f, 0.85f));
					break;
				case playerData.AmmoIconOverlay:
				case playerData.TripleIconOverlay:
				case playerData.SpeedIconOverlay:
				case playerData.ShieldIconOverlay:
					switch (i) {
					case playerData.AmmoIconOverlay:
						timerPercent = std::max(playerData.shootCoolDownTimer[playerData.currentAmmo] / playerData.shootCoolDownAmount[playerData.currentAmmo], 0.0f);
						break;
					case playerData.TripleIconOverlay:
						timerPercent = std::max(1 - (playerData.powerUps[TripleShot].duration / allPowerUps[TripleShot].duration), 0.0f);
						break;
					case playerData.SpeedIconOverlay:
						timerPercent = std::max(1 - playerData.powerUps[SpeedBoost].duration / allPowerUps[SpeedBoost].duration, 0.0f);
						break;
					case playerData.ShieldIconOverlay:
						timerPercent = std::max(1 - playerData.powerUps[Shield].duration / allPowerUps[Shield].duration, 0.0f);
						break;

					}


					fullHeight = displayData.animation.size.y;
					newHeight = fullHeight * (timerPercent);
					displayData.animation.frameRect.y = (fullHeight - newHeight);
					displayData.animation.frameRect.height = newHeight;
					displayData.animation.scaledSize.y = newHeight * displayData.animation.scale.y;
					displayData.offset.y = yMargin + (fullHeight - newHeight) * displayData.animation.scale.y;
					break;
				}
			}
			else if (i <= playerData.HousesLeftBG) {
				auto& displayData = std::get<Display>(guiData.guiItems[i]);

				displayData.origin = topLeftCameraPos;
				float xMargin = 30.0f + displayData.animation.size.x;
				float yMargin = 30.0f;
				displayData.offset = Vec2(xMargin, yMargin);
				displayData.middleWidth = GameEngine::windowWidth - displayData.edgeSize * 2 - 60.0f;
			}
			else if (i == playerData.HousesLeftProgress) {
				auto& BGdisplayData = std::get<Display>(guiData.guiItems[playerData.HousesLeftBG]);

				auto& displayData = std::get<Display>(guiData.guiItems[i]);
				displayData.offset = BGdisplayData.offset;
				displayData.origin = BGdisplayData.origin;
				displayData.middleWidth = (playerData.housesCompleted / (ComponentSet<CHouse>::count / 4)) * BGdisplayData.middleWidth;
			}
			else if (i == playerData.ClosestHouse) {
				auto& displayData = std::get<Display>(guiData.guiItems[i]);

				displayData.origin = entity.getComponent<CRigidBody>().position;
				Vec2 dir(0, 0);
				float lowestDist = 0;
				for (auto houseIds : ComponentSet<CHouse>::entities)
				{
					auto house = Entity(houseIds).getComponent<CHouse>().house;;
					auto dist = displayData.origin.dist(house->position);
					if (house->isComplete) continue;
					if (lowestDist <= 0 || dist < lowestDist) {
						dir = (displayData.origin - house->position).normalize();
						lowestDist = dist;
					}
				}
				displayData.angle = dir.angleBetween(Vec2::UP) * (180.0f / PI);
				if (dir.x > 0) {
					displayData.angle *= -1;
				}
				displayData.offset = dir * -70.0f;
			}
			else if (i == playerData.HpBarGray) {
				auto& displayData = std::get<Display>(guiData.guiItems[i]);

				displayData.origin = topLeftCameraPos;
				float xMargin = 30.0f + displayData.animation.size.x;
				float yMargin = 130.0f;
				displayData.offset = Vec2(xMargin, yMargin);
				displayData.middleWidth = 150 - displayData.edgeSize * 2 - 60.0f;

			}
			else if (i == playerData.HpBarWhite) {
				auto& BGdisplayData = std::get<Display>(guiData.guiItems[playerData.HpBarGray]);

				auto& displayData = std::get<Display>(guiData.guiItems[i]);
				displayData.offset = BGdisplayData.offset;
				displayData.origin = BGdisplayData.origin;
				auto& health = entity.getComponent<CHealth>();
				displayData.middleWidth = (health.health / (float) health.maxHealth) * BGdisplayData.middleWidth;

			}
		}

	}
}

void UpdateHouseGuiSystem::Execute(std::vector<uint16_t> entities, float deltaTime) {
	for (auto& id : entities)
	{
		auto entity = Entity(id);
		if (entity.getComponent<CTile>().tile.info.type != House_0) continue;

		auto& guiData = entity.getComponent<CGui>();

		auto& grayBarDisplay = std::get<Display>(guiData.guiItems[0]);
		auto& whiteBarDisplay = std::get<Display>(guiData.guiItems[1]);

		grayBarDisplay.origin = entity.getComponent<CHouse>().house->position;
		//96 is the tile size 20 is a buffer
		grayBarDisplay.offset = Vec2(0, -96 + 20);
		grayBarDisplay.middleWidth = 96 * 2 - grayBarDisplay.edgeSize * 2;

		auto house = entity.getComponent<CHouse>().house;
		whiteBarDisplay.origin = grayBarDisplay.origin;
		whiteBarDisplay.offset = grayBarDisplay.offset;
		whiteBarDisplay.middleWidth = grayBarDisplay.middleWidth * (1 - ((float)house->mailAmount / (float)house->totalMail));
	}
}