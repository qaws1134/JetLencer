#pragma once

namespace MATID
{
	enum ID { TRANS, ROT, SCALE, END };
}

namespace OBJID
{
	enum ID 
	{ 
		BACKGROUND, 
		PLAYER, 
		ENEMY, 
		PLAYER_BULLET, 
		ENEMY_BULLET, 
		MOUSE, 
		EFFECT, 
		UI, 
		DRONE, 
		END 
	};
}

namespace RENDERID
{
	enum ID 
	{
		BACKGROUND,
		MOVE_BACKGROUND1,
		MOVE_BACKGROUND2,
		MOVE_BACKGROUND3,
		GROUND,
		BULLET,
		OBJECT,
		EFFECT,
		BURNER,
		UI,
		END
	};
}
namespace COLLIDER
{
	enum ID 
	{
		PLAYER,
		PLAYER_SEARCH,
		ENEMY,
		PLAYER_BULLET,
		PLAYER_BULLET_BEAM,
		ENEMY_BULLET,
		EFFECT,
		END
	};
}

namespace PLAYER
{
	enum STATE 
	{
		IDLE,
		ROLL,
		ACCEL,
		AFTER_BURNUR,
		OVERHEAT,
		ROCKET,
		SPECIAL_CHARGE,
		SPECIAL_RELOAD_START,
		SPECIAL_RELOAD_END,
		END};
}
namespace ENEMY
{
	enum STATE 
	{
		ATTACK,			
		BACK,
		END
	};
}

namespace ARROW
{
	enum UI
	{
		DISTANS_FAR,		//ȸ��
		DISTANS_LEVEL0,		//���0ĭ
		DISTANS_LEVEL1,		//���1ĭ
		DISTANS_LEVEL2,		//���2ĭ
		END
	};
}


namespace UI
{
	enum TYPE
	{
		//���� �Ÿ� Gui
		ARROW_OFFSCREEN_BASE,
		ARROW_OFFSCREEN_DISTANCE1,
		ARROW_OFFSCREEN_DISTANCE2,
		ARROW_OFFSCREEN_RECT,
		ARROW_OFFSCREEN_ICON,

		AFTERBURNER,
		AFTERBURNER_RED,

		FLIP_PLATE,
		FLIP_PLATE_RED,
		FLIP_GLITCH,
		FLIP_VERTSPEED,
		FLIP_VERTSPEED_RED,

		ARROW,
		ARROW_RED,

		OVERHEAT_PLATE,
		OVERGEAT_PLATE_RED,
		OVERHEAT_TEXT,
		OVERHEAT_TEXT_RED,

		ROCKET,
		ROCKET_RED,
		ROCKET_PLATE,
		ROCKET_PLATE_RED,

		SPECIAL_CHARGE,
		SPECIAL_PLATE,
		SPECIAL_PLATE_RED,
		SPECIAL_RELOAD_GLITCH,
		SPECIAL_RELOAD_BAR,
		SPECIAL_RELOAD_READY_BAR,
		SPECIAL_RELOAD_READY_BACKPLATE,
		SPECIAL_RELOAD_READY_BACKPLATE_RED,
		

	};
	//������
	enum ICON
	{
		JET,
		JETS,
		SHIP,
		HACKER_TOWER,
		TARGET,
		SHILED,
		BOOM,
		TANK,
		HACKER_JET,
		BOSS,
	};
}

namespace BULLET
{
	enum TYPE { NORMAL, GUIDE, SHOTGUN, END };

	enum SUBWEAPON {CHARGE_0,CHARGE_1,CHARGE_2,CHARGE_3};
}

namespace EFFECT
{
	enum TYPE 
	{
		ROCKET_PTFIRE, 
		ROCKET_BOOM_PTFIRE,
		JET_PTFIRE,
		CHAGE_BEAM,
		GROUND_WATERSPLASH, 
		GROUND_WATERSPLASH_FAST,
		END
	};
}

namespace BURNER
{
	enum STATE { IDLE, START, START_OFF, ACCEL, AFTER, MEGA, BURST,END };
}

namespace ANGLE
{
	enum STATE {LEFT,UPLEFT,UP,UPRIGHT,RIGHT,DOWNRIGHT,DOWN,DOWNLEFT,END};
}

namespace SCROLL
{
	enum ID {LEFT,TOP,RIGHT,BOTTOM,END};
}