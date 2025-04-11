#include <charconv>

#include <geode.custom-keybinds/include/Keybinds.hpp>

#include <Geode/Geode.hpp>

#include <Geode/modify/MenuLayer.hpp>
#include <Geode/modify/PauseLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/CCMouseDispatcher.hpp>
#include <Geode/modify/PlayLayer.hpp>
#include <Geode/modify/CCEGLView.hpp>
#include <Geode/modify/CCApplication.hpp>
#include <Geode/modify/CCScheduler.hpp>

#include "settings.hpp"

using namespace geode::prelude;
using namespace keybinds;

// TODO: Uncomment this
// #if defined(GEODE_IS_WINDOWS)
// #include "windows.hpp"
// #elif defined(GEODE_IS_ANDROID64)
// #include "android.hpp"
// #endif

#include "android.hpp"

float clamp(float d, float min, float max) {
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

$execute {
	geode::log::info("Zoom mod loaded!");

	#if defined(GEODE_IS_WINDOWS)
		geode::log::info("Windows detected.");
	#elif defined(GEODE_IS_ANDROID64)
		geode::log::info("Android 64-bit detected.");
	#else
		geode::log::error("Unknown platform detected.");
		return;
	#endif

	SettingsManager::get()->init();
}