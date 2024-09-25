#pragma once
#include "SceneBase.hpp"
#include "GameUtils.hpp"

class GameScene : public SceneBase {
private:
	/*MAKE MACROS FOR INPUTS*/
	friend class PlayerDrivingSystem;
	friend class PlayerShootingSystem;
	
	inline static InputAction QUIT_GAME = { KEY_ESCAPE, GAMEPAD_BUTTON_MIDDLE_RIGHT};
	inline static InputAction MOVE_RIGHT = { KEY_D,KEY_RIGHT,GAMEPAD_LEFT_AXIS_POS_X };
	inline static InputAction MOVE_LEFT = { KEY_A,KEY_LEFT, GAMEPAD_LEFT_AXIS_NEG_X };
	inline static InputAction MOVE_UP = { KEY_W,KEY_UP,GAMEPAD_BUTTON_RIGHT_TRIGGER_2 };
	inline static InputAction MOVE_DOWN = { KEY_S,KEY_DOWN,GAMEPAD_BUTTON_LEFT_TRIGGER_2 };
	inline static InputAction SHOOT = { KEY_SPACE, GAMEPAD_BUTTON_RIGHT_FACE_DOWN };
	inline static InputAction SWITCH_GUN = { KEY_LEFT_SHIFT, GAMEPAD_BUTTON_RIGHT_FACE_RIGHT };
	static Entity createPlayer();
public:
	static inline Vec2 dimension;
	static inline int tileSize = 96;
	static inline int seed = 10;

	static inline TileInfo onCurrentTile = { UP, GRASS, {GRASS,GRASS,GRASS,GRASS} };

	GameScene() : SceneBase
	(
		std::make_shared<CleanCollisionSystem>(),
		std::make_shared<CollectCollisionSystem>(),
		std::make_shared<PlayerMovementSystem>(),
		std::make_shared<CameraFollowPlayerSystem>(),
		std::make_shared<PlayerDrivingSystem>(),
		std::make_shared<PlayerShootingSystem>(),
		std::make_shared<BulletMovementSystem>(),
		std::make_shared<PowerUpMovementSystem>(),
		std::make_shared<SpawnDogSystem>(),
		std::make_shared<StunSystem>(),
		std::make_shared<ChaseSystem>(),
		std::make_shared<PowerUpCollectionSystem>(),
		std::make_shared<PowerUpTimerSystem>(),
		std::make_shared<PlayerUpdateGuiSystem>(),
		std::make_shared<UpdateHouseGuiSystem>(),
		std::make_shared<SyncBodiesSystem>()
	) {};
	void init() override;
	void drawDebug() override;

	static Entity createDog(Vec2 position, DogType type);
	static Entity createPowerUp(Vec2 position);

};