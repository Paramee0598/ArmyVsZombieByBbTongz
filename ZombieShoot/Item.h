#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Item
{
private:
	//เก็บมาแล้ว เพิ่มเท่าไร
	const int HEALTH_START_VALUE = 50;
	const int AMMO_START_VALUE = 12;
	const int START_WAIT_TIME = 10;
	const int START_SECONDS_TO_LIVE = 5;

	//ตัวเกิด
	Sprite m_Sprite;

	
	IntRect m_Arena;

	
	int m_Value;

	
	// 1 = health, 2 = ammo
	int m_Type;

	
	bool m_Spawned;
	float m_SecondsSinceSpawn;
	float m_SecondsSinceDeSpawn;
	float m_SecondsToLive;
	float m_SecondsToWait;

	
public:

	Item(int type);

	// สุ่มไอเท็ม
	void setArena(IntRect arena);

	void spawn();

	
	FloatRect getPosition();

	
	Sprite getSprite();

	// เฟรม
	void update(float elapsedTime);

	
	bool isSpawned();

	// เก็บ
	int gotIt();

	// อัตตราสปอน
	void upgrade();

};



