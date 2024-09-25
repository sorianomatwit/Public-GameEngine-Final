
#pragma once
#include "GameScene.hpp"
#include <math.h>
#include <chrono>
#include "Utils.hpp";
#include "LoadingScene.hpp"

void createHouseUi(Entity house) {
	Vec2 origin = house.getComponent<CRigidBody>().position;
	Display ProgressBarBg = {
		Animation(GameEngine::getAssets().getAnimation("Gray_bar"), Vec2(2,2)),
		false,
		DARKGREEN,
		origin,
		true,
		10.0f,
		0
	};
	Display ProgressBar = {
		Animation(GameEngine::getAssets().getAnimation("White_bar"), Vec2(2,2)),
		false,
		GREEN,
		origin,
		true,
		10.0f,
		0
	};

	house.addComponent<CGui>(CGui(ProgressBarBg, ProgressBar));
}

Entity createTileEntity(TileNode t, Vec2 tileSize, Vec2 pos, Vec2 origin, std::map<int, std::shared_ptr<House>> houseMap) {

	auto b = EntityManager::createEntity();
	auto& cTile = b.addComponent<CTile>(CTile(t)).getComponent<CTile>();
	Texture2D texture;
	float angle = getAngle(t.info.direction);
	float colliderRadius = tileSize.x * 0.55f;
	Vec2 offset = Vec2(0, 0);
	float dx;
	float dy;
	Vec2 posit(origin.x + tileSize.x * 0.5f + pos.x * tileSize.x,
		origin.y + tileSize.y * 0.5f + pos.y * tileSize.y);
	std::vector<Collider> colliders;
	switch (t.info.type)
	{
	case ROAD:
		//texture = createTileTexture(tileSize.x, tileSize.y, GRAY, { t.info.direction,t.info.type,t.info.edges }, angle);
		texture = GameEngine::getAssets().getAnimation("Road").getTexture();
		break;
	case GRASS:
		//texture = createTileTexture(tileSize.x, tileSize.y, GREEN, { t.info.direction,t.info.type,t.info.edges }, angle);
		texture = GameEngine::getAssets().getAnimation("Grass").getTexture();
		break;
	case CURVE_ROAD:
		//texture = createTileTexture(tileSize.x, tileSize.y, DARKGRAY, { t.info.direction,t.info.type,t.info.edges }, angle);
		texture = GameEngine::getAssets().getAnimation("Curved_road").getTexture();
		break;
	case INTERSECTION:
		texture = GameEngine::getAssets().getAnimation("Intersection").getTexture();

		break;
	case House_0:
		offset = Vec2(1, 1) * colliderRadius * 0.5f;
		texture = GameEngine::getAssets().getAnimation("House_TL").getTexture();
		b.addComponent<CHouse>(CHouse(houseMap[getIndex(pos)]));
		houseMap[getIndex(pos)]->position = posit + (tileSize * 0.5f);
		houseMap[getIndex(pos)]->tiles.push_back(b);
		break;
	case House_1:
		offset = Vec2(-1, 1) * colliderRadius * 0.5f;
		texture = GameEngine::getAssets().getAnimation("House_TR").getTexture();
		b.addComponent<CHouse>(CHouse(houseMap[getIndex(pos)]));
		houseMap[getIndex(pos)]->tiles.push_back(b);

		break;
	case House_2:
		texture = GameEngine::getAssets().getAnimation("House_BL").getTexture();
		offset = Vec2(1, -0.75f) * colliderRadius * 0.25f;
		b.addComponent<CHouse>(CHouse(houseMap[getIndex(pos)]));
		houseMap[getIndex(pos)]->tiles.push_back(b);

		break;
	case House_3:
		texture = GameEngine::getAssets().getAnimation("House_BR").getTexture();
		offset = Vec2(-1, -0.75f) * colliderRadius * 0.25f;
		b.addComponent<CHouse>(CHouse(houseMap[getIndex(pos)]));
		houseMap[getIndex(pos)]->tiles.push_back(b);

		break;
	case ROAD_BLOCK:
		colliderRadius = tileSize.x * 0.35f;
		dx = (t.info.direction == RIGHT) - (t.info.direction == LEFT);
		dy = (t.info.direction == DOWN) - (t.info.direction == UP);
		offset = Vec2(dx, dy) * colliderRadius;
		texture = GameEngine::getAssets().getAnimation("Road_block").getTexture();
		if (dx == 0) {
			colliders.push_back({ colliderRadius,  offset + Vec2(colliderRadius,0) });
			colliders.push_back({ colliderRadius,  offset + Vec2(-colliderRadius,0) });
		}
		else {
			colliders.push_back({ colliderRadius,  offset + Vec2(0,colliderRadius) });
			colliders.push_back({ colliderRadius,  offset + Vec2(0,-colliderRadius) });
		}
		break;
	case FENCE:
		texture = GameEngine::getAssets().getAnimation("Fence").getTexture();
		break;
	case FENCE_CORNER:
		texture = GameEngine::getAssets().getAnimation("Fence_corner").getTexture();
		break;
	case DOG_HOUSE:
		texture = GameEngine::getAssets().getAnimation("Dog_house").getTexture();
		b.addComponent<CDogHouse>(CDogHouse(5.0f, CHASER));
		break;
	default:
		texture = GameEngine::getAssets().getAnimation("Placeholder").getTexture();
		break;
	}

	colliders.push_back({ colliderRadius,  offset });
	b.addComponent<CAnimation>(CAnimation(
		Animation(texture, Vec2(1.0f, 1.0f)), false,
		Vector2(posit.x, posit.y), angle))
		.addComponent<CRigidBody>(CRigidBody(posit, Vec2(0, 0), angle, colliders));
	switch (t.info.type)
	{
	case House_0:
		createHouseUi(b);
		break;
	}
	return b;
}

