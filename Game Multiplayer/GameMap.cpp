#include "GameMap.h"
#include "GameLog.h"

GameMap::GameMap(char* filePath)
{
	LoadMap(filePath);
	debugDraw = new GameDebugDraw();
}

void GameMap::LoadMap(char* filePath)
{
	map = new Tmx::Map();
	map->ParseFile(filePath);

	// ID bắt đầu của các object trong map
	int networkID = 100;

	// hcn với size của map
	RECT r;
	r.left = 0;
	r.top = 0;
	r.right = this->GetWidth();
	r.bottom = this->GetHeight();

	// đưa danh sách hình ảnh tileset vào tilesetList
	for (size_t i = 0; i < map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map->GetTileset(i);

		Sprite *sprite = new Sprite(tileset->GetImage()->GetSource().c_str());
		tilesetList.insert(std::pair<int, Sprite*>(i, sprite));
	}

	for (int i = 0; i < map->GetNumTileLayers(); i++)
	{
		// duyệt các layer của map
		const Tmx::TileLayer *layer = map->GetTileLayer(i);
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Tile Layer 1") 
		{
			for (int j = 0; j < map->GetNumTilesets(); j++)
			{
				const Tmx::Tileset *tileSet = map->GetTileset(j);

				int tileWidth = map->GetTileWidth();
				int tileHeight = map->GetTileHeight();
				int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
				int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

				for (int m = 0; m < layer->GetHeight(); m++)
				{
					for (int n = 0; n < layer->GetWidth(); n++)
					{
						if (layer->GetTileTilesetIndex(n, m) != -1)
						{
							// xác định tọa độ object
							D3DXVECTOR2 pos(n * tileWidth + tileWidth / 2, 
								m * tileHeight + tileHeight / 2);

							// khởi tạo các loại brick, tăng ID lên mỗi lần thêm 1 viên
							Brick* brick;
							if (layer->GetName() == "Brick")
							{
								brick = new BrickNormal(pos);
								brick->NetworkID = networkID++;
							}
							else if (layer->GetName() == "Metal Brick")
							{
								brick = new MetalBrick(pos);
								brick->NetworkID = networkID++;
							}
							else if (layer->GetName() == "Water")
							{
								brick = new Water(pos);
								brick->NetworkID = networkID++;
							}
							else if (layer->GetName() == "Tile Layer 1")
							{
								brick = new Boundary(pos);
								brick->NetworkID = networkID++;
							}

							brickList.push_back(brick);
						}
					}
				}
			}
		}
	}

}

void GameMap::Draw()
{
	// vẽ các object viên brick
	for (int i = 0; i < brickList.size(); i++)
	{
		//D3DXVECTOR2 a = mListBrick[i]->GetPosition();
		//mDebugDraw->DrawRect(mListBrick[i]->GetBound());
		brickList[i]->Draw();
	}

	// vẽ các hình ảnh khác lên
	for (int i = 0; i < map->GetNumTileLayers(); i++)
	{
		const Tmx::TileLayer *layer = map->GetTileLayer(i);

		// layer là các viên brick đã khởi tạo => bỏ qua
		if (layer->GetName() == "Brick" || 
			layer->GetName() == "Metal Brick" || 
			layer->GetName() == "Water" || 
			layer->GetName() == "Tile Layer 1")
			continue;

		if (!layer->IsVisible())
		{
			continue;
		}

		RECT sourceRECT;

		int tileWidth = map->GetTileWidth();
		int tileHeight = map->GetTileHeight();

		for (int m = 0; m < layer->GetHeight(); m++)
		{
			for (int n = 0; n < layer->GetWidth(); n++)
			{
				int tilesetIndex = layer->GetTileTilesetIndex(n, m);

				if (tilesetIndex != -1)
				{
					const Tmx::Tileset *tileSet = map->GetTileset(tilesetIndex);

					int tileSetWidth = tileSet->GetImage()->GetWidth() / tileWidth;
					int tileSetHeight = tileSet->GetImage()->GetHeight() / tileHeight;

					Sprite* sprite = tilesetList[layer->GetTileTilesetIndex(n, m)];

					//tile index
					int tileID = layer->GetTileId(n, m);
					int y = tileID / tileSetWidth;
					int x = tileID - y * tileSetWidth;

					// xác định hcn trong hình ảnh gốc
					sourceRECT.top = y * tileHeight;
					sourceRECT.bottom = sourceRECT.top + tileHeight;
					sourceRECT.left = x * tileWidth;
					sourceRECT.right = sourceRECT.left + tileWidth;

					// vị trí để vẽ map lên
					D3DXVECTOR2 pos(n * tileWidth + tileWidth / 2.f, 
						m * tileHeight + tileHeight / 2.f);

					// vẽ lên
					sprite->SetPosition(pos);
					sprite->SetRect(sourceRECT);
					sprite->SetCenter(D3DXVECTOR2(tileWidth / 2.f, tileHeight / 2.f));
					sprite->Draw();
				}
			}

		}
	}
}

Tmx::Map* GameMap::GetMap()
{
	return map;
}
int GameMap::GetWidth()
{
	return map->GetWidth() * map->GetTileWidth();
}

int GameMap::GetHeight()
{
	return map->GetHeight() * map->GetTileHeight();
}
int GameMap::GetTileWidth()
{
	return map->GetTileWidth();
}

int GameMap::GetTileHeight()
{
	return map->GetTileHeight();
}

std::vector<Brick*> GameMap::GetBrickList()
{
	return brickList;
}

RECT GameMap::GetWorldMapBound()
{
	RECT bound;
	bound.left = bound.top = 0;
	bound.right = map->GetWidth() * map->GetTileWidth();
	bound.bottom = map->GetHeight() * map->GetTileHeight();

	return bound;
}

GameMap::~GameMap()
{
	for (size_t i = 0; i < tilesetList.size(); i++)
	{
		if (tilesetList[i])
			delete tilesetList[i];
	}
	tilesetList.clear();
	delete map;
	for (auto brick : brickList)
		delete brick;
}