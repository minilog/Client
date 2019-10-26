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

	Tmx::Map *map;
	std::map<int, Sprite*>  tilesetList;
	GameDebugDraw* debugDraw;

public:
	GameMap(char* filePath);
	~GameMap();

	void Draw();

	Tmx::Map* GetMap();
	std::vector<Brick*> GetBrickList();
	RECT GetWorldMapBound();

// các hàm hỗ trợ
private:
	void LoadMap(char* filePath);
	int GetWidth();
	int GetHeight();
	int GetTileWidth();
	int GetTileHeight();
};

