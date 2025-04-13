#pragma once

class SettingsManager {
public:
	static SettingsManager* get();
	void init();

	// TODO: Macro these based on platform
	bool autoHideMenu;
	bool autoShowMenu;
	bool altDisablesZoom;
};