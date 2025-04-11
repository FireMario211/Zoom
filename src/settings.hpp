#pragma once

class SettingsManager {
public:
	static SettingsManager* get();
	void init();

	bool autoHideMenu;
	bool autoShowMenu;
	bool altDisablesZoom;
};