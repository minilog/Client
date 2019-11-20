#include "GameMap.h"
#include "GameLog.h"

GameMap::GameMap(char* filePath)
{
	LoadMap(filePath);
	//debugDraw = new GameDebugDraw();
}

void GameMap::LoadMap(char* filePath)
{
	map = new Tmx::Map();
	map->ParseFile(filePath);

	// đưa danh sách hình ảnh tileset vào tilesetList
	for (int i = 0; i < map->GetNumTilesets(); i++)
	{
		const Tmx::Tileset *tileset = map->GetTileset(i);
		//char* link = "Resource files/%s", tileset->GetImage()->GetSource().c_str();
		Sprite *sprite = new Sprite("Resource files/tileset.png", D3DXCOLOR(255, 255, 255, 255));
		tilesetList.insert(std::pair<int, Sprite*>(i, sprite));
	}

	// duyệt các layer của map
	for (int i = 0; i < map->GetNumTileLayers(); i++)
	{	
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

				for (int m = 0; m < layer->GetHeight(); m++)
				{
					for (int n = 0; n < layer->GetWidth(); n++)
					{
						if (layer->GetTileTilesetIndex(n, m) != -1)
						{
							// tọa độ object
							D3DXVECTOR2 pos((float)(n * tileWidth + tileWidth / 2), 
								(float)(m * tileHeight + tileHeight / 2));

							// khởi tạo brick
							Brick* brick;
							if (layer->GetName() == "Brick")
							{
								brick = new BrickNormal(pos);
								brickNorList.push_back((BrickNormal*)brick);
							}
							else if (layer->GetName() == "Metal Brick")
							{
								brick = new MetalBrick(pos);
							}
							else if (layer->GetName() == "Water")
							{
								brick = new Water(pos);
							}
							else if (layer->GetName() == "Tile Layer 1")
							{
								brick = new Boundary(pos);
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
	for (int i = 0; i < (int)brickList.size(); i++)
	{
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