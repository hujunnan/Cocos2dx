#include "GameScene.h"
#include <algorithm>

USING_NS_CC;

using namespace CocosDenshion;

Scene* GameScene::createScene() {
	// 'scene' is an autorelease object
	//auto scene = Scene::create();
	auto scene = Scene::createWithPhysics();
	//开启物理世界
	scene->getPhysicsWorld()->setAutoStep(false);
	// Debug 模式
	// 开启debug模式需要setAutoStep(true) 并注释掉update函数第一行
	//scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
	scene->getPhysicsWorld()->setGravity(Vec2(0, -300.0f));

	// 'layer' is an autorelease object
	auto layer = GameScene::create();

	layer->setPhysicsWorld(scene->getPhysicsWorld());

	// add layer as a child to scene
	scene->addChild(layer);

	// return the scene
	return scene;
}

void GameScene::setPhysicsWorld(PhysicsWorld * world)
{
	m_world = world;
}

bool GameScene::init() {
	if (!Layer::init()) {
		return false;
	}

	visibleSize = Director::getInstance()->getVisibleSize();
	IsPlayerLeft = false;
	IsPlayerRight = false;
	IsPlayerJump = false;
	angry = 100;
	max = 500;

	addSprite();	
	preloadMusic();   
	preloadAnimation();
	
	auto width = visibleSize.width, height = visibleSize.height;
	auto origin = Director::getInstance()->getVisibleOrigin();

	game = this;
	//hp条
	Sprite* sp0 = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(0, 320, 420, 47)));
	Sprite* sp = Sprite::create("hp.png", CC_RECT_PIXELS_TO_POINTS(Rect(610, 362, 4, 16)));
	//使用hp条设置progressBar
	pT = ProgressTimer::create(sp);
	pT->setScaleX(90);
	pT->setAnchorPoint(Vec2(0, 0));
	pT->setType(ProgressTimerType::BAR);
	pT->setBarChangeRate(Point(1, 0));
	pT->setMidpoint(Point(0, 1));
	pT->setPercentage(angry);
	pT->setPosition(Vec2(origin.x + 14 * pT->getContentSize().width, origin.y + visibleSize.height - 2 * pT->getContentSize().height));
	addChild(pT, 1);
	sp0->setAnchorPoint(Vec2(0, 0));
	sp0->setPosition(Vec2(origin.x + pT->getContentSize().width, origin.y + visibleSize.height - sp0->getContentSize().height));
	addChild(sp0, 0);
	if(Global::round == 1) addWall();
	else createRound();

	addTouchListener();
	addKeyboardListener();
	addContactListener();

	schedule(schedule_selector(GameScene::update), 0.01f, kRepeatForever, 0.1f);
	schedule(schedule_selector(GameScene::timeCountSchedule), 1.0f, kRepeatForever, 0);
	schedule(schedule_selector(GameScene::movePlaneSchedule), 0.06f, kRepeatForever, 0);
	return true;
}


void GameScene::preloadMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	audio->preloadBackgroundMusic("music/BGM.mp3");
	audio->preloadEffect("music/gameover.wav");
	audio->preloadEffect("music/getheart.wav");
	audio->preloadEffect("music/hit.wav");
	audio->preloadEffect("music/win.wav");

	audio->playBackgroundMusic("music/BGM.mp3", true);
}

