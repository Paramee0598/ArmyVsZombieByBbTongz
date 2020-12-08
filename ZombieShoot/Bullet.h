#pragma once
#include <SFML/Graphics.hpp>

using namespace sf;

class Bullet
{
private:
	// ตำแหน่งกระสุน
	Vector2f m_Position;

	
	RectangleShape m_BulletShape;

	
	bool m_InFlight = false;

	// กระสุนวิ่งสุดที่
	float m_BulletSpeed = 1000;

	// เเกนX Y
	float m_BulletDistanceX =0;
	float m_BulletDistanceY = 0;

	// กระสุน ตอนนี้
	float m_XTarget = 0;
	float m_YTarget = 0;

	// จำกัดระยะ
	float m_MaxX = 0;
	float m_MinX = 0;
	float m_MaxY = 0;
	float m_MinY = 0;

	
public:
	// สร้างกระสุน
	Bullet();

	// หยุดสร้าง
	void stop();

	
	bool isInFlight();

	// new bullet
	void shoot(float startX, float startY,
		float xTarget, float yTarget);

	
	FloatRect getPosition();

	
	RectangleShape getShape();

	// เฟรมกระสุน
	void update(float elapsedTime);

};