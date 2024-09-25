#include "GameSystems.hpp"
#include <imgui/imgui.h>
#include <imgui/rlImGui.h>
#include <imgui/imgui_stdlib.h>
#include "GameScene.hpp"

Entity createBulletEntity(BulletType type, Vec2 startingPosition, Vec2 direction, float bulletSpeed) {
	Vec2 scale(0.55f, 0.55f);
	auto animation = (type == MAIL)
		? Animation(GameEngine::getAssets().getAnimation("Mail_Icon"), scale)
		: Animation(GameEngine::getAssets().getAnimation("Dog_treat_icon"), scale);

	Vec2 vel = direction * bulletSpeed;

	std::string shootSound = (type == MAIL) ? "shoot_mail" : "shoot_treat";

	
	auto b = EntityManager::createEntity()
		.addComponent<CAnimation>(CAnimation(animation, false, Vector2(startingPosition.x, startingPosition.y), 0, 10))
		.addComponent<CRigidBody>(CRigidBody(Vec2(startingPosition), vel, 0, { {12.0f, Vec2(0,0)} }))
		.addComponent<TBullet>(TBullet(type));
		EntityManager::createEntity()
		.addComponent<CAudio>(CAudio(GameEngine::getAssets().getSound(shootSound), false))
		.setActive(true);

	b.setActive(true);
	return b;
}

void PlayerShootingSystem::Execute(std::vector<uint16_t> entites, float deltatime) {
	for (auto id : entites)
	{
		auto player = Entity(id);
		auto& inputData = player.getComponent<CInput>();
		auto& rbData = player.getComponent<CRigidBody>();
		auto& playerData = player.getComponent<CPlayer>();
		//TODO make it the same direction as driving
		playerData.shootCoolDownTimer[MAIL] -= (playerData.shootCoolDownTimer[MAIL] <= 0) ? 0 : deltatime;
		playerData.shootCoolDownTimer[TREAT] -= (playerData.shootCoolDownTimer[TREAT] <= 0) ? 0 : deltatime;

		bool canShoot = playerData.shootCoolDownTimer[playerData.currentAmmo] <= 0;

		float rads = rbData.angle * PI / 180.0f;
		Vec2 direction = Vec2(cosf(rads), sinf(rads)).normalize() * -1;
		if (inputData.isInputReleased(GameScene::SHOOT) && canShoot) {
			//shoot bullet and rreset timer
			createBulletEntity(playerData.currentAmmo, rbData.position, direction, playerData.bulletSpeed);

			if (playerData.powerUps[TripleShot].isActive) {
				auto rightDir = direction.rotateRad(-45 * (PI / 180));
				auto leftDir = direction.rotateRad(45 * (PI / 180));
				createBulletEntity(playerData.currentAmmo, rbData.position, rightDir, playerData.bulletSpeed);
				createBulletEntity(playerData.currentAmmo, rbData.position, leftDir, playerData.bulletSpeed);
			}
			playerData.shootCoolDownTimer[playerData.currentAmmo] = playerData.shootCoolDownAmount[playerData.currentAmmo];

		}
		//Switch ammo
		if (inputData.isInputPressed(GameScene::SWITCH_GUN)) {
			// bullet can either be MAIL (0) or TREAT(1) by using the ! operator we are able to 
			// treat the enum as a bool, a bool can also be read as a int so we get that int by the enum type in order to secure type safety
			playerData.currentAmmo = static_cast<BulletType>(!playerData.currentAmmo);
		}
	}
}