void GameScene::createGreenplane(int i, int j)
{
	auto wall1 = Sprite::create("greenplane.png");
	wall1->setPosition(i, j);
	simplePlane.push_back(wall1);
	auto wallbody = PhysicsBody::createBox(wall1->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	wallbody->setCategoryBitmask(0xFFFFFFFF);
	wallbody->setCollisionBitmask(0xFFFFFFFF);
	wallbody->setContactTestBitmask(0xFFFFFFFF);
	wallbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	wall1->setPhysicsBody(wallbody);
	this->addChild(wall1);
}

void GameScene::createHeart(int i, int j)
{
	auto heart2 = Sprite::create("heart.png");
	heart2->setPosition(i, j);
	heart2->setScale(0.5, 0.5);
	heart2->setTag(3);
	hearts.push_back(heart2);
	auto heartbody = PhysicsBody::createBox(heart2->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	heartbody->setCategoryBitmask(0xFFFFFFFF);
	heartbody->setCollisionBitmask(0xFFFFFFFF);
	heartbody->setContactTestBitmask(0xFFFFFFFF);
	heartbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	heart2->setPhysicsBody(heartbody);
	this->addChild(heart2);
}

void GameScene::createPiggy(int i, int j)
{
	auto piggy1 = Sprite::createWithSpriteFrame(piggyframe);
	piggy1->setPosition(i, j);
	piggys.push_back(piggy1);
	piggy1->setScale(0.3, 0.3);
	piggy1->setTag(4);
	auto piggybody = PhysicsBody::createBox(piggy1->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	piggybody->setCategoryBitmask(0xFFFFFFFF);
	piggybody->setCollisionBitmask(0xFFFFFFFF);
	piggybody->setContactTestBitmask(0xFFFFFFFF);
	piggybody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	piggy1->setPhysicsBody(piggybody);
	this->addChild(piggy1);
}

void GameScene::createIceplane(int i, int j)
{
	auto wall8 = Sprite::create("iceplane.png");
	wall8->setPosition(i, j);
	wall8->setTag(6);
	icePlane.push_back(wall8);
	auto wallbody = PhysicsBody::createBox(wall8->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	wallbody->setCategoryBitmask(0xFFFFFFFF);
	wallbody->setCollisionBitmask(0xFFFFFFFF);
	wallbody->setContactTestBitmask(0xFFFFFFFF);
	wallbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	wall8->setPhysicsBody(wallbody);
	this->addChild(wall8);
}

void GameScene::createLrmoveplane(int i, int j)
{
	auto wall4 = Sprite::create("moveplane.png");
	wall4->setPosition(i, j);
	lrmovePlane.push_back(wall4);
	auto wallbody = PhysicsBody::createBox(wall4->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	wallbody->setCategoryBitmask(0xFFFFFFFF);
	wallbody->setCollisionBitmask(0xFFFFFFFF);
	wallbody->setContactTestBitmask(0xFFFFFFFF);
	wallbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	wall4->setPhysicsBody(wallbody);
	this->addChild(wall4);
}

void GameScene::createUdmoveplane(int i, int j)
{
	auto wall10 = Sprite::create("moveplane.png");
	wall10->setPosition(i, j);
	udmovePlane.push_back(wall10);
	auto wallbody = PhysicsBody::createBox(wall10->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	wallbody->setCategoryBitmask(0xFFFFFFFF);
	wallbody->setCollisionBitmask(0xFFFFFFFF);
	wallbody->setContactTestBitmask(0xFFFFFFFF);
	wallbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	wall10->setPhysicsBody(wallbody);
	this->addChild(wall10);
}

void GameScene::createBox(int i, int j)
{
	auto box = Sprite::create("box.png");
	box->setPosition(i, j);
	boxes.push_back(box);
	box->setTag(5);
	auto boxbody = PhysicsBody::createBox(box->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	boxbody->setCategoryBitmask(0xFFFFFFFF);
	boxbody->setCollisionBitmask(0xFFFFFFFF);
	boxbody->setContactTestBitmask(0xFFFFFFFF);
	boxbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	box->setPhysicsBody(boxbody);
	this->addChild(box,3);
}

//愤怒值增加
void GameScene::angryIncrease()
{
	auto per = pT->getPercentage();
	if (per <= 90)
	{
		angry = per + 10;
		auto to = ProgressFromTo::create(0.01f, per, per + 10);
		pT->runAction(to);
	}
	else
	{
		angry = 100;
		auto to = ProgressFromTo::create(0.01f, per, 100);
		pT->runAction(to);
	}
}

//愤怒值减少
void GameScene::angryDecrease()
{
	auto per = pT->getPercentage();
	if (per >=  Global::round)
	{
		angry = per - Global::round;
		auto to = ProgressFromTo::create(0.0f, per, per - Global::round);
		pT->runAction(to);
	}
	else
	{
		angry = 0;
		auto to = ProgressFromTo::create(0.0f, per, 0);
		pT->runAction(to);
	}
}

void GameScene::createRound()
{	
	//width (1 - 8) * 100, height( 0 - 4 )*100
	int totalnum;
	if (Global::round < 7) totalnum = 23 - Global::round * 3;
	else totalnum = 4;
	
	int lrNum = totalnum * Global::round / 20;
	int udNum = totalnum * Global::round / 20;
	int iceNum = totalnum * Global::round / 20;
	int groundNum = totalnum - lrNum - udNum - iceNum;
	
	int isUsed[9][5];
	
	bool isPlayerSet = false;
	bool isBoxSet = false;
	int playerPos = random(0, totalnum);
	int boxPos = random(0, totalnum);
	while (boxPos == playerPos) boxPos = random(0, totalnum);
	
	int anchorwidth = 0;
	int anchorheight = 0;
	
	for (int i = 0; i < 9; i++)
		for (int j = 0; j < 5; j++)
			isUsed[i][j] = 0;
	
	for (int i = 0; i < totalnum; i++)
	{
		//round * 20 percent lr, round * 20 ud,  round * 20 percent iceground
		bool insertSuccess = false;
		while(! insertSuccess)
		{
			int width =  (int)(random(1, 9));
			int height =  (int)(random(0, 5));
			if (isUsed[width][height] == 0)
			{
				isUsed[width][height] = 1;
				insertSuccess = true;
				if (i < lrNum) createLrmoveplane(anchorwidth + width * 100, anchorheight + height * 100);
				else if (i < lrNum + udNum) createUdmoveplane(anchorwidth + width * 100, anchorheight + height * 100);
				else if (i < lrNum + udNum + iceNum)
				{
					createIceplane(anchorwidth + width * 100, anchorheight + height * 100);
				}
				else
				{
					createGreenplane(anchorwidth + width * 100, anchorheight + height * 100);
					if ((int)random(0, 2)) createPiggy(anchorwidth + width * 100, anchorheight + height * 100 + 30);
				}
				if (width == playerPos && !isPlayerSet)
				{
					player->setPosition(Vec2(anchorwidth + width * 100, anchorheight + height * 100 + 40));
					isPlayerSet = true;
				}
				//else if (!isBoxSet && width == boxPos)
				//{
				//	createBox(anchorwidth + width * 100, anchorheight + height * 100 + 80);
				//	isBoxSet = true;
				//}
			}
		}
	}
	for (int i = 0; i < random(1, 10); i++)
	{
		int width = (int)(random(1, 16)) * 50;
		int height = (int)(random(1, 11)) * 50 + 30;
		createHeart(width, height);
	}
	createBox(800, 500);
}



void GameScene::addWall() {
	auto width = visibleSize.width, height = visibleSize.height;
	auto origin = Director::getInstance()->getVisibleOrigin();

	//创建普通绿地
	createGreenplane(100, 50);
	createGreenplane(200, 50);
	createGreenplane(300, 50);
	createGreenplane(700, 150);
	createGreenplane(800, 150);
	createGreenplane(visibleSize.width - 20, 200);
	createGreenplane(50, 150);
	createGreenplane(150, 150);
	createGreenplane(250, 250);
	createGreenplane(350, 250);
	createGreenplane(300, 550);
	createGreenplane(400, 450);
	createGreenplane(500, 550);
	createGreenplane(600, 550);
	createGreenplane(700, 450);
	createGreenplane(150, 350);
	
	//创建小心心
	createHeart(300, 80);
	createHeart(visibleSize.width - 20, 230);
	createHeart(50, 380);

	//创建小怪兽
	createPiggy(800, 180);
	createPiggy(150, 380);
	createPiggy(700, 480);
	
	//创建普通冰块
	createIceplane(820, 290);
	createIceplane(650, 290);
	createIceplane(50, 350);
	createIceplane(250, 350);

	//创建左右移动的冰块
	createLrmoveplane(500, 100);
	createLrmoveplane(visibleSize.width - 50, 380);

	//创建上下移动的冰块
	createUdmoveplane(500, 360);

	//创建箱子
	createBox(600, 580);
}

void GameScene::preloadAnimation() {
	Vector<SpriteFrame*> PlayerIdle;
	Vector<SpriteFrame*> PlayerRun;
	Vector<SpriteFrame*> PlayerJump;
	Vector<SpriteFrame*> PiggyRunLeft;
	Vector<SpriteFrame*> PiggyRunRight;

	auto PlayerImage = Director::getInstance()->getTextureCache()->addImage("player1.png");

	PlayerIdle.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(32 * i, 5, 32, 36)));
		PlayerIdle.pushBack(frame);
	}
	Animation* PlayerIdelWithoutBoxAnimation = Animation::createWithSpriteFrames(PlayerIdle, 0.1f);
	AnimationCache::getInstance()->addAnimation(PlayerIdelWithoutBoxAnimation, "IdleWithoutBoxAnimation");

	PlayerRun.reserve(8);
	for (int i = 0; i < 8; i++) {
		auto frame = SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(32 * i, 41, 32, 36)));
		PlayerRun.pushBack(frame);
	}
	Animation* PlayerRunWithoutBoxAnimation = Animation::createWithSpriteFrames(PlayerRun, 0.1f);
	AnimationCache::getInstance()->addAnimation(PlayerRunWithoutBoxAnimation, "RunWithoutBoxAnimation");


	PlayerJump.pushBack(SpriteFrame::createWithTexture(PlayerImage, CC_RECT_PIXELS_TO_POINTS(Rect(64, 41, 32, 36))));
	PlayerJump.pushBack(frame1);	
	Animation* PlayerJumpWithoutBoxAnimation = Animation::createWithSpriteFrames(PlayerJump, 1.0f);
	AnimationCache::getInstance()->addAnimation(PlayerJumpWithoutBoxAnimation, "JumpWithoutBoxAnimation");


	auto pigImage = Director::getInstance()->getTextureCache()->addImage("pig.png");
	PiggyRunLeft.reserve(3); PiggyRunRight.reserve(3);
	for (int i = 0; i < 3; i++) {
		auto framel = SpriteFrame::createWithTexture(pigImage, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 0, 80, 80)));
		auto framer = SpriteFrame::createWithTexture(pigImage, CC_RECT_PIXELS_TO_POINTS(Rect(80 * i, 80, 80, 80)));
		PiggyRunLeft.pushBack(framel);
		PiggyRunRight.pushBack(framer);
	}
	Animation* piggyRunLeft = Animation::createWithSpriteFrames(PiggyRunLeft, 0.06f), 
		* piggyRunRight = Animation::createWithSpriteFrames(PiggyRunRight, 0.06f);
	AnimationCache::getInstance()->addAnimation(piggyRunLeft, "piggyRunLeft");
	AnimationCache::getInstance()->addAnimation(piggyRunRight, "piggyRunRight");

}

