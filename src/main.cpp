#include <charconv>

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

#if defined(GEODE_IS_ANDROID) || defined(GEODE_IS_IOS)
#include "mobile.hpp"
#endif

using namespace geode::prelude;

float clamp(float d, float min, float max) {
	const float t = d < min ? min : d;
	return t > max ? max : t;
}

$execute {
	geode::log::info("Zoom mod loaded!");

	#if defined(GEODE_IS_WINDOWS)
		geode::log::info("Windows detected.");
	#elif defined(GEODE_IS_ANDROID)
		geode::log::info("Android detected.");
	#elif defined(GEODE_IS_IOS)
		geode::log::info("IOS detected.");
	#else
		geode::log::error("Unknown platform detected.");
		return;
	#endif

	SettingsManager::get()->init();
}