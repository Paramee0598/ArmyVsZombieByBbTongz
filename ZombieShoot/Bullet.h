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
	float m_BulletDistanceX;
	float m_BulletDistanceY;

	// กระสุน ตอนนี้
	float m_XTarget;
	float m_YTarget;

	// จำกัดระยะ
	float m_MaxX;
	float m_MinX;
	float m_MaxY;
	float m_MinY;

	
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