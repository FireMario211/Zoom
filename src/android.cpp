#ifdef GEODE_IS_ANDROID

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
	backSpr->setOpacity(100);
	
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
	m_playLayer->setScale(1.0f);
	m_playLayer->setPosition(ccp(0, 0));
	m_pauseLayer->setVisible(true);
	this->removeFromParentAndCleanup(true);
	AndroidZoomLayer::instance = nullptr;
}

bool AndroidZoomLayer::ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) {
	m_touches.push_back(pTouch);
	if (m_touches.size() > 1 && !m_isZooming) {
		m_isZooming = true;
		m_ZoomAnchor = getAnchorPoint(m_touches[0], m_touches[1]);
	}

	return true;
}

void AndroidZoomLayer::ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent) {
	if (m_touches.size() == 1) {
		CCTouch* touch = m_touches[0];
		CCPoint delta = touch->getDelta();
		CCPoint pos = m_playLayer->getPosition();
		m_playLayer->setPosition(pos.x + delta.x, pos.y + delta.y);
		clampPlayLayerPos(m_playLayer);
	} else {
		if (!m_isZooming) return;

		CCTouch* movingTouch = m_touches[0];
		CCTouch* anchoredTouch = m_touches[1];

		// Only process the first two touches
		if (pTouch != movingTouch && pTouch != anchoredTouch) return;
		if (pTouch == anchoredTouch) {
			movingTouch = m_touches[1];
			anchoredTouch = m_touches[0];
		}

		// Center the anchor point between the two touches
		CCPoint newAnchor = getAnchorPoint(movingTouch, anchoredTouch);
		CCPoint deltaAnchor = ccpSub(newAnchor, m_ZoomAnchor);
		m_ZoomAnchor = newAnchor;
		
		// Move the play layer based on the delta of the anchor point
		CCPoint pos = m_playLayer->getPosition();
		m_playLayer->setPosition(pos.x + deltaAnchor.x, pos.y + deltaAnchor.y);
		
		CCPoint delta = movingTouch->getDelta();
		CCPoint touchDisplacement = ccpSub(movingTouch->getLocation(), anchoredTouch->getLocation());
		float scaleDelta = touchDisplacement.normalize().dot(delta) / 100.0f;
		
		zoomPlayLayer(m_playLayer, scaleDelta, m_ZoomAnchor);
		clampPlayLayerPos(m_playLayer);
	}
}

void AndroidZoomLayer::ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) {
	removeTouchEvent(pTouch, pEvent);
}

void AndroidZoomLayer::ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent) {
	removeTouchEvent(pTouch, pEvent);
}

void AndroidZoomLayer::removeTouchEvent(CCTouch* pTouch, CCEvent* pEvent) {
	m_touches.erase(std::remove(m_touches.begin(), m_touches.end(), pTouch), m_touches.end());

	if (m_touches.size() < 2) {
		m_isZooming = false;
		m_ZoomAnchor = ccp(0, 0);
	}
}

CCPoint AndroidZoomLayer::getAnchorPoint(CCTouch* movingTouch, CCTouch* anchoredTouch) {
	auto movingTouchPos = movingTouch->getLocation();
	auto anchoredTouchPos = anchoredTouch->getLocation();

	return ccp(
		(movingTouchPos.x + anchoredTouchPos.x) / 2,
		(movingTouchPos.y + anchoredTouchPos.y) / 2
	);
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

#endif // GEODE_IS_ANDROID