void createPlayerUI(Entity player) {
	Vec2 topLeft = Vec2(GameEngine::mainCamera.target.x - GameEngine::windowWidth * 0.5f, GameEngine::mainCamera.target.y - GameEngine::windowHeight * 0.5f);

#pragma region Side Panel
	Animation Iconbg = GameEngine::getAssets().getAnimation("IconBG");
	Animation Iconoverlay = GameEngine::getAssets().getAnimation("IconOverlay");
	Vec2 scale(0.85f, 0.85f);
	Display IconBG = {
		Animation(Iconbg, scale),
		false,
		WHITE,
		topLeft
	};
	Display IconOverlay = {
		Animation(Iconbg,scale),
		false,
		ColorFromNormalized({0.0f,0.0f,0.0f,0.65f}),
		topLeft
	};

	auto ammoBg = IconBG;
	auto ammoover = IconOverlay;
	//Ammo Display
	Display ammoIcon = {
		GameEngine::getAssets().getAnimation("Mail_Icon"),
		false,
		WHITE,
		topLeft,
	};

	scale = Vec2(0.65f, 0.65f);
	IconBG.animation = Animation(Iconbg, scale);
	IconOverlay.animation = Animation(Iconoverlay, scale);
	//Power Up
	//triple Shot
	Display powerupTIcon = {
		Animation(GameEngine::getAssets().getAnimation("Triple_M_cannon"), Vec2(1.2,1.2)),
		false,
		WHITE,
		topLeft,
	};
	powerupTIcon.angle = 270;
	//speed boost
	Display powerupSIcon = {
		Animation(GameEngine::getAssets().getAnimation("Speed_boost"),scale),
		false,
		WHITE,
		topLeft,
	};

	Display powerupShieldIcon = {
		Animation(GameEngine::getAssets().getAnimation("Shield"),scale),
		false,
		WHITE,
		topLeft,
	};

#pragma endregion

#pragma region Score
	Display ProgressBarBg = {
		Animation(GameEngine::getAssets().getAnimation("Gray_bar"), Vec2(2,2)),
		false,
		WHITE,
		topLeft,
		true,
		10.0f,
		0
	};
	Display ProgressBar = {
		Animation(GameEngine::getAssets().getAnimation("White_bar"), Vec2(2,2)),
		false,
		WHITE,
		topLeft,
		true,
		10.0f,
		0
	};
	Text housesLeftStatic = {
		"Route Progress",
		GameEngine::getAssets().getFont("PlayMegaGames"),
		WHITE,
		32.0f
	};
	Display HealthBarBg = {
		Animation(GameEngine::getAssets().getAnimation("Gray_bar"), Vec2(2,2)),
		false,
		RED,
		topLeft,
		true,
		10.0f,
		0
	};
	Display HealthBar = {
		Animation(GameEngine::getAssets().getAnimation("White_bar"), Vec2(2,2)),
		false,
		RED,
		topLeft,
		true,
		10.0f,
		0
	};
#pragma endregion

	Display indicator = {
		GameEngine::getAssets().getAnimation("Indicator"),
		false,
		RED,
		topLeft
	};
	player.addComponent<CGui>(CGui(
		ammoBg,
		ammoIcon,
		ammoover,

		IconBG,
		powerupTIcon,
		IconOverlay,

		IconBG,
		powerupSIcon,
		IconOverlay,

		IconBG,
		powerupShieldIcon,
		IconOverlay,

		ProgressBarBg,
		ProgressBar,
		indicator,
		HealthBarBg,
		HealthBar
	));
}

