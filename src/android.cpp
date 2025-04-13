// #ifdef GEODE_IS_ANDROID64

#include "utils.hpp"
#include "android.hpp"
#include "settings.hpp"

#include <algorithm>
#include <Geode/modify/PauseLayer.hpp>

using namespace geode::prelude;
AndroidZoomLayer* AndroidZoomLayer::instance = nullptr;
	
AndroidZoomLayer* AndroidZoomLayer::create(CCNode* sceneLayer) {
	auto layer = new AndroidZoomLayer();
	if (layer && layer->init(sceneLayer)) {
		layer->autorelease();
		instance = layer;
		return layer;
	}
	
	delete layer;
	return nullptr;
}

AndroidZoomLayer* AndroidZoomLayer::getOrCreate(CCNode* sceneLayer) {
	if (instance) {
		return instance;
	}

	return create(sceneLayer);
}

bool AndroidZoomLayer::init(CCNode* sceneLayer) {
	if (!CCLayer::init())
		return false;

	if (!sceneLayer) {
		geode::log::error("Scene layer is null!");
		return false;
	}


	if (sceneLayer->getChildByID("AnodroidZoomLayer")) {
		geode::log::error("AnodroidZoomLayer already exists in scene!");
		return false;
	}

	m_sceneLayer = sceneLayer;
	m_sceneLayer->addChild(this);

	m_playLayer = m_sceneLayer->getChildByID("PlayLayer");

	if (!m_playLayer) {
		geode::log::error("PlayLayer is null!");
		return false;
	}

	m_pauseLayer = m_sceneLayer->getChildByID("PauseLayer");
	if (!m_pauseLayer) {
		geode::log::error("PauseLayer is null!");
		return false;
	}

	m_pauseLayer->setVisible(false);

	// Thanks SillyDoggo for the code snippet :D
	// https://github.com/TheSillyDoggo/GeodeMenu/blob/17b19215b80a263379a560edfaf63c2a3f17e2f8/src/Client/AndroidUI.cpp#L28

	auto backMenu = CCMenu::create();
    backMenu->ignoreAnchorPointForPosition(false);
    backMenu->setContentSize(ccp(0, 0));
    backMenu->setPositionX(0);
    backMenu->setPositionY(CCDirector::get()->getWinSize().height);
    backMenu->setID("back-menu");
	this->addChild(backMenu);

    auto backSpr = CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
	backSpr->setOpacity(50);
	
    auto backBtn = CCMenuItemSpriteExtra::create(backSpr, this, menu_selector(AndroidZoomLayer::onBackButton));
    backBtn->setPosition(ccp(24, -23));
    backBtn->setSizeMult(1.15f);

    backMenu->addChild(backBtn);
	
	this->setID("AnodroidZoomLayer");

	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, -250, true);
	this->setTouchEnabled(true);

	backMenu->setTouchPriority(CCDirector::sharedDirector()->getTouchDispatcher()->getTargetPrio());
	CCDirector::sharedDirector()->getTouchDispatcher()->registerForcePrio(backMenu, 2);

	geode::log::info("AndroidZoomLayer initialized!");
	return true;
}

void AndroidZoomLayer::onBackButton(CCObject* sender) {
	geode::log::info("Back button pressed in AndroidZoomLayer!");

	m_playLayer->setScale(1.0f);
	m_playLayer->setPosition(ccp(0, 0));
	m_pauseLayer->setVisible(true);
	this->removeFromParentAndCleanup(true);
	AndroidZoomLayer::instance = nullptr;
}

bool AndroidZoomLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
	m_touches.push_back(pTouch);
	return true;
}

void AndroidZoomLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent) {
	if (m_touches.size() == 1) {
		auto touch = m_touches[0];
		auto delta = touch->getDelta();
		auto pos = m_playLayer->getPosition();
		m_playLayer->setPosition(pos.x + delta.x, pos.y + delta.y);
		clampPlayLayerPos(m_playLayer);
	} else {
		// TODO: Add zoom functionality here
	}
}

void AndroidZoomLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
	m_touches.erase(std::remove(m_touches.begin(), m_touches.end(), pTouch), m_touches.end());
}

void AndroidZoomLayer::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent) {
	m_touches.erase(std::remove(m_touches.begin(), m_touches.end(), pTouch), m_touches.end());
}
	
class $modify(AndroidZoomPauseLayer, PauseLayer) {
	void customSetup() {
		PauseLayer::customSetup();

		auto rightButtonMenu = getChildByID("right-button-menu");

		auto zoomButtonSprite = CircleButtonSprite::createWithSprite("zoom_button.png"_spr, 1.0f, CircleBaseColor::Green, CircleBaseSize::MediumAlt);
		zoomButtonSprite->getTopNode()->setScale(1.0f);
		zoomButtonSprite->setScale(0.6f);
	
		auto zoomButton = CCMenuItemSpriteExtra::create(zoomButtonSprite, this, menu_selector(AndroidZoomPauseLayer::onZoomButton));
		zoomButton->setID("zoom-button"_spr);
	
		rightButtonMenu->addChild(zoomButton);
		rightButtonMenu->updateLayout();
	}

	void onZoomButton(CCObject* sender) {
		AndroidZoomLayer* layer = AndroidZoomLayer::getOrCreate(this->getParent());
	}
};

// #endif