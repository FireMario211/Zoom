#include "settings.hpp"

#include <geode.custom-keybinds/include/Keybinds.hpp>
#include <Geode/Geode.hpp>

using namespace geode::prelude;
using namespace keybinds;

SettingsManager* SettingsManager::get() {
	static auto inst = new SettingsManager;
	return inst;
}

void SettingsManager::init() {
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

    BindManager::get()->registerBindable({
        "toggle_menu"_spr,
        "Toggle Pause Menu",
        "",
        { Keybind::create(KEY_Home, Modifier::None) },
        "Zoom",
		false
    });
}