Entity GameScene::createPlayer() {
	auto spawnLoc = Vec2(GameEngine::worldWidth * 0.5f, GameEngine::worldHeight * 0.5f);
	auto texture = GameEngine::getAssets().getAnimation("Player");
	auto playrRb = CRigidBody(spawnLoc, Vec2(0.0f, 0.0f), 0, { {15, Vec2(0, 0)} });

	auto e = EntityManager::createEntity()
		.addComponent<CInput>(CInput(
			GameScene::MOVE_RIGHT,
			GameScene::MOVE_UP,
			GameScene::MOVE_DOWN,
			GameScene::MOVE_LEFT,
			GameScene::SWITCH_GUN,
			GameScene::SHOOT,
			GameScene::QUIT_GAME
		))
		.addComponent<CAnimation>(CAnimation(texture, false, Vector2(spawnLoc.x, spawnLoc.y), -1))
		.addComponent<CRigidBody>(playrRb)
		.addComponent<CPlayer>(CPlayer())
		.addComponent<CHealth>(CHealth(5, 5))
		.addComponent<CCarController>(CCarController(30.0f, 200.0f, 0.1f));
	GameEngine::mainCamera.target = Vector2(spawnLoc.x, spawnLoc.y);
	e.getComponent<CPlayer>().canon = EntityManager::createEntity()
		.addComponent<CRigidBody>(CRigidBody(playrRb))
		.addComponent<CAnimation>(CAnimation(GameEngine::getAssets().getAnimation("Mail_cannon"), false));
	createPlayerUI(e);
	return e;
}

Entity GameScene::createDog(Vec2 position, DogType type) {
	Texture2D texture;
	Animation animation;


	auto e = EntityManager::createEntity()
		.addComponent<CRigidBody>(CRigidBody(position, Vec2(0.0f, 0.0f), 0.0f, { {15.0f, Vec2(0.0f, 0.0f)} }))
		.addComponent<TDog>(TDog());

	switch (type) {
	case CHASER:
		e.addComponent<CChaser>(CChaser());
		e.addComponent<CHealth>(CHealth(1, 1));
		EntityManager::createEntity()
			.addComponent<CAudio>(CAudio(GameEngine::getAssets().getSound("chaser_bark1"), true))
			.setActive(true);
		
		animation = GameEngine::getAssets().getAnimation("Chaser_walk_backward");
		break;

	case BARKER:
		e.addComponent<CBarker>(CBarker());
		e.addComponent<CHealth>(CHealth(1, 1));
		EntityManager::createEntity()
			.addComponent<CAudio>(CAudio(GameEngine::getAssets().getSound("barker_bark1"), true))
			.setActive(true);

		animation = GameEngine::getAssets().getAnimation("Barker_walk_backward");
		break;

	default:
		break;
	}

	e.addComponent<CAnimation>(CAnimation(animation, true, Vector2(position.x, position.y), 0.0f,3));
	e.setActive(true);
	
	return e;
}


Entity GameScene::createPowerUp(Vec2 position) {
	Animation animation;

	auto e = EntityManager::createEntity()
		.addComponent<CPowerUp>(CPowerUp());
	auto type = e.getComponent<CPowerUp>().powerUp.type;
	switch (type) {
	case Shield:
		animation = GameEngine::getAssets().getAnimation("Shield");
		break;
	case SpeedBoost:
		animation = GameEngine::getAssets().getAnimation("Speed_boost");
		break;
	case ExtraHealth:
		animation = GameEngine::getAssets().getAnimation("Heart");
		break;
	case TripleShot:
		animation = GameEngine::getAssets().getAnimation("Triple_cannon");
		break;
	case NONE:
		e.destroy();
		return Entity(UINT16_MAX);
	default:
		animation = GameEngine::getAssets().getAnimation("Placeholder");
		break;
	}

	e
		.addComponent<CAnimation>(CAnimation(Animation(animation, Vec2(0.85f, 0.85f)), false, Vector2(position.x, position.y), 0.0f, 10))
		.addComponent<CRigidBody>(CRigidBody(position, Vec2(0.0f, 0.0f), 0.0f, { {15.0f, Vec2(0, 0)} }));
//	e.getComponent<CRigidBody>().acceleration = dir * 50.0f;
	e.setActive(true);

	return e;
}

