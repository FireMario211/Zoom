#pragma once

#include "utils.hpp"
using namespace geode::prelude;

CCSize getScreenSize() {
	float screenTop = CCDirector::sharedDirector()->getScreenTop();
	float screenBottom = CCDirector::sharedDirector()->getScreenBottom();
	float screenLeft = CCDirector::sharedDirector()->getScreenLeft();
	float screenRight = CCDirector::sharedDirector()->getScreenRight();

	return CCSize{ screenRight - screenLeft, screenTop - screenBottom };
}

void clampPlayLayerPos() {
	CCNode* playLayer = CCScene::get()->getChildByID("PlayLayer");
	if (!playLayer) return;

	CCPoint pos = playLayer->getPosition();
	CCSize screenSize = getScreenSize();
	CCSize contentSize = playLayer->getContentSize();

	float xLimit = (contentSize.width * playLayer->getScale() - screenSize.width) * 0.5f;
	float yLimit = (contentSize.height * playLayer->getScale() - screenSize.height) * 0.5f;

	pos.x = clamp(pos.x, -xLimit, xLimit);
	pos.y = clamp(pos.y, -yLimit, yLimit);

	playLayer->setPosition(pos);
}