﻿#include "pch.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "ZombieShoot.h"
#include "Player.h"
#include "TextureHolder.h"
#include "Bullet.h"
#include "Item.h"
#include <cstdlib>
#include <ctime>

using namespace std;

using namespace sf;

int random_between(unsigned short int min, unsigned short int max)
{
	unsigned short int r = (rand() / (float)RAND_MAX) * (max + 1) + min;
	return r > max ? max : r;
}

int main()
{

	// เรียกใช้ TextureHolder
	TextureHolder Main;

	// สถานะเกม ใน State หยุดได้ อัพเวล จบเกม เล่น
	enum class State { PAUSED, LEVELING_UP, GAME_OVER, PLAYING ,FRIST_GAME,HIGH_SCORE,HOW_TO_PLAY,BEFORE_OVER};
	// สถานะไหนตอนเริ่ม

	State state = State::FRIST_GAME; // 1.ตอนเริ่ม state จะเป็นดังนี้


	// ขนาด window
	Vector2f resolution;
	resolution.x = VideoMode::getDesktopMode().width; // กว้าง
	resolution.y = VideoMode::getDesktopMode().height; //สูง

	RenderWindow window(VideoMode(resolution.x, resolution.y),
		"Zombie Army : DEAD LAB", Style:: Fullscreen);/////////////////////////////////////////

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
		"graphics/background_sheetReal.png");

	// zombies
	int numZombies;
	int numZombiesAlive;
	Zombie* zombies = NULL;
	

	// กระสุน
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
	short int score = 0;
	short int hiScore = 1200;
	short int hiScore2 = 0;
	short int hiScore3 = 0;
	short int hiScore4 = 0;
	short int hiScore5 = 0;
	short int lastScore = 0;

	short int h1 = 0;
	short int h2 = 0;
	short int h3 = 0;
	short int h4 = 0;
	short int h5 = 0;

	// หน้า home  พื้นหลัง
	Sprite spriteGameOver;
	Texture textureGameOver = TextureHolder::GetTexture("graphics/SjAw8B5.jpg");
	spriteGameOver.setTexture(textureGameOver);
	spriteGameOver.setPosition(0, 0);

	// หน้า howToPlay
	Sprite Howplay;
	Texture texturespriteHowplay = TextureHolder::GetTexture("graphics/howplay.jpg");
	Howplay.setTexture(texturespriteHowplay);
	Howplay.setPosition(0, 0);

	// หน้า frist
	Sprite spriteFrist;
	Texture texturespriteFrist = TextureHolder::GetTexture("graphics/frist.png");
	spriteFrist.setTexture(texturespriteFrist);
	spriteFrist.setPosition(0, 0);

	// Logo
	Sprite spriteLogo;
	Texture textureLogo = TextureHolder::GetTexture("graphics/logo.png");
	spriteLogo.setTexture(textureLogo);
	spriteLogo.setPosition(585, 15);

	// หน้า score  
	Sprite spriteScoreBoard;
	Texture textureScoreBoard = TextureHolder::GetTexture("graphics/ScoreS.png");
	spriteScoreBoard.setTexture(textureScoreBoard);
	spriteScoreBoard.setPosition(0, 0);

	//หน้า level up
	Sprite spriteLevelUp;
	Texture textureLevelUp = TextureHolder::GetTexture("graphics/leup.jpg");
	spriteLevelUp.setTexture(textureLevelUp);
	spriteLevelUp.setPosition(0, 0);

	// view
	View hudView(sf::FloatRect(0, 0, resolution.x, resolution.y));

	//  sprite กระสุน icon
	Sprite spriteAmmoIcon;
	Texture textureAmmoIcon = TextureHolder::GetTexture("graphics/ammo_icon.png");
	spriteAmmoIcon.setTexture(textureAmmoIcon);
	spriteAmmoIcon.setPosition(1650, 973);

	//  sprite WASD
	Sprite spriteWASD;
	Texture textureWASD = TextureHolder::GetTexture("graphics/howToPlay.png");
	spriteWASD.setTexture(textureWASD);
	spriteWASD.setPosition(450, 355);

	//  sprite R
	Sprite spriteR;
	Texture textureR = TextureHolder::GetTexture("graphics/mouse.png");
	spriteR.setTexture(textureR);
	spriteR.setPosition(440, 705);

	


	// Load font
	Font font;
	font.loadFromFile("fonts/youmurdererbb.ttf");

	// คนสร้าง
	Text tong;
	tong.setFont(font);
	tong.setCharacterSize(55);
	tong.setFillColor(Color::White);
	tong.setOutlineColor(sf::Color::Magenta);
	tong.setOutlineThickness(1);
	tong.setPosition(665, 1000);
	tong.setString("MR.Paramee Chumsri  63010598");


	// Game Over
	Text gameOverText;
	gameOverText.setFont(font);
	gameOverText.setCharacterSize(125);
	gameOverText.setFillColor(Color::Yellow);
	gameOverText.setOutlineColor(sf::Color::Black);
	gameOverText.setOutlineThickness(8);
	gameOverText.setPosition(250, 650);
	gameOverText.setString(" | T | - play games");

	// viewScoreBoard
	Text viewScoreBoardText;
	viewScoreBoardText.setFont(font);
	viewScoreBoardText.setCharacterSize(125);
	viewScoreBoardText.setFillColor(Color::Yellow);
	viewScoreBoardText.setOutlineColor(sf::Color::Black);
	viewScoreBoardText.setOutlineThickness(8);
	viewScoreBoardText.setPosition(250, 850);
	viewScoreBoardText.setString(" | U | - Score Board");

	// SCOREBOARD
	Text SCOREBOARDText;
	SCOREBOARDText.setFont(font);
	SCOREBOARDText.setCharacterSize(125);
	SCOREBOARDText.setFillColor(Color::Green);
	SCOREBOARDText.setOutlineColor(sf::Color::White);
	SCOREBOARDText.setOutlineThickness(4);
	SCOREBOARDText.setPosition(78, 25);
	SCOREBOARDText.setString("ScoreBoard");

	// HOWTOPLAY
	Text HOWTOPLAYText;
	HOWTOPLAYText.setFont(font);
	HOWTOPLAYText.setCharacterSize(125);
	HOWTOPLAYText.setFillColor(Color::White);
	HOWTOPLAYText.setOutlineColor(sf::Color::Red);
	HOWTOPLAYText.setOutlineThickness(4);
	HOWTOPLAYText.setPosition(78, 25);
	HOWTOPLAYText.setString("How To Play");

	
	// welcome
	Text welcomeText;
	welcomeText.setFont(font);
	welcomeText.setCharacterSize(75);
	welcomeText.setFillColor(Color::Black);
	welcomeText.setOutlineColor(sf::Color::Red);
	welcomeText.setOutlineThickness(1);
	welcomeText.setPosition(810, 365);
	welcomeText.setString("WELCOME...");

	// esc
	Text escText;
	escText.setFont(font);
	escText.setCharacterSize(55);
	escText.setFillColor(Color::Yellow);
	escText.setOutlineColor(sf::Color::Black);
	escText.setOutlineThickness(8);
	escText.setPosition(20, 1000);
	escText.setString(" | Esc | - Exit");

	// HowtoplayInGame
	Text HowtoplayInGame;
	HowtoplayInGame.setFont(font);
	HowtoplayInGame.setCharacterSize(55);
	HowtoplayInGame.setFillColor(Color::White);
	HowtoplayInGame.setOutlineColor(sf::Color::Red);
	HowtoplayInGame.setOutlineThickness(3);
	HowtoplayInGame.setPosition(865, 340);
	HowtoplayInGame.setString(" |W| - Move to the top\n |A| - Move to the Left\n |S| - Move to the Down\n |D| - Move to the Right\n |R| - Reload\n\n\n\n |Left Mouse| - Shoot");

	// HowToPlay
	Text howToPlayText;
	howToPlayText.setFont(font);
	howToPlayText.setCharacterSize(125);
	howToPlayText.setFillColor(Color::Yellow);
	howToPlayText.setOutlineColor(sf::Color::Black);
	howToPlayText.setOutlineThickness(8);
	howToPlayText.setPosition(250, 750);
	howToPlayText.setString(" | O | - How to play");


	// หยุด
	Text pausedText;
	pausedText.setFont(font);
	pausedText.setCharacterSize(155);
	pausedText.setFillColor(Color::Green);
	pausedText.setOutlineColor(sf::Color::White);
	pausedText.setOutlineThickness(2);
	pausedText.setPosition(400, 400);
	pausedText.setString("Press  |P| \nto Continue or\nPress  |Esc| to Exit ");

	// หยุด
	Text beforeGameOverText;
	beforeGameOverText.setFont(font);
	beforeGameOverText.setCharacterSize(155);
	beforeGameOverText.setFillColor(Color::Green);
	beforeGameOverText.setOutlineColor(sf::Color::White);
	beforeGameOverText.setOutlineThickness(2);
	beforeGameOverText.setPosition(300, 400);
	beforeGameOverText.setString("GAME OVER \n   Press  |C| \nto Continue  ");



	// หน้า Levelling up
	Text levelUpText;
	levelUpText.setFont(font);
	levelUpText.setCharacterSize(80);
	levelUpText.setFillColor(Color::White);
	levelUpText.setOutlineColor(sf::Color::Black);
	levelUpText.setOutlineThickness(3);
	levelUpText.setPosition(150, 250); //ตำแหน่งตัวอักษร
	std::stringstream levelUpStream;
	levelUpStream <<
		"   |G| --> + Rate of fire" <<
		"\n   |H| --> + Clip size(next reload)" <<
		"\n   |J| --> + Max health 10 o/o" <<
		"\n   |B| --> + Run speed 10 o/o" <<
		"\n   |N| --> + Health pickups 5 o/o" <<
		"\n   |M| --> + Ammo pickups 5 o/o";
	levelUpText.setString(levelUpStream.str());
	

	

	// Ammo
	Text ammoText;
	ammoText.setFont(font);
	ammoText.setCharacterSize(75);
	ammoText.setFillColor(Color::Yellow);
	ammoText.setOutlineColor(sf::Color::Black);
	ammoText.setOutlineThickness(2);
	ammoText.setPosition(1750, 980);


	// Score
	Text scoreText;
	scoreText.setFont(font);
	scoreText.setCharacterSize(55);
	scoreText.setFillColor(Color::White);
	scoreText.setOutlineColor(sf::Color::Green);
	scoreText.setOutlineThickness(1);
	scoreText.setPosition(720, 5);

	// Last Score
	Text lastScoreText;
	lastScoreText.setFont(font);
	lastScoreText.setCharacterSize(55);
	lastScoreText.setFillColor(Color::White);
	lastScoreText.setOutlineColor(sf::Color::Green);
	lastScoreText.setOutlineThickness(1);
	lastScoreText.setPosition(20, 100);
	

	// ดึง high score จาก text file/
	std::ifstream inputFile1("gamedata/Score/scores1.txt");
	if (inputFile1.is_open())
	{
		inputFile1 >> hiScore;
		inputFile1.close();
	}
	std::ifstream inputFile2("gamedata/Score/scores2.txt");
	if (inputFile2.is_open())
	{
		inputFile2 >> hiScore2;
		inputFile2.close();
	}
	std::ifstream inputFile3("gamedata/Score/scores3.txt");
	if (inputFile3.is_open())
	{
		inputFile3 >> hiScore3;
		inputFile3.close();
	}
	std::ifstream inputFile4("gamedata/Score/scores4.txt");
	if (inputFile4.is_open())
	{
		inputFile4 >> hiScore4;
		inputFile4.close();
	}
	std::ifstream inputFile5("gamedata/Score/scores5.txt");         ////////////////////
	if (inputFile5.is_open())
	{
		inputFile5 >> hiScore5;
		inputFile5.close();
	}

	////////////////////////////////

	std::string name1 = {'B','e','a','T','o','n','g'};
	std::string name2 = {};
	std::string name3 = {};
	std::string name4 = {};
	std::string name5 = {};

	char showtext = {};
	// ดึง name จาก text file/
	std::ifstream inputFileN("gamedata/Name/name.txt");
	if (inputFileN.is_open())
	{
		inputFileN >> showtext;
		inputFileN.close();
	}

	std::ifstream fileName5;
	fileName5.open("gamedata/Name/name5.txt");
	if (fileName5.is_open())
	{
		std::cout << "name5 is open" << endl;
		fileName5 >> name5;
		fileName5.close();
	}

	std::ifstream fileName4;
	fileName4.open("gamedata/Name/name4.txt");
	if (fileName4.is_open())
	{
		std::cout << "name4 is open" << endl;
		fileName4 >> name4;
		fileName4.close();
	}

	std::ifstream fileName3;
	fileName3.open("gamedata/Name/name3.txt");
	if (fileName3.is_open())
	{
		std::cout << "name3 is open" << endl;
		fileName3 >> name3;
		fileName3.close();
	}

	std::ifstream fileName2;
	fileName2.open("gamedata/Name/name2.txt");
	if (fileName2.is_open())
	{
		std::cout << "name2 is open" << endl;
		fileName2 >> name2;
		fileName2.close();
	}



	sf::String input;
	sf::String sentence;
	std::string nameString;
	sf::Text text(sentence, font, 80);
	text.setPosition(825, 525);
	text.setFillColor(Color::Red);
	text.setOutlineColor(sf::Color::White);
	text.setOutlineThickness(1);
	text.setStyle(sf::Text::Underlined);

	///////////////////////////////////////////
	Text EnternameText;
	EnternameText.setFont(font);
	EnternameText.setCharacterSize(55);
	EnternameText.setFillColor(Color::White);
	EnternameText.setPosition(460, 545);
	EnternameText.setOutlineColor(sf::Color::Red);
	EnternameText.setOutlineThickness(0.5);
	std::stringstream SsEnternameTexts;
	SsEnternameTexts << "Enter your name :"; //ตอนหน้าfrist
	EnternameText.setString(SsEnternameTexts.str());


	//หน้าเรียง score
	Text listScoreText;
	listScoreText.setFont(font);
	listScoreText.setCharacterSize(70);
	listScoreText.setFillColor(Color::Green);
	listScoreText.setOutlineColor(sf::Color::White);
	listScoreText.setOutlineThickness(1);
	listScoreText.setPosition(650, 395);
	std::stringstream listScoreStream;
	listScoreStream <<
		"   1. " << name1 <<"   score : "<< hiScore <<
		"\n\n   2. " << name2 << "   score : " << hiScore2 <<
		"\n\n   3. " << name3 << "   score : " << hiScore3 <<
		"\n\n   4. " << name4 << "   score : " << hiScore4 <<
		"\n\n   5. " << name5 << "    score : " << hiScore5;
	listScoreText.setString(listScoreStream.str());
	

	// Hight Score
	Text hiScoreText;
	hiScoreText.setFont(font);
	hiScoreText.setCharacterSize(55);
	hiScoreText.setFillColor(Color::Green);
	hiScoreText.setOutlineColor(sf::Color::Red);
	hiScoreText.setOutlineThickness(1);
	hiScoreText.setPosition(20, 5);
	std::stringstream s;
	s << "Hight Score:" << hiScore; //ตอนหน้าhome
	hiScoreText.setString(s.str());

	// Name show
	Text nameShowText;
	nameShowText.setFont(font);
	nameShowText.setCharacterSize(55);
	nameShowText.setFillColor(Color::Red);
	nameShowText.setOutlineColor(sf::Color::White);
	nameShowText.setOutlineThickness(1);
	nameShowText.setPosition(485, 5);
	std::stringstream sNameShow;
	
	
	// จำนวน Zombies 
	Text zombiesRemainingText;
	zombiesRemainingText.setFont(font);
	zombiesRemainingText.setCharacterSize(55);
	zombiesRemainingText.setFillColor(Color::White);
	zombiesRemainingText.setOutlineColor(sf::Color::Red);
	zombiesRemainingText.setOutlineThickness(1);
	zombiesRemainingText.setPosition(175, 980);
	zombiesRemainingText.setString("");
	
	// ด่าน
	int wave = 0;
	Text waveNumberText;
	waveNumberText.setFont(font);
	waveNumberText.setCharacterSize(55);
	waveNumberText.setFillColor(Color::White);
	waveNumberText.setOutlineColor(sf::Color::Red);
	waveNumberText.setOutlineThickness(1);
	waveNumberText.setPosition(25, 980);
	waveNumberText.setString("");

	// แถบเลือด
	RectangleShape healthBar;
	healthBar.setFillColor(Color::Red);
	healthBar.setOutlineColor(sf::Color::Black);
	healthBar.setOutlineThickness(5);
	healthBar.setPosition(800, 980);

	
	int framesSinceLastHUDUpdate = 0;

	Time timeSinceLastUpdate;
	//frames เกม 
	int fpsMeasurementFrameInterval = 1000;
	// uplevel
	SoundBuffer levelUpSoundBuffer;
	levelUpSoundBuffer.loadFromFile("sound/fileSound/levelUp.wav");
	Sound levelUpSound;
	levelUpSound.setBuffer(levelUpSoundBuffer);
	//BRUH
	SoundBuffer BRUHBuffer;
	BRUHBuffer.loadFromFile("sound/fileSound/levelUp.wav");
	Sound BRUHSound;
	BRUHSound.setBuffer(BRUHBuffer);
	

	// namePass
	SoundBuffer namePassBuffer;
	namePassBuffer.loadFromFile("sound/fileSound/ZombieFristgame.wav");
	Sound namePassSound;
	namePassSound.setBuffer(namePassBuffer);

	// nameFail
	SoundBuffer nameFailBuffer;
	nameFailBuffer.loadFromFile("sound/fileSound/nope.wav");
	Sound nameFailSound;
	nameFailSound.setBuffer(nameFailBuffer);

	// PlayerDead
	SoundBuffer PlayerDeadBuffer;
	PlayerDeadBuffer.loadFromFile("sound/fileSound/misfail.wav");
	Sound PlayerDeadSound;
	PlayerDeadSound.setBuffer(PlayerDeadBuffer);

	// ScoreBoard
	SoundBuffer ScoreBoardBuffer;
	ScoreBoardBuffer.loadFromFile("sound/fileSound/seeScore.wav");
	Sound ScoreBoardSound;
	ScoreBoardSound.setBuffer(ScoreBoardBuffer);

	// stopSound
	SoundBuffer stopSoundBuffer;
	stopSoundBuffer.loadFromFile("sound/fileSound/racche.wav");
	Sound EstopSound;
	EstopSound.setBuffer(stopSoundBuffer);

	// ZombieDead
	SoundBuffer ZombieDeadBuffer;
	ZombieDeadBuffer.loadFromFile("sound/fileSound/soundZombieDie.wav");
	Sound ZombieDead;
	ZombieDead.setBuffer(ZombieDeadBuffer);
	ZombieDead.setVolume(55.f);

	// เสียงตอนโดนตี
	SoundBuffer hitBuffer;
	hitBuffer.loadFromFile("sound/fileSound/soundZombieHit.wav");
	Sound hit;
	hit.setBuffer(hitBuffer);

	// เสียงตอนยิงโดน
	SoundBuffer splatBu;
	splatBu.loadFromFile("sound/fileSound/bloodhitting.wav");
	Sound splat;
	splat.setBuffer(splatBu);

	
	SoundBuffer splatBuf;
	splatBuf.loadFromFile("sound/fileSound/littlerobot.wav");
	Sound splat1;
	splat1.setBuffer(splatBuf);
	

	SoundBuffer splatBuffer2;
	splatBuffer2.loadFromFile("sound/fileSound/soundZombie.wav");
	Sound splat2;
	splat2.setBuffer(splatBuffer2);
	splat2.setVolume(55.f);

	SoundBuffer splatBuffer3;
	splatBuffer3.loadFromFile("sound/fileSound/soundZombie2.wav");
	Sound splat3;
	splat3.setBuffer(splatBuffer3);
	splat3.setVolume(55.f);

	SoundBuffer splatBuffer4;
	splatBuffer4.loadFromFile("sound/fileSound/zombieDead.wav");
	Sound splat4;
	splat4.setBuffer(splatBuffer4);
	splat4.setVolume(55.f);
	// เสียงตอนยิง
	SoundBuffer shootBuffer;
	shootBuffer.loadFromFile("sound/shoot.wav");
	Sound shoot;
	shoot.setBuffer(shootBuffer);
	shoot.setVolume(55.f);

	// เสียงรีโหลด
	SoundBuffer reloadBuffer;
	reloadBuffer.loadFromFile("sound/reload1.wav");
	Sound reload;
	reload.setBuffer(reloadBuffer);
	reload.setVolume(55.f);

	// เสียงตอนหมด
	SoundBuffer reloadFailedBuffer;
	reloadFailedBuffer.loadFromFile("sound/reload_failed.wav");
	Sound reloadFailed;
	reloadFailed.setBuffer(reloadFailedBuffer);


	// เสียงตอนเก็บ
	SoundBuffer pickupBuffer;
	pickupBuffer.loadFromFile("sound/fileSound/health.wav");
	Sound pickup;
	pickup.setBuffer(pickupBuffer);

	// เสียงเริ่มเกม
	SoundBuffer startGameBuffer;
	startGameBuffer.loadFromFile("sound/counterStrike.wav");
	Sound startGame;
	startGame.setBuffer(startGameBuffer);
	startGame.setVolume(55.f);

	Music musicHomeGame;
	musicHomeGame.setLoop(true);
	musicHomeGame.openFromFile("sound/ShooterZombieArmy.wav");
	musicHomeGame.setVolume(15.f);
	musicHomeGame.play();
	musicHomeGame.getLoop();
	Music musicInGame;
	
	musicInGame.openFromFile("sound/ZombieInGame.wav");
	musicInGame.setVolume(20.f);
	musicInGame.setLoop(true);
	musicInGame.getLoop();
	musicInGame.stop();
	
	
 ///////////////////////////////////////

	// main game loop
	while (window.isOpen())
	{
		if (musicHomeGame.getStatus()== Music :: Status ::Stopped) {
			musicHomeGame.stop();
			if (musicHomeGame.getStatus() == Music::Status::Stopped) {
				musicHomeGame.play();
				continue;
			}
		}
		if((musicInGame.getStatus() == Music::Status::Stopped)|| (musicInGame.getStatus() == Music::Status::Paused)) {
			musicInGame.stop();
			if (musicInGame.getStatus() == Music::Status::Paused) {
				musicInGame.play();
				continue;
			}
		}
		
		
		
		// เริ่มมมมมมมมมมมมมมมมมม

		// player กับ เกม

		sf :: Event event;
		while (window.pollEvent(event))
		{
			
			
			if (state == State::FRIST_GAME) {
				if (event.type == sf::Event::TextEntered)
				{
					if (event.text.unicode < 128)
					{
						//std::cout << "Text entered: " << (event.text.unicode) << std::endl;
						if (event.text.unicode == static_cast<sf::Uint32>(8) && input.getSize() > 0)
						{
							input.erase(input.getSize() - 1);
							nameString.erase(nameString.size() - 1);
							
						}
						else
						{
							if (input.getSize() < 13 && event.text.unicode != 13)
							{
								if (event.text.unicode >= 97 && event.text.unicode <= 122)
								{
									event.text.unicode -= 32;
								}
								input += event.text.unicode;
								nameString += event.text.unicode;
							}
						}
						
					}
					text.setString(input);
				}
				if (event.key.code == Keyboard::Enter &&
					state == State::FRIST_GAME && input.getSize() > 0)
				{
					ScoreBoardSound.play();
					namePassSound.play();
					std::ofstream file;
					file.open("gamedata/Name/name.txt");
					file << nameString;
					file.close();
					state = State::GAME_OVER;
					std::cout << nameString << endl;



				}
				else if (event.key.code == Keyboard::Enter &&
					state == State::FRIST_GAME && input.getSize() <= 0) {
					nameFailSound.play();
				}
				else if (event.key.code == Keyboard::Escape &&
					state == State::FRIST_GAME ) {
					window.close();
				}

				
			}
				
			
				

			if (event.type == Event::KeyPressed)
			{
				//before_Over
				if (event.key.code == Keyboard::C &&
					state == State::BEFORE_OVER)
				{
					ScoreBoardSound.play();
					state = State::GAME_OVER;
					continue;

				}
				// howToPlay ///
					if (event.key.code == Keyboard::O &&
						state == State::GAME_OVER)
					{
						ScoreBoardSound.play();
						state = State::HOW_TO_PLAY;
						continue;

					}
				// สกอร์ ///
				if (event.key.code == Keyboard::O &&
					state == State::HOW_TO_PLAY)
				{
					ScoreBoardSound.play();
					state = State::GAME_OVER;
					clock.restart();
				}

				// สกอร์ ///
				if (event.key.code == Keyboard::U &&
					state == State::GAME_OVER)
				{
					ScoreBoardSound.play();
					state = State::HIGH_SCORE;
					continue;

				}
				// สกอร์ ///
				if (event.key.code == Keyboard::U &&
					state == State::HIGH_SCORE)
				{
					ScoreBoardSound.play();
					state = State::GAME_OVER;
					clock.restart();
				}
				// หยุดเกมตอนเล่น  ///
				if (event.key.code == Keyboard::P &&
					state == State::PLAYING)
				{
					EstopSound.play();
					musicInGame.pause();
					musicHomeGame.play();
					state = State::PAUSED;

				}

				// รีดฟรมตอนหยุด เล่นต่อ
				else if (event.key.code == Keyboard::P &&
					state == State::PAUSED)
				{
					musicInGame.play();
					state = State::PLAYING;
					clock.restart();
				}

				// ทำเมื่อเกมจบ
				else if (event.key.code == Keyboard::T &&
					state == State::GAME_OVER)
				{
				
				state = State::LEVELING_UP;
				
				wave = 0;
				lastScore = score;
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
		     }

				if (state == State::PLAYING)
				{
					 //////////////////////////////////////////////////////////////

					// Reload
					if (event.key.code == Keyboard::R) {
						if (gameTimeTotal.asMilliseconds()
							- lastPressed.asMilliseconds() > 1050) {
							lastPressed = gameTimeTotal;
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

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&&(numZombiesAlive != 0))
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
			if ((wave ==0 )&& (score ==0))
			{
			
				state = State::PLAYING;
			}
			// Handle the player levelling up
			if ((event.key.code == Keyboard::G)&& (state == State::LEVELING_UP))
			{
				// ยิงรัวขึ้น
				fireRate++;
				state = State::PLAYING;
			}

			if ((event.key.code == Keyboard::H) && (state == State::LEVELING_UP))
			{
				// เพิ่มตัวเก็บกระสุน
				reClipSize += reClipSize;
				state = State::PLAYING;
			}

			if ((event.key.code == Keyboard::J) && (state == State::LEVELING_UP))
			{
				// เพิ่มขีดจำกัดเลือด
				player.upgradeHealth();
				state = State::PLAYING;
			}

			if ((event.key.code == Keyboard::B) && (state == State::LEVELING_UP))
			{
				// เพิ่มความเร็ว
				player.upgradeSpeed();
				state = State::PLAYING;
			}
			//เพิ่มค่าไอเท็มเลือด
			if ((event.key.code == Keyboard::N) && (state == State::LEVELING_UP))
			{
				healthPickup.upgrade();
				state = State::PLAYING;
			}
			//เพิ่มค่าสุ่มกระสุน
			if ((event.key.code == Keyboard::M) && (state == State::LEVELING_UP))
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
				numZombies = 5 * wave;

				
				// //////////////////////////////////////////////////////////
				delete[] zombies;
				zombies = createHorde(numZombies, arena);
				numZombiesAlive = numZombies;
				
				numZombiesAlive = numZombies;

				startGame.play();
				musicInGame.play();


				clock.restart();
			}
		}//Level


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

			//////////////////////////////////////////////////////////////////////
			for (int i = 0; i < numZombies; i++)
			{
				if (zombies[i].isAlive())
				{
					zombies[i].update(dt.asSeconds(), playerPosition);
				}
			}

			

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
					if (bullets[i].isInFlight() &&
						zombies[j].isAlive())
						
					{
						if (bullets[i].getPosition().intersects        ///////////////////////////////////////////////////////////////////////////////////////////////////
						(zombies[j].getPosition()))
						{
				
							// Stop the bullet
							bullets[i].stop();

							//  kill 
							if (zombies[j].hit()) {

								
								
								ZombieDead.play();                        ////////////////////////////////////////////////////////////////////////////////////////////////
								score += 10;
								
								if (score >= hiScore)
								{
									hiScore = score;
								}
								numZombiesAlive--;

								
							}
							
							std::cout << j << endl;
							if ( ((j%3) == 0)||j==0)
							{
								
								std::cout << "0" << endl;
								
                              splat.play(); 
							  continue;
							}
							else if ( ((j % 5) == 0) || j == 1)
							{
								
								std::cout << "1" << endl;
								
								splat1.play();
								continue;
							}
							else if ( ((j % 7) == 0) || j == 2)
							{
								lastPressed = gameTimeTotal;
								std::cout << "2" << endl;
								
								splat2.play();
								continue;
							}
							else if (((j % 11) == 0) || j==3)
							{
								lastPressed = gameTimeTotal;
								std::cout << "3" << endl;
								
								splat3.play();
								continue;
							}
							else if ( j !=0)
							{
								lastPressed = gameTimeTotal;
								std::cout << "4" << endl;
								
								splat4.play();
								continue;
							}
							

							
							

						}
					}

				}
				if (numZombiesAlive == 0) {
					
					
					levelUpSound.play();
				

					if (sf::Mouse::isButtonPressed(sf::Mouse::Left)&& (framesSinceLastHUDUpdate == 0))
					{
						state = State::LEVELING_UP;

					}
				
			     }// End zombie being shot
			// zombies are dead 
			



			}
			// Have any zombies touched the player			
			for (int i = 0; i < numZombies; i++)
			{
				if (player.getPosition().intersects
				(zombies[i].getPosition()) && zombies[i].isAlive())
				{

					if (player.hit(gameTimeTotal)&& (gameTimeTotal.asMilliseconds()
						- lastPressed.asMilliseconds() > 500))
					{
						lastPressed = gameTimeTotal;
							// More here later
						hit.play();
					}
						

					if (player.getHealth() <= 0)
					{      //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						PlayerDeadSound.play();
						lastScore = score;
						/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
						std::ostringstream ssListScore;
						//////////////////////////////////////
						if (score > hiScore5) {
							if ((score > hiScore5) && (score <= hiScore4)) {
								hiScore5 = score;
								
								fstream outputFile5;
								outputFile5.open("gamedata/Score/scores5.txt", ios::out);
								if (outputFile5.is_open()) {
									std::cout << "file is Open scores5" << std::endl;

									outputFile5 << hiScore5 << endl;
									
		
									outputFile5.close();
								}



								std::fstream file;
								file.open("gamedata/Name/name.txt");
								file >> name5;
								file.close();

								std::cout << nameString << endl;

								std::ofstream fileName5;
								fileName5.open("gamedata/Name/name5.txt");
								if (fileName5.is_open()) {
									std::cout << "file is fileName5 Open" << std::endl;
									fileName5 << nameString;
									fileName5.close();
								}


								ssListScore <<
									"   1. " << name1 << "   score : " << hiScore <<
									"\n\n   2. " << name2 << "   score : " << hiScore2 <<
									"\n\n   3. " << name3 << "   score : " << hiScore3 <<
									"\n\n   4. " << name4 << "   score : " << hiScore4 <<
									"\n\n  5. " << name5 << "    score : " << hiScore5;
								listScoreText.setString(ssListScore.str());                                   ///////////////////////////////////////////////////////////

								state = State::BEFORE_OVER;

							}
							if ((score > hiScore5) && (score >= hiScore4) && (score < hiScore3)) {
								h5 = hiScore4;
								hiScore5 = h5;
								hiScore4 = score;
								fstream outputFile4;
								outputFile4.open("gamedata/Score/scores4.txt");
								if (outputFile4.is_open()) {
									std::cout << "file is Openscores4" << std::endl;
									outputFile4 << hiScore4;
									outputFile4.close();
								}

								std::fstream file;
								file.open("gamedata/Name/name.txt");
								file >> name4;
								file.close();

								std::cout << nameString << endl;

								std::ofstream fileName4;
								fileName4.open("gamedata/Name/name4.txt");
								if (fileName4.is_open()) {
									std::cout << "file is fileName4 Open" << std::endl;
									fileName4 << nameString;
									fileName4.close();
								}


								ssListScore <<
									"   1. " << name1 << "   score : " << hiScore <<
									"\n\n   2. " << name2 << "   score : " << hiScore2 <<
									"\n\n   3. " << name3 << "   score : " << hiScore3 <<
									"\n\n   4. " << name4 << "   score : " << hiScore4 <<
									"\n\n  5. " << name5 << "    score : " << hiScore5;
								listScoreText.setString(ssListScore.str());




								state = State::BEFORE_OVER;

							}
							if ((score > hiScore5) && (score > hiScore4) && (score >= hiScore3) && (score < hiScore2)) {
								h4 = hiScore3;
								h5 = hiScore4;
								hiScore5 = h5;
								hiScore4 = h4;
								hiScore3 = score;
								fstream outputFile3;
								outputFile3.open("gamedata/Score/scores3.txt");
								if (outputFile3.is_open()) {
									std::cout << "file is Openscores3" << std::endl;
									outputFile3 << hiScore3;
									outputFile3.close();
								}


								std::fstream file;
								file.open("gamedata/Name/name.txt");
								file >> name3;
								file.close();

								std::cout << nameString << endl;

								std::ofstream fileName3;
								fileName3.open("gamedata/Name/name3.txt");
								if (fileName3.is_open()) {
									std::cout << "file is fileName3 Open" << std::endl;
									fileName3 << nameString;
									fileName3.close();
								}


								ssListScore <<
									"   1. " << name1 << "   score : " << hiScore <<
									"\n\n   2. " << name2 << "   score : " << hiScore2 <<
									"\n\n   3. " << name3 << "   score : " << hiScore3 <<
									"\n\n   4. " << name4 << "   score : " << hiScore4 <<
									"\n\n  5. " << name5 << "    score : " << hiScore5;
								listScoreText.setString(ssListScore.str());




								state = State::BEFORE_OVER;


							}
							if ((score > hiScore5) && (score > hiScore4) && (score > hiScore3) && (score >= hiScore2) && (score < hiScore)) {
								h3 = hiScore2;
								h4 = hiScore3;
								h5 = hiScore4;
								hiScore5 = h5;
								hiScore4 = h4;
								hiScore3 = h3;
								hiScore2 = score;
								fstream outputFile2;
								outputFile2.open("gamedata/Score/scores2.txt");
								if (outputFile2.is_open()) {
									std::cout << "file is Openscores2" << std::endl;
									outputFile2 << hiScore2;
									outputFile2.close();
								}

								std::fstream file;
								file.open("gamedata/Name/name.txt");
								file >> name2;
								file.close();

								std::cout << nameString << endl;

								std::ofstream fileName2;
								fileName2.open("gamedata/Name/name2.txt");
								if (fileName2.is_open()) {
									std::cout << "file is fileName2 Open" << std::endl;
									fileName2 << nameString;
									fileName2.close();
								}


								ssListScore <<
									"   1. " << name1 << "   score : " << hiScore <<
									"\n\n   2. " << name2 << "   score : " << hiScore2 <<
									"\n\n   3. " << name3 << "   score : " << hiScore3 <<
									"\n\n   4. " << name4 << "   score : " << hiScore4 <<
									"\n\n  5. " << name5 << "    score : " << hiScore5;
								listScoreText.setString(ssListScore.str());




								state = State::BEFORE_OVER;




							}
						}
						else {

						/*ssListScore <<
							"   1. " << name1 << "   score : " << hiScore <<
							"\n\n   2. " << name2 << "   score : " << hiScore2 <<
							"\n\n   3. " << name3 << "   score : " << hiScore3 <<
							"\n\n   4. " << name4 << "   score : " << hiScore4 <<
							"\n\n  5. " << name5 << "    score : " << hiScore5;
						listScoreText.setString(ssListScore.str());*/

						state = State::BEFORE_OVER;
						musicInGame.stop();
						musicHomeGame.play();
					}
					}
				}
			}// /////////////////////////////////////////////////////////////////////////////////////////////////////

			

			// ถ้าเก็บ  health pickup
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

			framesSinceLastHUDUpdate+=15;//อัพเดตเวลา 

			if (framesSinceLastHUDUpdate > fpsMeasurementFrameInterval) 
			{
				// แสดงอักษรที่เปลี่ยนแปลงได้ (ข้อความที่มีค่า) ใช้ฟังก์ชั่น std::stringstream
				std::stringstream ssWave;
				std::stringstream ssZombiesAlive;
				std::stringstream ssAmmo;
				std::stringstream ssScore;
				std::stringstream ssHiScore;
				std::stringstream sslastScore;
				std::stringstream sNameShow;

				

				// ข้อความที่เป็นค่าคงที่ แต่แสดงเฉยๆ
				// แสดง ตัวเลขกระสุน
				ssAmmo << bulletsInClip << "/" << bulletsSpare;
				ammoText.setString(ssAmmo.str());

				// แสดง score
				ssScore << "Score:" << score;
				scoreText.setString(ssScore.str());
				//wave
				ssWave << "Wave: " << wave;
				waveNumberText.setString(ssWave.str());

				//zombie
				ssZombiesAlive << "ZombieAlive: " << numZombiesAlive;
				zombiesRemainingText.setString(ssZombiesAlive.str());

				// แสดง lastscore
				sslastScore << "Last Score:" << lastScore;
				lastScoreText.setString(sslastScore.str());

				// แสดง Hight score
				ssHiScore << "Hight score:" << hiScore; //ตอนเล่น
				hiScoreText.setString(ssHiScore.str());

				// name
			    sNameShow << "\" " << nameString << " \"";
				nameShowText.setString(sNameShow.str());


				framesSinceLastHUDUpdate = 0;
				timeSinceLastUpdate = Time::Zero;
				
			}
			// /////

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

			// Draw the zombies
			for (int i = 0; i < numZombies; i++)///////////////////////////////////////////////////////////////////
			{
				window.draw(zombies[i].getSprite());
			}

			for (int i = 0; i < 100; i++)
			{
				if (bullets[i].isInFlight())
				{
					window.draw(bullets[i].getShape());
				}
			}

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
			window.draw(hiScoreText);
			window.draw(lastScoreText);
			window.draw(scoreText);
			window.draw(healthBar);
			window.draw(waveNumberText);
			window.draw(zombiesRemainingText);
			window.draw(nameShowText);
			//window.draw(spriteLogo);
			//window.draw(tong);

		}
		
		if (state == State::LEVELING_UP)
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				BRUHSound.play();
				window.close();

			}

			window.draw(spriteLevelUp);
			window.draw(levelUpText);
			window.draw(waveNumberText);
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(lastScoreText);
			window.draw(nameShowText);
			//window.draw(tong);

			
			
			
		}

		if (state == State::PAUSED)
		{
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				BRUHSound.play();
				window.close();
			}
			window.draw(spriteLogo);
			window.draw(pausedText);
			window.draw(tong);


			
		}

		if (state == State::GAME_OVER)//  GAME_OVER  111
		{
			State state = State::PLAYING; 
			if (Keyboard::isKeyPressed(Keyboard::Escape))
			{
				BRUHSound.play();
				
				window.close();
			}

			
			
			spriteLogo.setPosition(1150, 15);
			window.draw(spriteGameOver);
			window.draw(gameOverText);
			///////////////////////////////////////////////////////////////////////////////////////////
		
			/////////////////////////////////////////////////////////////////////////
			window.draw(scoreText);
			window.draw(hiScoreText);
			window.draw(lastScoreText);
			window.draw(nameShowText);
			window.draw(howToPlayText);
			window.draw(viewScoreBoardText);
			window.draw(escText);
			window.draw(spriteLogo);
			window.draw(tong);


			
		}
		
		if (state == State::FRIST_GAME) {
			window.draw(spriteFrist);
			window.draw(text);
			window.draw(EnternameText);
			window.draw(welcomeText);
			window.draw(spriteLogo);
			window.draw(tong);

			
			
			
		}
		if (state == State::HIGH_SCORE)
		{
			window.draw(spriteScoreBoard);
			window.draw(listScoreText);
			window.draw(SCOREBOARDText);
			window.draw(spriteLogo);
			window.draw(tong);



			
		}
		if (state == State::BEFORE_OVER)
		{
			
			window.draw(spriteLogo);
			window.draw(tong);
			window.draw(nameShowText);

			window.draw(hiScoreText);
			window.draw(scoreText);
			window.draw(beforeGameOverText);

			




		}
		if (state == State::HOW_TO_PLAY)
		{
			window.draw(Howplay);
			window.draw(spriteWASD);
			window.draw(spriteR);
			window.draw(HowtoplayInGame); 
			window.draw(HOWTOPLAYText);
			window.draw(spriteLogo);
			window.draw(tong);
		
			


		}
		
		window.display();  // 3. มันจะวาดทุกสิ่งทุกอย่างเลยที่เซ็ทมา  7.ก็วาดสิ่งที่เซ็ท ละวนไปเรื่อยๆ 
		window.clear();

	}// End game loop วนไป isOpen

	return 0;
}