void GameScene::init() {
	std::vector<TileNode> gameTiles;
	srand(time(nullptr));
	auto& width = GameEngine::worldWidth;
	auto& height = GameEngine::worldHeight;
	int dx = width / tileSize;
	int dy = height / tileSize;
	dimension = Vec2(dx, dy);

	//init all nodes insert maxEntropy
	LoadingScene::loadingTotal = dimension.x * dimension.y;
	for (int i = 0; i < dimension.x * dimension.y;i++) gameTiles.push_back(TileNode());

	generateBorder(gameTiles, dimension.x, dimension.y);
	Vec2 cell = Vec2(0, 0);
	int availableIndex = 0;
	int index = getIndex(cell);
	int tileCount = 0;
	do {
		//std::cout << "CREATING TILE MAP\n";
		if (!gameTiles[index].isSet && gameTiles[index].availableTiles.size() <= 0) {
			std::cout << "triggered";
			continue;
		}
		cell = findLowestEntropy(gameTiles);
		index = getIndex(cell);
		gameTiles[index].decide();
		if (!cleanNeighbors(gameTiles, cell)) {
			resetArea(gameTiles, cell);
		}
		fixTileErrors(gameTiles);
		tileCount = completedTiles(gameTiles);
		LoadingScene::loadingCount = tileCount;
	} while (tileCount < gameTiles.size() - 1);

	fixTileErrors(gameTiles);
	auto houseMap = generateHouses(gameTiles, dimension.x, dimension.y);

	for (int i = 0; i < gameTiles.size(); i++)
	{
		auto& currentCell = gameTiles[i];
		if (currentCell.isSet) {
			createTileEntity(currentCell, Vec2(tileSize, tileSize), getCoord(i), Vec2(0, 0), houseMap);
		}
	}

	auto p = GameScene::createPlayer();

	EntityManager::createEntity().addComponent<CAudio>(CAudio(GameEngine::getAssets().getMusic("theme1"), true));
}

#pragma region Debug
std::string TileTypeToString(TILETYPE type) {
	switch (type) {
	case GRASS: return "GRASS";
	case ROAD: return "ROAD";
	case CURVE_ROAD: return "CURVE_ROAD";
	case DOG_HOUSE: return "DOG_HOUSE";
	case ROAD_BLOCK: return "ROAD_BLOCK";
	case FENCE: return "FENCE";
	case FENCE_CORNER: return "FENCE_CORNER";
	case INTERSECTION: return "INTERSECTION";
	case House_0: return "House_0";
	case House_1: return "House_1";
	case House_2: return "House_2";
	case House_3: return "House_3";
	default: return "UNKNOWN";
	}
}

