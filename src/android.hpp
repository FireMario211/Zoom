// #ifdef GEODE_IS_ANDROID64
#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class AndroidZoomLayer : public CCLayer {

};

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
		geode::createQuickPopup(
			"Title",            // title
			"Say hi to mom?",   // content
			"Nah", "Yeah",      // buttons
			[](auto, bool btn2) {
				if (btn2) {
					// say hi to mom
				}
			}
		);
	}
};

// #endif