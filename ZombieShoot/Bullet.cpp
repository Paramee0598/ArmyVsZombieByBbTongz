#include "pch.h"
#include "Bullet.h"



Bullet::Bullet()
{
	//ขนาดกระสุน
	m_BulletShape.setSize(sf::Vector2f(5, 5));
	m_BulletShape.setFillColor(sf::Color::Yellow);
}

void Bullet::shoot(float startX, float startY,
	float targetX, float targetY)
{
	//ติดตามกระสุน
	m_InFlight = true;
	m_Position.x = startX;
	m_Position.y = startY;

	// การวิ่งของกระสุน
	float gradient = (startX - targetX) / (startY - targetY);

	// ทำให้ตำหน่งกระสุนนื่ง
	if (gradient < 0)
	{
		gradient *= -1;
	}

	//ความเร็วตอนวิ่งขึ้นกนY
	float ratioXY = m_BulletSpeed / ( 1+gradient);
	
	
	m_BulletDistanceY = ratioXY;   //กระสุนกนX
	m_BulletDistanceX = ratioXY * gradient; //กระสุนกนY

	//ถ้ากระสุนวิ่งไปซ้ายมันจะเป็นค่าลบแล้วมันจะวิ่งย้อนกลับ  แกนX
	//ทำให้มันไม่ลบ
	if (targetX < startX)
	{
		m_BulletDistanceX *= -1;
	}
	//ถ้ากระสุนวิ่งไปล่างมันจะเป็นค่าลบแล้วมันจะวิ่งย้อนกลับ  แกนY
	//ทำให้มันไม่ลบ
	if (targetY < startY)
	{
		m_BulletDistanceY *= -1;
	}

	
	//m_XTarget = targetX;
	//m_YTarget = targetY;

	//ระยะยิงไกล 1000 pixels
	float range = 1000;
	//ลบออกเพื่อวื่งได้
	m_MinX = startX - range;
	m_MaxX = startX + range;
	m_MinY = startY - range;
	m_MaxY = startY + range;

	
	m_BulletShape.setPosition(m_Position);
}

void Bullet::stop()
{
	m_InFlight = false;
}

bool Bullet::isInFlight()
{
	return m_InFlight;
}

FloatRect Bullet::getPosition()
{
	return m_BulletShape.getGlobalBounds();
}

RectangleShape Bullet::getShape()
{
	return m_BulletShape;
}


void Bullet::update(float elapsedTime)
{
	///ทำให้กระสุนสมูท
	m_Position.x += m_BulletDistanceX * elapsedTime;
	m_Position.y += m_BulletDistanceY * elapsedTime;

	// กระสุนเคลื่อนที่
	m_BulletShape.setPosition(m_Position);

	
	if (m_Position.x < m_MinX || m_Position.x > m_MaxX ||
		m_Position.y < m_MinY || m_Position.y > m_MaxY)
	{
		m_InFlight = false;
	}
	
}