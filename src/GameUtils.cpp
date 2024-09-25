#include "GameUtils.hpp"
#include "GameComponents.hpp"
#include "EngineComponents.hpp"

#pragma region Map Generation

int getIndex(Vec2 coord) {
	if (coord.x < 0 || coord.x >= GameScene::dimension.x || coord.y < 0 || coord.y >= GameScene::dimension.y) return -1;
	return coord.y * GameScene::dimension.x + coord.x;
}

Vec2 getCoord(int index) {
	int cols = GameScene::dimension.x;
	return Vec2(index % cols, index / cols);
}

std::vector<int> getAdjacentIndexes(int index) {
	auto centerCell = getCoord(index);
	return {
		getIndex(centerCell + Vec2(0,-1)),
		getIndex(centerCell + Vec2(1,0)),
		getIndex(centerCell + Vec2(0,1)),
		getIndex(centerCell + Vec2(-1, 0))
	};
}

float getAngle(DIRECTION d) {
	float angle = 0;

	switch (d)
	{
	case UP:
		angle = 0;
		break;
	case DOWN:
		angle = 180;
		break;
	case RIGHT:
		angle = 270 + 180;
		break;
	case LEFT:
		angle = 270;
		break;
	default:
		break;
	}
	return angle;
}

int completedTiles(std::vector<TileNode>& allTiles) {
	int sum = 0;
	for (int i = 1; i < allTiles.size(); i++)
	{
		if (allTiles[i].isSet) sum++;
	}
	return sum;
}

Vec2 findLowestEntropy(std::vector<TileNode>& allTiles) {
	Vec2 cell = getCoord(0);
	int entropy = allTiles[0].availableTiles.size();

	for (int i = 1; i < allTiles.size(); i++)
	{
		if (allTiles[i].isSet) continue;
		if (entropy > allTiles[i].availableTiles.size() || entropy <= 0) {
			cell = getCoord(i);
			entropy = allTiles[i].availableTiles.size();
		}
	}
	return cell;

}

int resetArea(std::vector<TileNode>& allTiles, Vec2 centerCell) {

	int index = getIndex(centerCell);
	auto allNeighbors = getAdjacentIndexes(index);
	allTiles[index] = TileNode();
	int resetCount = 1;
	for (int i = 0; i < 4; i++)
	{
		if (allNeighbors[i] != -1 && allTiles[allNeighbors[i]].info.type != FENCE) {
			allTiles[allNeighbors[i]] = TileNode();
		}
	}
	for (int k = 0; k < 4; k++)
	{
		if (allNeighbors[k] != -1) {
			if (allTiles[allNeighbors[k]].info.type == FENCE || allTiles[allNeighbors[k]].info.type == FENCE_CORNER) continue;

			auto getOtherNeighbors = getAdjacentIndexes(allNeighbors[k]);
			bool canClean = true;
			for (int i = 0; i < 4; i++)
			{
				if (getOtherNeighbors[i] != -1 && allTiles[getOtherNeighbors[i]].isSet)
				{
					auto tile = allTiles[getOtherNeighbors[i]];
					canClean = canClean && allTiles[allNeighbors[k]].clean(tile.info, static_cast<DIRECTION>(i));
				}
			}
			if (canClean) {
				allTiles[allNeighbors[k]].decide();
			}
			else allTiles[allNeighbors[k]] = TileNode();

		}
	}
	bool canClean = true;
	for (int i = 0; i < 4; i++)
	{
		if (allNeighbors[i] != -1 && allTiles[allNeighbors[i]].isSet) {
			canClean = canClean && allTiles[index].clean(allTiles[allNeighbors[i]].info, static_cast<DIRECTION>(i));
		}
	}
	if (canClean) {
		allTiles[index].decide();
	}
	else allTiles[index].repopulate();

	return resetCount;
}

bool cleanNeighbors(std::vector<TileNode>& allTiles, Vec2 placedCell) {
	auto index = getIndex(placedCell);
	auto& placedTile = allTiles[index];

	Vec2 cells[4]{
		placedCell + Vec2(0,-1),
		placedCell + Vec2(1,0),
		placedCell + Vec2(0,1),
		placedCell + Vec2(-1,0)
	};
	int sides[4] = {
			getIndex(cells[0]),
			getIndex(cells[1]),
			getIndex(cells[2]),
			getIndex(cells[3])
	};
	bool output = true;
	for (int i = 0; i < 4; i++) {
		if (sides[i] != -1 && !allTiles[sides[i]].isSet) {
			output = output && allTiles[sides[i]].clean(placedTile.info, static_cast<DIRECTION>(i));
		}
	}
	return output;
}

