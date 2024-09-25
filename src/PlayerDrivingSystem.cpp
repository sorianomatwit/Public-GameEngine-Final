#include "GameSystems.hpp"
#include "GameScene.hpp";
#include <algorithm>

void PlayerDrivingSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	for (auto id : entites)
	{
		auto entity = Entity(id);
		auto& inputData = entity.getComponent<CInput>();
		auto& renderData = entity.getComponent<CAnimation>();
		auto& rbData = entity.getComponent<CRigidBody>();
		auto& carData = entity.getComponent<CCarController>();
		auto& player = entity.getComponent<CPlayer>();
		float steeringInput = inputData.isInputPress(GameScene::MOVE_RIGHT) - inputData.isInputPress(GameScene::MOVE_LEFT);
		float accelerationInput = (inputData.isInputPress(GameScene::MOVE_DOWN) * 0.75) - (inputData.isInputPress(GameScene::MOVE_UP));

		if (inputData.isInputPressed(GameScene::QUIT_GAME)) {
			GameEngine::quit();
		}
		float modifier = 1;
		if (player.powerUps[SpeedBoost].isActive) {
			//set animation to speeedy car
			renderData.animation = Animation(GameEngine::getAssets().getAnimation("Fast_car"),Vec2(.9f,.9f));
			modifier = 1.75f;
		} else renderData.animation = GameEngine::getAssets().getAnimation("Player");

		Color behindMe = WHITE;
		//TODO add colliision check to change drag
		carData.dragC = carData.baseDragC;
		if (enableGrassDrag ) {
			if (entity.hasComponent<CCollisionContainer>()) {
				for (auto& c : entity.getComponent<CCollisionContainer>().enterCollisions)
				{
					if (c.otherEntity.hasComponent<CTile>()) {
						auto& cTile = c.otherEntity.getComponent<CTile>();
						if (cTile.tile.info.type == GRASS || cTile.tile.info.type == FENCE || cTile.tile.info.type == House_2 || cTile.tile.info.type == House_3) {
							carData.dragC = carData.baseDragC * 1.5;
							behindMe = BROWN;
						}
					}
				}
			}
		}

		float canTurn = (floorf(rbData.velocity.length()) > 0) || (inputData.isInputPress(GameScene::MOVE_DOWN) && inputData.isInputPress(GameScene::MOVE_UP));
		rbData.angle += carData.turnSpeed * modifier * deltatime * steeringInput * canTurn;

		//This retains the angle to be between -360 and 360;
		rbData.angle = std::fmod(rbData.angle, 360);

		float rAngle = rbData.angle * (PI / 180.0f);

		Vec2 direction = Vec2(cosf(rAngle), sinf(rAngle));

		Vec2 drivingForce = direction * accelerationInput * carData.carAcceleration * modifier * deltatime;

		if (accelerationInput && !entity.hasComponent<CParticleEmitter>()) {
			float particleAngle = rbData.angle + 90;
			/*if (direction.x > 0) {
				particleAngle *= -1;
				particleAngle -= 180;
			}*/
			entity.addComponent<CParticleEmitter>(
				CParticleEmitter(
					Animation(GameEngine::getAssets().getAnimation("Circle"), Vec2(1,1)),
					30,
					rbData.position,
					{ std::max(0.0f,rbData.velocity.length() - 3),rbData.velocity.length() },
					{ particleAngle - 45,particleAngle + 45 },
					{ 0.1f, 0.5 },
					behindMe
				));
		}
		else {

		}
		rbData.acceleration = drivingForce;


	}
}