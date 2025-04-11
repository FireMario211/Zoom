#ifdef GEODE_IS_WINDOWS
#pragma once

#include <Geode/Geode.hpp>

using namespace geode::prelude;

class WindowsZoomManager {
public:
	CCPoint lastMousePos;
	CCPoint deltaMousePos;

	bool isPaused;
	bool isPanning;

	static WindowsZoomManager* get();

	void update(float dt);
	void togglePauseMenu();
	CCPoint screenToWorld(CCPoint pos);

	void setPauseMenuVisible(bool visible);
	void setPos(float x, float y);
	void setZoom(float zoom);

	void zoom(float delta);
	void move(CCPoint delta);

	float getZoom();
	CCSize getScreenSize();
	CCPoint getMousePosOnScreen();
	CCPoint getMousePosOnNode(CCNode* node);
	
	void onResume();
	void onPause();
	void onScroll(float y, float x);
private:
	void onScreenResize();
	void clampPos();
	void onScreenMove();
};
#endif // GEODE_IS_WINDOWS