void GameScene::addSprite() {
	//background
    bgSprite = Sprite::create("bg.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
		visibleSize.height / bgSprite->getContentSize().height);
	bgSprite->setTag(7);
	this->addChild(bgSprite, 0);

	//player
	texture1 = Director::getInstance()->getTextureCache()->addImage("player1.png");
	frame1 = SpriteFrame::createWithTexture(texture1, CC_RECT_PIXELS_TO_POINTS(Rect(0, 4, 32, 36)));
	player = Sprite::createWithSpriteFrame(frame1);
	player->setTag(2);
	int xpos = visibleSize.width / 2;
	player->setPosition(100, 90);
	player->setName("player");
	auto playerbody = PhysicsBody::createBox(player->getContentSize(), PhysicsMaterial(100.0f, 0.0f, 1.0f));
	playerbody->setCategoryBitmask(0xFFFFFFFF);
	playerbody->setCollisionBitmask(0xFFFFFFFF);
	playerbody->setContactTestBitmask(0xFFFFFFFF);
	playerbody->setRotationEnable(false);
	//playerbody->setDynamic(false);  // 静态刚体不受重力影响, 同时也无法发生倾斜了
	player->setPhysicsBody(playerbody);
	this->addChild(player, 2);

	//piggy
	auto piggytexture = Director::getInstance()->getTextureCache()->addImage("pig.png");
	piggyframe = SpriteFrame::createWithTexture(piggytexture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 80, 80)));

	char* tmp2 = new char[13];
	char* time = "Time: ";
	sprintf(tmp2, "%s%d", time, Global::dtime);
	timeCount = Label::createWithTTF(tmp2, "Marker Felt.ttf", 30);
	timeCount->setColor(Color3B(255, 255, 255));
	timeCount->setPosition(visibleSize.width / 2, visibleSize.height - 30);
	this->addChild(timeCount, 3);

	char* tmp = new char[13];
	char* round = "Round: ";
	sprintf(tmp, "%s%d", round, Global::round);
	roundCount = Label::createWithTTF(tmp, "Marker Felt.ttf", 30);
	roundCount->setColor(Color3B(255, 255, 255));
	roundCount->setPosition(visibleSize.width / 2 + 150, visibleSize.height - 30);
	this->addChild(roundCount, 3);

	char* tmp1 = new char[13];
	char* lives = "Lives: ";
	sprintf(tmp1, "%s%d", lives, Global::heart);
	heartCount = Label::createWithTTF(tmp1, "Marker Felt.ttf", 30);
	heartCount->setColor(Color3B(255, 255, 255));
	heartCount->setPosition(visibleSize.width / 2 + 300, visibleSize.height - 30);
	this->addChild(heartCount, 3);

	musicOn = MenuItemImage::create("musicon.png", "musicon.png", CC_CALLBACK_0(GameScene::handleMusic, this));
	musicOn->setPosition(visibleSize.width - 30, visibleSize.height - 30);
	auto menu = Menu::create(musicOn, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);
}

