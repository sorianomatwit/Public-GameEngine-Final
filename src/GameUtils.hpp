#pragma once

#include "GameScene.hpp"

int getIndex(Vec2 coord);

Vec2 getCoord(int index);

std::vector<int> getAdjacentIndexes(int index);

float getAngle(DIRECTION d);

int completedTiles(std::vector<TileNode>& allTiles);

Vec2 findLowestEntropy(std::vector<TileNode>& allTiles);

int resetArea(std::vector<TileNode>& allTiles, Vec2 centerCell);

bool cleanNeighbors(std::vector<TileNode>& allTiles, Vec2 placedCell);

void generateBorder(std::vector<TileNode>& allTiles, int width, int height);

std::map <int, std::shared_ptr<House>> generateHouses(std::vector<TileNode>& allTiles, int width, int height);

void fixTileErrors(std::vector<TileNode>& allTiles);
