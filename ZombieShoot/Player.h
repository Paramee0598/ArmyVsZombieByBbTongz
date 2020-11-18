#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Player
{
private:
	const float START_SPEED = 200; //การเลื่อนplayer
	const float START_HEALTH = 100; //เลือด

	
	Vector2f m_Position;

	
	Sprite m_Sprite;

	
	Texture m_Texture;

	
	Vector2f m_Resolution;

	
	IntRect m_Arena;

	
	int m_TileSize;

	
	bool m_UpPressed;
	bool m_DownPressed;
	bool m_LeftPressed;
	bool m_RightPressed;

	
	int m_Health;
	
	int m_MaxHealth;

	
	Time m_LastHit;

	
	float m_Speed;


	
public:

	Player();

	//รีเซ็ทค่าเพล์เยอร์
	void resetPlayerStats();

	void spawn(IntRect arena, Vector2f resolution, int tileSize);

	// โดนตี
	bool hit(Time timeHit);

	
	Time getLastHitTime();

	//  player
	FloatRect getPosition();

	// center of the player
	Vector2f getCenter();

	// วง player
	float getRotation();

	// ส่ง Sprite
	Sprite getSprite();

	
	int getHealth();

	// ฟังก์ชั่นเคลื่อนที่
	void moveLeft();

	void moveRight();

	void moveUp();

	void moveDown();

	//  ฟังก์ชั่นหยุดเคลื่อนที่
	void stopLeft();

	void stopRight();

	void stopUp();

	void stopDown();

	//เฟรม
	void update(float elapsedTime, Vector2i mousePosition);

	// เพิ่มสปีด
	void upgradeSpeed();

	// เพิ่มขีดจำกัดเลือด
	void upgradeHealth();

	// เพิ่มเลือด
	void increaseHealthLevel(int amount);


};