void GameScene::update(float f)
{
	this->getScene()->getPhysicsWorld()->step(1 / 100.0f);
	if (IsPlayerLeft || IsPlayerRight)
	{
		//player->stopAllActionsByTag(10);
		//player->setSpriteFrame(frame1);
		auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("RunWithoutBoxAnimation")));
		animation->setTag(10);
		player->runAction(animation);
		player->getPhysicsBody()->setVelocity(Vec2(IsPlayerLeft ? -150 : 150, 0));
	}
	if (player->getPositionX() <= 10)
	{
		player->setPositionX(10);
	}
	else if (player->getPositionX() >= visibleSize.width - 10)
	{
		player->setPositionX(visibleSize.width - 10);
	}

	if (player->getPositionY() <= 0)
	{
		Global::heart--;
		char* tmp = new char[13];
		char* time = "Lives: ";
		sprintf(tmp, "%s%d", time, Global::heart);
		heartCount->setString(tmp);
		if(Global::heart > 0) gameOver();
	}
	else if (player->getPositionY() >= visibleSize.height)
	{
		Global::heart--;
		char* tmp = new char[13];
		char* time = "Lives: ";
		sprintf(tmp, "%s%d", time, Global::heart);
		heartCount->setString(tmp);
		if (Global::heart > 0) gameOver();
	}
	if(Global::heart <= 0)
	{
		unschedule(schedule_selector(GameScene::timeCountSchedule));
		unscheduleUpdate();
		unscheduleAllSelectors();
		_eventDispatcher->removeAllEventListeners();
		player->stopAllActions();
		SimpleAudioEngine::getInstance()->stopBackgroundMusic();
		SimpleAudioEngine::getInstance()->playEffect("music/gameover.wav", false);
		for (auto a : this->getChildren())
		{
			if (a->getTag() != 7)
			{
				auto b = a;
				auto moveto = MoveTo::create(random(0, 1), Vec2(a->getPositionX(), 0));
				auto Fade = FadeOut::create(1.0f);
				a->runAction(moveto);
				a->runAction(Fade);
			}
		}

		auto fade = FadeOut::create(1.0f);
		auto sequence = Sequence::create(fade, [this](){
		}, nullptr);
		bgSprite->runAction(sequence);

		auto gameover = Label::createWithTTF("Game Over", "fonts/AaMangoBrother.ttf", 44);
		gameover->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 100);
		this->addChild(gameover, 4);

		char* tmp2 = new char[23];
		char* time = "Total Round: ";
		sprintf(tmp2, "%s%d", time, Global::round - 1);
		auto Count = Label::createWithTTF(tmp2, "fonts/AaMangoBrother.ttf", 30);
		Count->setColor(Color3B(255, 255, 255));
		Count->setPosition(visibleSize.width / 2, visibleSize.height / 2 + 30);
		this->addChild(Count, 4);

		char* tmp = new char[43];
		char* time2 = "Your Time: ";
		char* juhao = "\'";
		char* fenhao = "\'";
		char* miao = "\"";
		sprintf(tmp, "%s%02d%s%02d%s%02d%s", time2, (Global::dtime / 3600) % 24, juhao, Global::dtime / 60, fenhao, Global::dtime % 60, miao);
		auto Count1 = Label::createWithTTF(tmp, "fonts/AaMangoBrother.ttf", 30);
		Count1->setColor(Color3B(255, 255, 255));
		Count1->setPosition(visibleSize.width / 2, visibleSize.height / 2 -10);
		this->addChild(Count1, 4);

		auto playagain = MenuItemImage::create("playagain.png", "playagain.png", CC_CALLBACK_1(GameScene::replayCallback, this));
		playagain->setPosition(visibleSize.width / 2, visibleSize.height / 2 - 70);
		auto menu = Menu::create(playagain, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 4);

		Global::heart = 1;
		Global::round = 1;
		Global::dtime = 0;
	}
}

