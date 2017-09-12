#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"
#include "SimpleAudioEngine.h"
#include "Global.h"

USING_NS_CC;
using namespace std;
class GameScene : public cocos2d::Layer {
public:

	static PhysicsWorld* world;
	
	// there's no 'id' in cpp, so we recommend returning the class instance pointer
	static cocos2d::Scene* createScene();

	void setPhysicsWorld(PhysicsWorld * world);

	// Here's a difference. Method 'init' in cocos2d-x returns bool, instead of returning 'id' in cocos2d-iphone
	virtual bool init();

	// implement the "static create()" method manually
	CREATE_FUNC(GameScene);
private:
	PhysicsWorld* m_world;
	void preloadMusic();
	void preloadAnimation();
	void addSprite();
	void addWall();
	void gameOver();
	void handleMusic();
	void replayCallback(Ref * pSender);

	void createGreenplane(int i, int j);
	void createHeart(int i, int j);
	void createPiggy(int i, int j);
	void createIceplane(int i, int j);
	void createLrmoveplane(int i, int j);
	void createUdmoveplane(int i, int j);
	void createBox(int i, int j);

	void angryIncrease();
	void angryDecrease();

	void createRound();

	//listener
	void addKeyboardListener();
	void addContactListener();

	void onKeyPressed(EventKeyboard::KeyCode code, Event* event);
	void onKeyReleased(EventKeyboard::KeyCode code, Event* event);
	bool onContactBegin(PhysicsContact & contact);

	//schedule
	void update(float f);
	void timeCountSchedule(float f);		
	void movePlaneSchedule(float f);
	
	Size visibleSize;
	Sprite* player;
	Sprite* bgSprite;
	char moveKey;
	bool IsPlayerLeft;
	bool IsPlayerRight;
	bool IsPlayerJump;
	int angry; //愤怒值
	int max;   //最大力度
	GameScene* game;

	MenuItemImage* musicOn;

	// show time
	Label* timeCount;
	Label* roundCount;
	Label* heartCount;

	Texture2D* texture1;
	SpriteFrame* frame1;
	SpriteFrame* piggyframe;
	list<Sprite*> simplePlane;
	list<Sprite*> lrmovePlane;
	list<Sprite*> udmovePlane;
	list<Sprite*> duPlane;
	list<Sprite*> icePlane;
	list<Sprite*> hearts;
	list<Sprite*> piggys;
	list<Sprite*> boxes;

	cocos2d::ProgressTimer* pT;
};

#endif 
