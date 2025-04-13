// #ifdef GEODE_IS_ANDROID64
#pragma once

#include <Geode/Geode.hpp>
#include <vector>

using namespace geode::prelude;

class AndroidZoomLayer : public CCLayer {
public:
	static AndroidZoomLayer* instance;

	static AndroidZoomLayer* create(CCNode* sceneLayer);
	static AndroidZoomLayer* getOrCreate(CCNode* sceneLayer);

	bool init(CCNode* sceneLayer);
	bool ccTouchBegan(CCTouch* pTouch, CCEvent* pEvent) override;
	void ccTouchMoved(CCTouch* pTouch, CCEvent* pEvent) override;
	void ccTouchEnded(CCTouch* pTouch, CCEvent* pEvent) override;
	void ccTouchCancelled(CCTouch* pTouch, CCEvent* pEvent) override;
	void onBackButton(CCObject* sender);
	void onBackButton23(CCObject* sender);
private:
	CCNode* m_sceneLayer;
	std::vector<CCTouch*> m_touches = {};
};

// #endif