void GameScene::timeCountSchedule(float f) {
	angryDecrease();
	
	Global::dtime++;
	char* tmp = new char[13];
	char* time = "time: ";
	sprintf(tmp, "%s%d", time, Global::dtime);
	timeCount->setString(tmp);
}

void GameScene::movePlaneSchedule(float f) {
	//angryDecrease();

	static int ct = 0;
	static int dir = 3;
	ct++;
	if (ct == 120) {
		ct = 40;
		dir = -dir;
	}
	else if (ct == 80) {
		dir = -dir;
	}
	else if (ct == 20) {
		ct = 40;
		dir = -dir;
	}	
	if (lrmovePlane.size() != 0) {
		for (Sprite* s : lrmovePlane) {
			if (s != NULL) {
				s->setPosition(s->getPosition() + Vec2(dir, 0));
			}
		}
	}
	if (udmovePlane.size() != 0) {
		for (Sprite *s : udmovePlane) {
			if (s != NULL)
				s->setPosition(s->getPosition() + Vec2(0, -dir));
		}
	}
	if (piggys.size() != 0) {
		for (auto s : piggys) {
			if (s != NULL) {
				s->stopAllActions();
				string aname = dir >= 0 ? "piggyRunLeft" : "piggyRunRight";
				auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation(aname)));
				s->runAction(animation);
				s->setPosition(s->getPosition() + Vec2(dir / 2, 0));
			}
		}
	}
}

