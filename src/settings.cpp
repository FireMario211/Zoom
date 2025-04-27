#include "settings.hpp"

#include <Geode/Geode.hpp>

using namespace geode::prelude;

#if defined(GEODE_IS_WINDOWS)
#include <geode.custom-keybinds/include/Keybinds.hpp>
using namespace keybinds;
#endif

SettingsManager* SettingsManager::get() {
	static auto inst = new SettingsManager;
	return inst;
}

void SettingsManager::init() {
	#if defined(GEODE_IS_WINDOWS)
	autoHideMenu = Mod::get()->getSettingValue<bool>("auto-hide-menu");
	listenForSettingChanges("auto-hide-menu", [&](bool enable) {
		autoHideMenu = enable;
	});

	autoShowMenu = Mod::get()->getSettingValue<bool>("auto-show-menu");
	listenForSettingChanges("auto-show-menu", [&](bool enable) {
		autoShowMenu = enable;
	});

	altDisablesZoom = Mod::get()->getSettingValue<bool>("alt-disables-zoom");
	listenForSettingChanges("alt-disables-zoom", [&](bool enable) {
		altDisablesZoom = enable;
	});

	zoomSensitivity = Mod::get()->getSettingValue<float>("zoom-sensitivity");
	listenForSettingChanges("zoom-sensitivity", [&](float sensitivity) {
		zoomSensitivity = sensitivity;
	});

    BindManager::get()->registerBindable({
        "toggle_menu"_spr,
        "Toggle Pause Menu",
        "",
        { Keybind::create(KEY_Home, Modifier::None) },
        "Zoom",
		false
    });
	#endif // GEODE_IS_WINDOWS
}