#pragma once
#include <d3dx9.h>
#include <d3d9.h>
#include <vector>

#include "Sprite.h"	
#include "MapReader\Tmx.h.in"
#include "GameGlobal.h"
#include "Brick.h"
#include "BrickNormal.h"
#include "MetalBrick.h"
#include "Water.h"
#include "Boundary.h"
#include "GameCollision.h"
#include "GameDebugDraw.h"
#include "QuadTree.h"

class GameMap
{
	std::vector<Brick*> brickList;
	std::vector<BrickNormal*> brickNorList;
	Tmx::Map *map;
	std::map<int, Sprite*>  tilesetList;
	GameDebugDraw* debugDraw;

public:
	GameMap(char* filePath);
	~GameMap();

	void Draw();

	Tmx::Map* GetMap() { return map; }
	std::vector<Brick*> GetBrickList() { return brickList; }
	std::vector<BrickNormal*> GetBrickNorList() { return brickNorList; }

// các hàm hỗ trợ
private:
	void LoadMap(char* filePath);
	int GetWidth() { return map->GetWidth() * map->GetTileWidth(); }
	int GetHeight() { return map->GetHeight() * map->GetTileHeight(); }
	int GetTileWidth() { return map->GetTileWidth(); }
	int GetTileHeight() { return map->GetTileHeight(); }
};

