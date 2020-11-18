#include "pch.h"
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ZombieShoot.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Item.h"

using namespace sf;

int main()
{
	// เรียกใช้ TextureHolder
	TextureHolder Main;

	// สถานะเกม ใน State หยุดได้ อัพเวล จบเกม เล่น
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING };
	// สถานะไหนตอนเริ่ม

	State state = State::GAME_OVER; // 1.ตอนเริ่ม state จะเป็นดังนี้


	// ขนาด window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width; // กว้าง
	resolution.y = VideoMode::getDesktopMode().height; //สูง

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Army Shooter", Style::Fullscreen);

	// view
	View mainView(sf::FloatRect(0, 0, resolution.x, resolution.y));


	Clock clock;

	Time gameTimeTotal;

	Vector2f mouseWorldPosition;

	Vector2i mouseScreenPosition;

	Player player;

	IntRect arena;

	VertexArray background;

	// Load ground 
	Texture textureBackground = TextureHolder::GetTexture(
		"graphics/background_sheet11.png");


	int numZombies;
	int numZombiesAlive;
	

	// ค่ากระสุน
	Bullet bullets[100];
	int currentBullet = 0;
	int bulletsSpare = 24;
	int bulletsInClip = 6;
	int reClipSize = 6;
	float fireRate = 1;

	Time lastPressed;

	//รูปยัดเมาส์
	window.setMouseCursorVisible(true);
	Sprite spriteCrosshair;
	Texture textureCrosshair = TextureHolder::GetTexture("graphics/crosshair1.png");
	spriteCrosshair.setTexture(textureCrosshair);
	spriteCrosshair.setOrigin(30, 35);

	// ค่าของที่สร้าง
	Item healthPickup(1);
	Item ammoPickup(2);

	// score
	int score = 0;
	int hiScore = 0;
	int lastScore = 0;

	// หน้า home  พื้นหลัง
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/SjAw8B.jpg");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(150, 100);

	//หน้า level up
	Sprite spriteLevelUp;
	Texture textureLevelUp = TextureHolder::GetTexture("graphics/leup.jpg");
	spriteLevelUp.setTexture(textureLevelUp);
	spriteLevelUp.setPosition(100, 50);

	// view
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//  sprite กระสุน icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(1650, 973);

	// Load font
	Font font;
	font.loadFromFile("fonts/youmurdererbb.ttf");

	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::White);
	gameOverText.setPosition(250, 850);
	gameOverText.setString("Press Space to play games");

	// หยุด
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::White);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press  Space \nto continue");

	// หน้า Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setPosition(150, 250); //ตำแหน่งตัวอักษร
	std::stringstream levelUpStream;
	

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(75);
	ammoText.setFillColor(Color::White);
	ammoText.setPosition(1750, 980);

	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setPosition(720, 5);

	// Last Score
	Text lastScoreText;
	lastScoreText.setFont(font);
	lastScoreText.setCharacterSize(55);
	lastScoreText.setFillColor(Color::White);
	lastScoreText.setPosition(20, 100);
	/*std::stringstream s;
	s << "Last Score:" << lastScore; //////////////////////////////////////////////////////////////////////////////////////////////////////////////
	lastScoreText.setString(s.str());*/

	// ดึง high score จาก text file/
	std::ifstream inputFile("gamedata/scores.txt");
	if (inputFile.is_open())
	{
		inputFile >> hiScore;
		inputFile.close();
	}

	// Hight Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::Green);
	hiScoreText.setPosition(20, 5);
	std::stringstream s;
	s << "Hight Score:" << hiScore; //ตอนหน้าhome
	hiScoreText.setString(s.str());

	// จำนวน Zombies 
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setPosition(175, 980);
	zombiesRemainingText.setString("");
	
	// ด่าน
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setPosition(25, 980);
	waveNumberText.setString("");

	// แถบเลือด
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Black);
	healthBar.setPosition(800, 980);

	
	int framesSinceLastHUDUpdate = 0;

	Time timeSinceLastUpdate;
	//frames เกม 
	int fpsMeasurementFrameInterval = 1000;

	// เสียงตอนโดนตี
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/hit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// เสียงตอนยิงโดน
	SoundBuffer splatBuffer;
	splatBuffer.loadFromFile("sound/splat.wav");
	sf::Sound splat;
	splat.setBuffer(splatBuffer);

	// เสียงตอนยิง
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);

	// เสียงรีโหลด
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload1.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);

	// เสียงตอนหมด
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);

	// เสียงเพิ่มพลัง
	SoundBuffer powerupBuffer;
	powerupBuffer.loadFromFile("sound/powerup.wav");
	Sound powerup;
	powerup.setBuffer(powerupBuffer);

	// เสียงตอนเก็บ
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/pickup.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// เสียงเริ่มเกม
	SoundBuffer startGameBuffer;
	startGameBuffer.loadFromFile("sound/counterStrike.wav");
	Sound startGame;
	startGame.setBuffer(startGameBuffer);

	Music musicHomeGame;
	musicHomeGame.openFromFile("sound/ShooterZombieArmy.wav");
	musicHomeGame.play();

	Music musicInGame;
	musicInGame.openFromFile("sound/ZombieInGame.wav");
	


	// main game loop
	while (window.isOpen())
	{
		// เริ่มมมมมมมมมมมมมมมมมม

		// player กับ เกม
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}


		Event event;
		while (window.pollEvent(event))  //เช็คevent ตอนแรกมันน่าจะผ่านไปก่อน มันมาเช็คตอนเริ่มวาดแล้ว เพราะ มันคือ event  //ตอนผ่านloopนี้มันน่าจะเร็วมากเร็วจนกดไม่ทันมันเลยข้ามไปที่loop // เข้าสู่การเล่น เลย มันเลยไม่บัค
			 
		{
			if (event.type == Event::KeyPressed)
			{
				// หยุดเกมตอนเล่น  ///
				if (event.key.code == Keyboard::Space &&
					state == State::PLAYING) // *****4.***ถ้ากดทันตอนแรกเข้าเกมน่าจะบัคตรงนี้

				{
					musicInGame.pause();
					musicHomeGame.play();
					state = State::PAUSED;
				}

				// รีดฟรมตอนหยุด เล่นต่อ
				else if (event.key.code == Keyboard::Space &&
					state == State::PAUSED)
				{
					musicInGame.play();
					state = State::PLAYING;
					clock.restart();
				}

				// ทำเมื่อเกมจบ
				else if (event.key.code == Keyboard::Space &&
					state == State::GAME_OVER)
				{
					state = State::LEVELING_UP;
					wave = 0;
					score = 0;

					// เกมจบจะเซทตามนี้
					currentBullet = 0;
					bulletsSpare = 24; // กระสุนเติมตอนเริ่ม
					bulletsInClip = 6; // กระสุนใช้ยิง
					reClipSize = 6; //เติมทีละ 6 ลูก
					fireRate = 1; //อัตรายิง

					// Reset the player stats
					player.resetPlayerStats();
					
					musicHomeGame.pause();
					
				}

				if (state == State::PLAYING)
				{
					// Reload
					if (event.key.code == Keyboard::R)
					{
						if (bulletsSpare >= reClipSize)
						{
							// ลบกระสุน
							bulletsInClip = reClipSize;
							bulletsSpare -= reClipSize;
							reload.play();
						}
						else if (bulletsSpare > 0)
						{
							// กระสุนเติมใกล้หมด
							bulletsInClip = bulletsSpare;
							bulletsSpare = 0;
							reload.play();
						}
						else
						{
							// กระสุนหมดเติมไม่ได้
							reloadFailed.play();
						}
					}
				}

			}
		}// จบ


		// เข้าสู่การเล่น
		if (state == State::PLAYING) //4. เมื่อผ่านบัคมาอย่างรวดเร็วเเล้ว จะเข้าloopนี้เพื่อทำเซ็ทฟังค์ชั่นต่างๆ เช่น เดินขึ้น ลง 
		{
			//  WASD keys play
			if (Keyboard::isKeyPressed(Keyboard::W))
			{
				player.moveUp();
			}
			else
			{
				player.stopUp();
			}

			if (Keyboard::isKeyPressed(Keyboard::S))
			{
				player.moveDown();
			}
			else
			{
				player.stopDown();
			}

			if (Keyboard::isKeyPressed(Keyboard::A))
			{
				player.moveLeft();
			}
			else
			{
				player.stopLeft();
			}

			if (Keyboard::isKeyPressed(Keyboard::D))
			{
				player.moveRight();
			}
			else
			{
				player.stopRight();
			}

			// ยิง

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			{
				
				
				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip > 0)
				{

					//ยิงจากตัวผู้เล่น
					bullets[currentBullet].shoot(
						player.getCenter().x, player.getCenter().y,
						mouseWorldPosition.x, mouseWorldPosition.y);

					currentBullet++;
					if (currentBullet > 99)
					{
						currentBullet = 0;
					}
					lastPressed = gameTimeTotal;
					shoot.play(); // กด
					bulletsInClip--; //ลดกระสุน
					

				}//กระสุนหมด
				if (gameTimeTotal.asMilliseconds()
					- lastPressed.asMilliseconds()
					> 1000 / fireRate && bulletsInClip >= 0) {
					lastPressed = gameTimeTotal;
					reloadFailed.play();
					continue;

				}
				
			}// จบ ยิง

		}// จบเดิน

		// ////////เปลี่ยนด่าน//////////////////////////////////////////////
		if (state == State::LEVELING_UP)
		{
			musicInGame.pause();
			// Handle the player levelling up
			if (event.key.code == Keyboard::Num1)
			{
				// ยิงรัวขึ้น
				fireRate++;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num2)
			{
				// เพิ่มตัวเก็บกระสุน
				reClipSize += reClipSize;
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num3)
			{
				// เพิ่มขีดจำกัดเลือด
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if (event.key.code == Keyboard::Num4)
			{
				// เพิ่มความเร็ว
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			//เพิ่มการสุ่มไอเท็มเลือด
			if (event.key.code == Keyboard::Num5)
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			//เพิ่มการสุ่มไอเท็มกระสุน
			if (event.key.code == Keyboard::Num6)
			{
				ammoPickup.upgrade();
				state = State::PLAYING;
			}
			// มาด่าน
			if (state == State::PLAYING) //5.ยังเซ็ทอยู่
			{
				// เพิ่มด่าน
				wave++;

				//ขยายพื้นที่ตามด่าน
				arena.width = 610 * wave;
				arena.height = 610 * wave;
				arena.left = 0;
				arena.top = 0;

				//สร้างด่านตามนั้นทุกครั้ง
				int tileSize = createBackground(background, arena);

				//ให้ player เกิดตรงกลาง
				player.spawn(arena, resolution, tileSize);

				//  pick-ups
				healthPickup.setArena(arena);
				ammoPickup.setArena(arena);

				// สร้าง zombies //กี่ตัวๆ ///////////////////////////////////////////////////////////////////////////////////
				numZombies = 0 * wave;

				
				
				
				numZombiesAlive = numZombies;

				startGame.play();
				musicInGame.play();


				clock.restart();
			}
		}// End levelling up


		 ////////***////*
		 /*เฟรม*/
		 ///////***////*

		if (state == State::PLAYING) ///5.แล้วมาเข้าloopนี้ต่อ เพื่อเซ็ทอีก
		{

			Time dt = clock.restart();

			gameTimeTotal += dt;

			float dtAsSeconds = dt.asSeconds();

			// เมาส์ pointer
			mouseScreenPosition = Mouse::getPosition();

			// เมาส์อยู่ใน view
			mouseWorldPosition = window.mapPixelToCoords(
				Mouse::getPosition(), mainView);


			spriteCrosshair.setPosition(mouseWorldPosition);


			player.update(dtAsSeconds, Mouse::getPosition());


			Vector2f playerPosition(player.getCenter());


			mainView.setCenter(player.getCenter());

			

			// กระสุนวิ่ง
			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					bullets[i].update(dtAsSeconds);
				}
			}


			healthPickup.update(dtAsSeconds);
			ammoPickup.update(dtAsSeconds);


			/////สปอนการมีอยู่ของซอมบี้ 
			for (int i = 0; i < 100; i++)
			{
				for (int j = 0; j < numZombies; j++)
				{
					if (bullets[i].isInFlight()  )
					{
						
					}

				}
			}///////////////////////////

			

			// ถ้าเก็บ ammo pickup health pickup
			if (player.getPosition().intersects
			(healthPickup.getPosition()) && healthPickup.isSpawned())
			{
				player.increaseHealthLevel(healthPickup.gotIt());

				pickup.play();

			}

			// ถ้าเก็บ ammo pickup
			if (player.getPosition().intersects
			(ammoPickup.getPosition()) && ammoPickup.isSpawned())
			{
				bulletsSpare += ammoPickup.gotIt();

				reload.play();

			}

			// size เลือด
			healthBar.setSize(Vector2f(player.getHealth() * 3, 70));

			// เฟรม
			timeSinceLastUpdate += dt;

			framesSinceLastHUDUpdate++;//อันนี้เป็นการตั้งให้เฟรมไม่กระพริบถี่เกินไป อันล่างด้วย

			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) 
			{
				// แสดงอักษรที่เปลี่ยนแปลงได้ (ข้อความที่มีค่า) ใช้ฟังก์ชั่น std::stringstream
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream sslastScore;

				// ข้อความที่เป็นค่าคงที่ แต่แสดงเฉยๆ
				// แสดง ตัวเลขกระสุน
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				// แสดง score
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());

				// แสดง lastscore
				sslastScore << "Last Score:" << lastScore;
				lastScoreText.setString(sslastScore.str());

				// แสดง Hight score
				ssHiScore << "Hight score:" << hiScore; //ตอนเล่น
				hiScoreText.setString(ssHiScore.str());



				framesSinceLastHUDUpdate = 0;
				timeSinceLastUpdate = Time::Zero;
			}
			// ////

		}/////// //

		 ///*
		 /*****///
		 ///วาดๆๆ///
		 /*****///
		 //*/

		if (state == State::PLAYING)  //6.หลังจากเซ็ทฟังก์ชั่น นู้นนี่นั้นเสร็จ จะมาเคลียร์เพื่อวาดละ จะเข้าloopนี้ต่อ เพื่อเซ็ทให้ วาด พื้นหลัง เมาส์ view ต่างๆ
		{
			window.clear();

			musicHomeGame.pause();
			window.setView(mainView);

			// background
			window.draw(background, &textureBackground);

			

			// player
			window.draw(player.getSprite());

			// item
			if (ammoPickup.isSpawned())
			{
				window.draw(ammoPickup.getSprite());
			}
			if (healthPickup.isSpawned())
			{
				window.draw(healthPickup.getSprite());
			}

			// เป้าเมาส์
			window.draw(spriteCrosshair);

			// view
			window.setView(hudView);


			window.draw(spriteAmmoIcon);
			window.draw(ammoText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(lastScoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
		}

		if (state == State::LEVELING_UP)
		{
            musicHomeGame.play();
			window.draw(spriteLevelUp);
			window.draw(levelUpText);
			
		}

		if (state == State::PAUSED)
		{
			
			window.draw(pausedText);
			
		}

		if (state == State::GAME_OVER)// 2. เพราะตั้ง staste เป็น  GAME_OVER เลยเข้าloop นี้ก่อนเลยอันดับแรก
		{
           
			State state = State::PLAYING; 
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(lastScoreText);
			//พอบอกให้มันเซ็ททุกอย่างจบ ก็จะออกลูปเพื่อไป window.display
		}

		window.display();  // 3. มันจะวาดทุกสิ่งทุกอย่างเลยที่เซ็ทมา  7.ก็วาดสิ่งที่เซ็ท ละวนไปเรื่อยๆ 

	}// End game loop วนไป isOpen

	return 0;
}