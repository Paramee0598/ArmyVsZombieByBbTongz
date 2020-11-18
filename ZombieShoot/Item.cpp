#include "pch.h"
#include "Item.h"
#include "TextureHolder.h"

Item::Item(int type)
{
	
	m_Type = type;

	//type เลือด = 1 กระสุน = 2
	if (m_Type == 1)
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/health_pickup.png"));

	
		m_Value = HEALTH_START_VALUE;

	}
	else
	{
		m_Sprite = Sprite(TextureHolder::GetTexture(
			"graphics/ammo_pickup.png"));

	
		m_Value = AMMO_START_VALUE;
	}

	m_Sprite.setOrigin(25, 25);

	m_SecondsToLive = START_SECONDS_TO_LIVE;
	m_SecondsToWait = START_WAIT_TIME;
}

void Item::setArena(IntRect arena)
{

	//ยัดค่า m_Arena
	m_Arena.left = arena.left + 50;
	m_Arena.width = arena.width - 50;
	m_Arena.top = arena.top + 50;
	m_Arena.height = arena.height - 50;

	spawn();
}

void Item::spawn()
{
	// สุ่ม สปอน
	srand((int)time(0) / m_Type);
	int x = (rand() % m_Arena.width);
	srand((int)time(0) * m_Type);
	int y = (rand() % m_Arena.height);

	
	m_Spawned = false; 
	m_SecondsSinceSpawn = 0;
	

	m_Sprite.setPosition(x, y);
}

FloatRect Item::getPosition()
{
	return m_Sprite.getGlobalBounds();
}

Sprite Item::getSprite()
{
	return m_Sprite;
}

bool Item::isSpawned()
{
	return m_Spawned;
}

int Item::gotIt()
{
	m_Spawned = false;
	m_SecondsSinceDeSpawn = 0;
	return m_Value;
}

void Item::update(float elapsedTime)
{
	if (m_Spawned)
	{
		m_SecondsSinceSpawn += elapsedTime;
	}
	else
	{
		m_SecondsSinceDeSpawn += elapsedTime;
	}


	// สุ่มมาละหาย
	if (m_SecondsSinceSpawn > m_SecondsToLive && m_Spawned)
	{
		
		m_Spawned = false;
		m_SecondsSinceDeSpawn = 0;
	}

	// สปอนต่อ
	if (m_SecondsSinceDeSpawn > m_SecondsToWait && !m_Spawned)
	{
	
		spawn();
	}

}

void Item::upgrade()
{
	if (m_Type == 1)
	{
		m_Value += (HEALTH_START_VALUE * .5);
	}
	else
	{
		m_Value += (AMMO_START_VALUE * .5);
	}

	//การอยู่นานของไอเทม กับ หายไป
	m_SecondsToLive += (START_SECONDS_TO_LIVE / 10);
	m_SecondsToWait -= (START_WAIT_TIME / 10);
}