void generateBorder(std::vector<TileNode>& allTiles, int width, int height) {
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width;x++) {
			if (x <= 0 || x >= width - 1 || y <= 0 || y >= height - 1) {
				auto dir = DOWN;
				auto type = FENCE;
				if (x <= 0) dir = LEFT;
				if (x >= width - 1) dir = RIGHT;
				if (y <= 0) dir = UP;

				if (x == 0 && y == 0) {
					type = FENCE_CORNER;
					dir = RIGHT;
				}
				if (x >= width - 1 && y >= height - 1) {
					type = FENCE_CORNER;
					dir = LEFT;
				}
				if (x >= width - 1 && y == 0) {
					type = FENCE_CORNER;
					dir = DOWN;
				}
				if (x == 0 && y == height - 1) {
					type = FENCE_CORNER;
					dir = UP;
				}
				auto cell = Vec2(x, y);
				allTiles[getIndex(cell)].decide({ dir, type, {GRASS,GRASS,GRASS,GRASS} });
				cleanNeighbors(allTiles, cell);
			}
		}
	}

}

std::map <int, std::shared_ptr<House>> generateHouses(std::vector<TileNode>& allTiles, int width, int height) {

	std::map <int, std::shared_ptr<House>> output;

	int houseAmount = 0;
	for (int k = 0; k < allTiles.size(); k++)
	{
		auto firstCell = getCoord(k);
		Vec2 point[4] = {
			firstCell,
			firstCell + Vec2(1,0),
			firstCell + Vec2(0,1),
			firstCell + Vec2(1,1)
		};
		int indexes[4] = {
			getIndex(point[0]),
			getIndex(point[1]),
			getIndex(point[2]),
			getIndex(point[3]),
		};
		bool allGrass = true;
		bool allDoghouse = true;
		for (int i = 0; i < 4; i++)
		{
			allGrass = allGrass && (indexes[i] != -1 && (allTiles[indexes[i]].info.type == GRASS));
			allDoghouse = allDoghouse && (indexes[i] != -1 && (allTiles[indexes[i]].info.type == DOG_HOUSE));
		}
		int randNum = Lehmer32(rand()) % 100;

		if ((allGrass && randNum < 60) || allDoghouse) { //gens house
			int houseAmount = 3 + (rand() % 6);
			std::shared_ptr<House> h = std::make_shared<House>(houseAmount,houseAmount, false, Vec2(0,0));


			for (int i = 0; i < 4; i++) //sets tiles to house
			{
				output[indexes[i]] = h;

				auto type = static_cast<TILETYPE>(i);
				allTiles[indexes[i]].decide({ UP,type,{type,type,type,type} });
				houseAmount++;
			}
		}
	}
	return output;
}

void fixTileErrors(std::vector<TileNode>& allTiles) {
	std::vector<int> errorIndexes;
	std::set<int> bruh;
	for (int i = 0; i < allTiles.size(); i++)
	{
		bool isError = false;
		if (allTiles[i].info.type == FENCE || allTiles[i].info.type == FENCE_CORNER) continue;

		auto neighbors = getAdjacentIndexes(i);
		for (int k = 0; k < neighbors.size(); k++)
		{
			if (neighbors[k] != -1 && allTiles[i].isSet && allTiles[neighbors[k]].isSet) {
				auto adjacentTile = allTiles[neighbors[k]];
				DIRECTION adjacentDir = static_cast<DIRECTION>(k);
				auto opp = TileNode::getOpposite(adjacentDir);
				isError = isError || allTiles[i].info.edges[adjacentDir] != adjacentTile.info.edges[opp];
				if (isError) {
					break;
				}
			}
		}
		if (isError) {
			allTiles[i] = TileNode();
			errorIndexes.push_back(i);
		}
	}

	for (int i = 0; i < errorIndexes.size(); i++)
	{
		auto neighbors = getAdjacentIndexes(i);
		for (int k = 0; k < neighbors.size(); k++)
		{
			if (neighbors[k] != -1 && allTiles[neighbors[k]].isSet) {
				auto& adjacentTile = allTiles[neighbors[k]];
				DIRECTION adjacentDir = static_cast<DIRECTION>(k);
				allTiles[errorIndexes[i]].clean(adjacentTile.info, adjacentDir);
			}
		}
	}

}


#pragma endregion 