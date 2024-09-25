#include "GameComponents.hpp"
#include "Utils.hpp"


CDogHouse::CDogHouse(float interval, DogType barking) : spawnInterval(interval), timeSinceLastSpawn(0.0f), dog(barking) {}

#pragma region Tile

void TileNode::addRotations(const TileInfo base) {
	int angle = 0;
	TileInfo newInfo = { base.direction,base.type,base.edges, base.weight };
	for (int i = 0; i < 4; i++)
	{
		if (angle == 90) {
			//right
			newInfo.direction = RIGHT;
			newInfo.edges = { base.edges[LEFT],base.edges[UP],base.edges[RIGHT],base.edges[DOWN] };
		}
		else if (angle == 180) {
			//down
			newInfo.direction = DOWN;
			newInfo.edges = { base.edges[DOWN],base.edges[LEFT],base.edges[UP],base.edges[RIGHT] };
		}
		else if (angle == 270) {
			//left
			newInfo.direction = LEFT;
			newInfo.edges = { base.edges[RIGHT],base.edges[DOWN],base.edges[LEFT],base.edges[UP] };
		}

		availableTiles.push_back(newInfo);
		angle += 90;
	}
}

TileNode::TileNode() {
	repopulate();
};

void TileNode::repopulate() {
	//GRASS
	isSet = false;
	TileInfo currentTile;
	currentTile = { UP, GRASS, {GRASS,GRASS,GRASS,GRASS},40 };
	addRotations(currentTile);

	//Intersection
	currentTile = { UP, INTERSECTION, {ROAD,ROAD,GRASS,ROAD},10 };
	addRotations(currentTile);

	//ROAD
	currentTile = { UP, ROAD, {ROAD , GRASS, ROAD, GRASS},20 };
	addRotations(currentTile);
	//CURVEROAD
	currentTile = { UP, CURVE_ROAD, { GRASS, ROAD, ROAD, GRASS },20 };
	addRotations(currentTile);

	//dog house
	currentTile = { UP, DOG_HOUSE, {GRASS,GRASS,GRASS,GRASS},7 };
	availableTiles.push_back(currentTile);
	availableTiles.push_back(currentTile);
	availableTiles.push_back(currentTile);
	availableTiles.push_back(currentTile);

	//////road block
	currentTile = { UP, ROAD_BLOCK, {GRASS , GRASS, ROAD, GRASS},5 };
	addRotations(currentTile);

	//////road block
	//currentTile = { UP, ROAD_BLOCK, {ROAD , GRASS, ROAD, GRASS} };
	//addRotations(currentTile);
}

TileInfo TileNode::decide(TileInfo i) {
	isSet = true;
	info = i;
	availableTiles.clear();
	return i;
}

TileInfo TileNode::decide() {
	int index = 0;
	int cumulativeProbability = 0;

	for (size_t i = 0; i < availableTiles.size(); ++i) {
		cumulativeProbability += availableTiles[i].weight;
	}
	float randInt = rand() % cumulativeProbability;
	for (size_t i = 0; i < availableTiles.size(); ++i) {
		if (randInt < availableTiles[i].weight) {
			index = i;
			break;
		}
		randInt -= availableTiles[i].weight;
	}

	isSet = true;
	info = availableTiles[index];
	availableTiles.clear();
	return info;
}

bool TileNode::clean(TileInfo tile, DIRECTION incomingDirection) {
	for (int i = availableTiles.size() - 1; i >= 0;i--) {
		auto direction = getOpposite(incomingDirection);
		bool notMatchingEdge = availableTiles[i].edges[direction] != tile.edges[incomingDirection];
		bool notValid = false;
		if (tile.type == ROAD_BLOCK) {
			notValid = availableTiles[i].type == ROAD_BLOCK;
		}
		if (tile.type == DOG_HOUSE) {
			notValid = availableTiles[i].type == DOG_HOUSE;
		}
		if (tile.type == INTERSECTION) {
			notValid = availableTiles[i].type == INTERSECTION;
		}
		if (notMatchingEdge || notValid) {
			availableTiles.erase(availableTiles.begin() + i);
		}
	}
	return availableTiles.size() > 0;
}

CTile::CTile() { };
CTile::CTile(TileNode t) : tile(t) {};

#pragma endregion

CPowerUp::CPowerUp() : Component<CPowerUp>() {

	int index = 0;
	int cumulativeProbability = 0;

	for (size_t i = 0; i < allPowerUps.size(); ++i) {
		cumulativeProbability += allPowerUps[i].probability;
	}
	float randInt = rand() % cumulativeProbability;
	for (size_t i = 0; i < allPowerUps.size(); ++i) {
		if (randInt < allPowerUps[i].probability) {
			index = i;
			break;
		}
		randInt -= allPowerUps[i].probability;
	}
	powerUp = allPowerUps[index];
}