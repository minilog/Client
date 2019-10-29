#pragma once
#define _USE_MATH_DEFINES
#include <math.h>

namespace Define {
	const int NBit_PacketType = 3; // type [0, 7]
	const int NBit_ID = 10; // ID [0, 1023]
 	const int NBit_Position = 14; // X * 10 hoặc Y * 10 [0, 16383]
	const int NBit_PacketID = 20; // thuộc loại ID++ theo thời gian [0, 1048576]
	const int NBit_Time = 32; // thời gian TickCount [-2147483648, 2147483647]
	const int NBit_PlayerID = 2; // số người chơi [0, 3]
	const int NBit_RoomID = 2; // RoomID [0, 3]
	const int NBit_HP = 2; // máu [0, 3]
	const int NBit_Direction = 3; // hướng di chuyển [0, 4]
}

// [0, 7] => 3 bit
enum PacketType
{
	PT_Unknown,
	PT_SyncTime,
	PT_ExitGame,
	PT_UpdateRooms,
	PT_JoinRoom,
	PT_ExitRoom,
	PT_StartGame,
	PT_GameState
};

// [0, 10] => 4 bit
enum EntityTypes
{
	ET_Unknown,
	ET_Player, 
	ET_Bullet,
	ET_Boundary, 
	ET_NormalBrick, 
	ET_MetalBrick, 
	ET_Water,
	ET_ProtectItem, 
	ET_UpgradeItem,
	ET_NPC,
	ET_Eagle
};

// [0, 4] => 3 bit
enum Direction
{
	D_Stand,
	D_Left,
	D_Right,
	D_Up,
	D_Down
};