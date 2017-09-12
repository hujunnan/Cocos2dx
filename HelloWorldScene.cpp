#include "HelloWorldScene.h"
#include "GameScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto bgSprite = Sprite::create("bg.png");
	bgSprite->setPosition(visibleSize / 2);
	bgSprite->setScale(visibleSize.width / bgSprite->getContentSize().width,
		visibleSize.height / bgSprite->getContentSize().height);
	this->addChild(bgSprite, -1);

	auto bgSprite2 = Sprite::create("muban.jpg");
	bgSprite2->setPosition(visibleSize / 2);
	bgSprite2->setScale((visibleSize.width - 00) / bgSprite2->getContentSize().width,
		visibleSize.height / bgSprite2->getContentSize().height);
	this->addChild(bgSprite2, 0);

	//add a meauitem
	auto clickItem = MenuItemImage::create("play.png", "play.png", CC_CALLBACK_1(HelloWorld::clickMenuCloseCallback, this));
	clickItem->setTag(1);
	clickItem->setPosition(Vec2(origin.x + visibleSize.width / 2,
		origin.y + clickItem->getContentSize().height / 2 + 40));

    auto menu = Menu::create(clickItem, NULL);
    menu->setPosition(Vec2::ZERO);
	menu->setTag(1);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows name
    // create and initialize a label
	CCDictionary* message = CCDictionary::createWithContentsOfFile("MyXml.xml");
	auto name = message->valueForKey("name");
	auto nametwo = message->valueForKey("nametwo");
	const cocos2d::__String *points[8];
	points[0] = message->valueForKey("guide");
	points[1] = message->valueForKey("firstpoint");
	points[2] = message->valueForKey("secondpoint");
	points[3] = message->valueForKey("thirdpoint");
	points[4] = message->valueForKey("forthpoint");
	points[5] = message->valueForKey("fifthpoint");
	points[6] = message->valueForKey("sixpoint");
	points[7] = message->valueForKey("sevenpoint");
	auto ready = message->valueForKey("ready");

    auto label = Label::createWithTTF(name->getCString(), "fonts/AaMangoBrother.ttf", 34);
    label->setPosition(Vec2(origin.x + (visibleSize.width  )/2,
                            origin.y + visibleSize.height - label->getContentSize().height));
    this->addChild(label, 2);

	auto label2 = Label::createWithTTF(nametwo->getCString(), "fonts/AaMangoBrother.ttf", 34);
	label2->setPosition(Vec2(origin.x + (visibleSize.width) / 2,
		                     origin.y + visibleSize.height - 2 * label2->getContentSize().height - 20));
	this->addChild(label2, 2);

	for (int i = 0; i < 8; i++)
	{
		auto label = Label::createWithTTF((points[i])->getCString(), "fonts/AaMangoBrother.ttf", 24);
		label->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 4 * label2->getContentSize().height - i * 40));
		this->addChild(label, 2);
	}
    
	auto label3 = Label::createWithTTF(ready->getCString(), "fonts/AaMangoBrother.ttf", 24);
	label3->setPosition(Vec2(origin.x + (visibleSize.width) / 2,
		origin.y + visibleSize.height - 4 * label2->getContentSize().height - 320));
	this->addChild(label3, 2);

    return true;
}

void HelloWorld::clickMenuCloseCallback(Ref* pSender)
{
	auto scene = GameScene::createScene();
	auto transition = TransitionPageTurn::create(1.0f, scene, false);
	Director::getInstance()->replaceScene(transition);
}