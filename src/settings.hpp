#pragma once

class SettingsManager {
public:
	static SettingsManager* get();
	void init();

	#if defined(GEODE_IS_WINDOWS)
	bool autoHideMenu;
	bool autoShowMenu;
	bool altDisablesZoom;
	float zoomSensitivity;
	#endif
};