void GameScene::handleMusic() {
	auto audio = SimpleAudioEngine::getInstance();
	if (audio->isBackgroundMusicPlaying()) {
		audio->pauseBackgroundMusic();
		musicOn->removeFromParent();
		musicOn = MenuItemImage::create("musicoff.png", "musicoff.png", CC_CALLBACK_0(GameScene::handleMusic, this));
		musicOn->setPosition(visibleSize.width - 30, visibleSize.height - 30);
		auto menu = Menu::create(musicOn, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
		
	else {
		audio->resumeBackgroundMusic();
		musicOn->removeFromParent();
		musicOn = MenuItemImage::create("musicon.png", "musicon.png", CC_CALLBACK_0(GameScene::handleMusic, this));
		musicOn->setPosition(visibleSize.width - 30, visibleSize.height - 30);
		auto menu = Menu::create(musicOn, NULL);
		menu->setPosition(Vec2::ZERO);
		this->addChild(menu, 1);
	}
		
	
}

void GameScene::addKeyboardListener() {
	auto keyboardListener = EventListenerKeyboard::create();
	keyboardListener->onKeyPressed = CC_CALLBACK_2(GameScene::onKeyPressed, this);
	keyboardListener->onKeyReleased = CC_CALLBACK_2(GameScene::onKeyReleased, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}

void GameScene::addContactListener()
{
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(GameScene::onContactBegin, this);
	this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactListener, this);
}

void GameScene::onKeyPressed(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
		//move to left
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
		case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		case EventKeyboard::KeyCode::KEY_A:
			IsPlayerLeft = true;
			player->setFlippedX(true);
			break;

		//move to right
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
		case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		case EventKeyboard::KeyCode::KEY_D:
			IsPlayerRight = true;
			player->setFlippedX(false);
			break;
		
		//jump
		case EventKeyboard::KeyCode::KEY_SPACE:
		case EventKeyboard::KeyCode::KEY_UP_ARROW:
			if (!IsPlayerJump)
			{
				IsPlayerJump = true;
				auto animation = RepeatForever::create(Animate::create(AnimationCache::getInstance()->getAnimation("JumpWithoutBoxAnimation")));
				animation->setTag(11);
				player->getPhysicsBody()->setVelocity(Vec2(0, max * angry / 100));
				player->runAction(animation);
			}
			break;		
	}
}

void GameScene::onKeyReleased(EventKeyboard::KeyCode code, Event* event) {
	switch (code) {
	case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
	case EventKeyboard::KeyCode::KEY_A:
	case EventKeyboard::KeyCode::KEY_CAPITAL_A:
		IsPlayerLeft = false;
		player->stopAllActionsByTag(10);
		player->setSpriteFrame(frame1);
		break;

	case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
	case EventKeyboard::KeyCode::KEY_D:
	case EventKeyboard::KeyCode::KEY_CAPITAL_D:
		IsPlayerRight = false;
		player->stopAllActionsByTag(10);
		player->setSpriteFrame(frame1);
		break;

	default:break;
	}
}

bool GameScene::onContactBegin(PhysicsContact & contact)
{
	auto sp1 = (Sprite*)contact.getShapeA()->getBody()->getNode();
	auto sp2 = (Sprite*)contact.getShapeB()->getBody()->getNode();
	auto sp1tag = sp1->getTag();
	auto sp2tag = sp2->getTag();
	
	//其中一个角色是玩家
	if (sp1tag == 2 || sp2tag == 2)
	{
		SimpleAudioEngine::getInstance()->playEffect("music/hit.wav", false);
		//碰到小心心, 小心心消失，愤怒值增加
		if (sp1tag == 3 || sp2tag == 3)
		{
			angryIncrease();
			if (sp1tag == 3)
			{
				sp1->removeFromParent();
				hearts.remove(sp1);
			}
			else
			{
				sp2->removeFromParent();
				hearts.remove(sp2);
			}
			Global::heart++;
			char* tmp = new char[13];
			char* time = "Lives: ";
			sprintf(tmp, "%s%d", time, Global::heart);
			heartCount->setString(tmp);
		}

		//碰到怪物，游戏结束
		else if (sp1tag == 4 || sp2tag == 4)
		{
			Global::heart--;
			char* tmp = new char[13];
			char* time = "Lives: ";
			sprintf(tmp, "%s%d", time, Global::heart);
			heartCount->setString(tmp);
			if (Global::heart > 0) gameOver();
		}

		//碰到箱子，下一回合
		else if (sp1tag == 5 || sp2tag == 5)
		{
			Global::round++;
			//createRound();
			unscheduleAllSelectors();
			unscheduleAllCallbacks();
			unscheduleUpdate();
			_eventDispatcher->removeAllEventListeners();
			SimpleAudioEngine::getInstance()->stopBackgroundMusic();
			SimpleAudioEngine::getInstance()->playEffect("music/win.wav", false);
			auto scene = GameScene::createScene();
			auto transition = TransitionZoomFlipY::create(1.0f, scene);
			Director::getInstance()->replaceScene(transition);
		}

		//碰到冰，冰融化
		else if (sp1tag == 6 || sp2tag == 6)
		{
			if (sp1tag == 6)
			{
				auto to = CCFadeOut::create(3);
				auto temp = sp1;
				//auto seq = Sequence::create(to, [=]() {
				//	sp1->removeFromParent();
				//}, nullptr);
				temp->removeFromParent();
				//sp1->runAction(seq);
				icePlane.remove(temp);
			}
		}

		//跳跃时碰撞，注意要除去小心心
		if (IsPlayerJump && sp1tag != 3 && sp2tag != 3)
		{
			IsPlayerJump = false;
			player->stopAllActionsByTag(11);
			player->stopAllActions();
			//if(player->getSpriteFrame() != frame1)player->setSpriteFrame(frame1);
			angryIncrease();
		}
	}

	return true;
}

void GameScene::gameOver() {
	unschedule(schedule_selector(GameScene::timeCountSchedule));
	unscheduleUpdate();
	unscheduleAllSelectors();
	_eventDispatcher->removeAllEventListeners();
	player->stopAllActions();
	SimpleAudioEngine::getInstance()->stopBackgroundMusic();
	SimpleAudioEngine::getInstance()->playEffect("music/gameover.wav", false);
	auto playagain = MenuItemImage::create("playagain.png", "playagain.png", CC_CALLBACK_1(GameScene::replayCallback, this));
	playagain->setPosition(visibleSize.width / 2, visibleSize.height / 2);
	auto menu = Menu::create(playagain, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 4);
}

void GameScene::replayCallback(Ref * pSender) {
	Director::getInstance()->replaceScene(GameScene::createScene());
}