void GameScene::drawDebug() {
	ImGui::Begin("Game Scene", NULL, ImGuiWindowFlags_NoResize);
	std::string text;
	if (ImGui::BeginTabBar("scene##left_tabs_bar")) {
		if (ImGui::BeginTabItem("Game Engine")) {
			text = std::format("FPS {}", GetFPS());
			ImGui::Text(text.c_str());
			ImGui::SameLine();
			text = std::format("Render delta {}", GameEngine::renderDeltaTime);
			ImGui::Text(text.c_str());

			ImGui::SameLine();
			text = std::format("System delta {}", GameEngine::systemsDeltaTime);
			ImGui::Text(text.c_str());

			text = std::format("Collidable Entities {} / {}", CollectCollisionSystem::collidableEntityCount, ComponentSet<CRigidBody>::count);
			ImGui::Text(text.c_str());
			ImGui::EndTabItem();
			ImGui::Checkbox("Show Colliders", &GameEngine::debug.showColliders);
			ImGui::Checkbox("Go Pass Edge", &CameraFollowPlayerSystem::goPassWorldBoundary);
			ImGui::Checkbox("Grass Drag", &PlayerDrivingSystem::enableGrassDrag);

			for (auto componentSet : ComponentManager::allComponentSets)
			{
				text = std::format("{} Count: {}", typeid(*componentSet).name() + 25, componentSet->GetComponentCount());
				ImGui::Text(text.c_str());
			}
		}

		if (ImGui::BeginTabItem("Systems Toggles")) {
			for (auto& system : Features)
			{
				const char* name = typeid(*system).name();

				ImGui::Checkbox(name + 6, &system->isActive);
			}
			ImGui::EndTabItem();
		}

		auto player = Entity(ComponentSet<CPlayer>::entities[0]);
		auto& rbdata = player.getComponent<CRigidBody>();

		if (ImGui::BeginTabItem("Player Info")) {
			auto ct = TileTypeToString(GameScene::onCurrentTile.type);
			auto fulls = std::format("type: {}  edges: {} {} {} {}", ct,
				TileTypeToString(GameScene::onCurrentTile.edges[0]),
				TileTypeToString(GameScene::onCurrentTile.edges[1]),
				TileTypeToString(GameScene::onCurrentTile.edges[2]),
				TileTypeToString(GameScene::onCurrentTile.edges[3])
			);
			ImGui::Text(fulls.c_str());

			int count = 0;
			if (player.hasComponent<CCollisionContainer>()) {
				count = player.getComponent<CCollisionContainer>().enterCollisions.size();
			}
			text = std::format("Player Collision Count {}", count);
			ImGui::Text(text.c_str());

			text = std::format("Position {}, {}", rbdata.position.x, rbdata.position.y);
			ImGui::Text(text.c_str());
			text = std::format("Velocity {}, {}", rbdata.velocity.x, rbdata.velocity.y);
			ImGui::Text(text.c_str());
			text = std::format("Angle {}", rbdata.angle);
			ImGui::Text(text.c_str());




			if (ImGui::Button("Activate Stun")) {
				auto& playerData = player.getComponent<CPlayer>();
				playerData.stunned = true;
				playerData.stunTimer = 3.0f;
			}


			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Car Info")) {
			auto& carData = player.getComponent<CCarController>();
			text = std::format("Position {}, {}", rbdata.position.x, rbdata.position.y);
			ImGui::Text(text.c_str());
			text = std::format("Velocity {}, {}", rbdata.velocity.x, rbdata.velocity.y);
			ImGui::Text(text.c_str());
			text = std::format("Angle {}", rbdata.angle);
			ImGui::Text(text.c_str());
			ImGui::DragFloat("Car Acceleration", &carData.carAcceleration, 1, 10.0f, 500.0f);
			ImGui::DragFloat("Turn Speed", &carData.turnSpeed, 1, 100.0f, 500.0f);
			ImGui::DragFloat("Drag", &carData.baseDragC, 0.001f, 0.001f, 1.0f);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Bullet Info")) {
			auto& playerdata = player.getComponent<CPlayer>();
			text = std::format("Bullet Entity Count", ComponentSet<TBullet>::count);
			ImGui::Text(text.c_str());

			text = std::format("Mail Time {}", playerdata.shootCoolDownTimer[MAIL]);
			ImGui::Text(text.c_str());
			text = std::format("Treat Time {}", playerdata.shootCoolDownTimer[TREAT]);
			ImGui::Text(text.c_str());
			text = std::format("Current Ammo {}", (playerdata.currentAmmo == MAIL) ? "MAIL" : "TREAT");
			ImGui::Text(text.c_str());

			ImGui::DragFloat("Bullet Speed", &playerdata.bulletSpeed, 1, 1, 60);
			ImGui::DragFloat("Mail Cooldown", &playerdata.shootCoolDownAmount[MAIL], 0.1f, 0, 15);
			ImGui::DragFloat("Treat Cooldown", &playerdata.shootCoolDownAmount[TREAT], 0.1f, 0, 15);
			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Input Info")) {
			auto& inputdata = player.getComponent<CInput>();

			text = std::format("SHOOT: {}", inputdata.isInputPress(SHOOT));
			ImGui::Text(text.c_str());
			text = std::format("Bullet Swap: {}", inputdata.isInputPressed(SWITCH_GUN));
			ImGui::Text(text.c_str());
			text = std::format("MOVE UP: {}", inputdata.isInputPress(MOVE_UP));
			ImGui::Text(text.c_str());
			text = std::format("MOVE DOWN: {}", inputdata.isInputPress(MOVE_DOWN));
			ImGui::Text(text.c_str());
			text = std::format("MOVE RIGHT: {}", inputdata.isInputPress(MOVE_RIGHT));
			ImGui::Text(text.c_str());
			text = std::format("MOVE LEFT: {}", inputdata.isInputPress(MOVE_LEFT));
			ImGui::Text(text.c_str());

			ImGui::EndTabItem();
		}

		if (ImGui::BeginTabItem("Enemy Info")) {
			if (ImGui::Button("Spawn Chaser")) {
				createDog(Vec2(GameEngine::worldWidth * 0.5f, GameEngine::worldHeight * 0.5f), CHASER);
			}
			if (ImGui::Button("Spawn Barker")) {
				createDog(Vec2(GameEngine::worldWidth * 0.5f, GameEngine::worldHeight * 0.5f), BARKER);
			}
			if (ImGui::Button("Destroy Dogs")) {
				for (auto id : ComponentSet<TDog>::entities) {
					Entity(id).destroy();
				}
			}


			ImGui::EndTabItem();
		}

		ImGui::EndTabBar();
	}
	ImGui::End();
}
#pragma endregion 
