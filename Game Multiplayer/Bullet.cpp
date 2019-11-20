#include "Bullet.h"

void Bullet::Read(InputMemoryBitStream & _is, bool _canReceive)
{
	bool _isDelete = false;
	int x = 0;
	int y = 0;
	Direction dir = D_Stand;
	int spawnX = 0;
	int spawnY = 0;
	int destroyX = 0;
	int destroyY = 0;

	_is.Read(_isDelete);
	_is.Read(x, NBit_Position);
	_is.Read(y, NBit_Position);
	_is.Read(dir, NBit_Direction);
	_is.Read(spawnX, NBit_Position);
	_is.Read(spawnY, NBit_Position);
	_is.Read(destroyX, NBit_Position);
	_is.Read(destroyY, NBit_Position);

	if (_canReceive)
	{
		receivedPosition = D3DXVECTOR2(x / 10.0f, y / 10.0f);

		// nếu đang delete thì cho nó là vị trí spawn
		if (IsDelete)
			position = D3DXVECTOR2(spawnX / 10.0f, spawnY / 10.0f);

		IsDelete = _isDelete;

		SetDirection(dir);

		// nếu vị trí destroy bị thay đổi thì spawn vụ nổ lên
		if (destroyPosition.x != destroyX / 10.0f || destroyPosition.y != destroyY / 10.0f)
		{
			destroyPosition = D3DXVECTOR2(destroyX / 10.0f, destroyY / 10.0f);
			for (auto ep : explosionList)
			{
				if (ep->IsDelete)
				{
					ep->Spawn(destroyPosition);
					break;
				}
			}
		}
	}
}
