#include "PickpocketReplace.h"
#include "Events.h"

void InitLogger()
{
#ifndef NDEBUG
	auto sink = std::make_shared<spdlog::sinks::msvc_sink_mt>();
#else
	auto path = logger::log_directory();
	if (!path) {
		return;
	}

	*path /= fmt::format("{}.log"sv, "HandToHandVR"sv);
	auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(path->string(), true);
#endif

	auto log = std::make_shared<spdlog::logger>("global log"s, std::move(sink));

#ifndef NDEBUG
	log->set_level(spdlog::level::trace);
#else
	log->set_level(spdlog::level::info);
	log->flush_on(spdlog::level::info);
#endif

	spdlog::set_default_logger(std::move(log));
	spdlog::set_pattern("%s(%#): [%^%l%$] %v"s);

	logger::info(FMT_STRING("{} v{}"), "HandToHandVR"sv, "1.0.0");
}

extern "C" DLLEXPORT constexpr auto SKSEPlugin_Version = []() {
    SKSE::PluginVersionData v{};
    v.PluginVersion(REL::Version{ 1,0,0,0 });
    v.PluginName("HandtoHandVR"sv);
    v.AuthorName("Rafear and colinswrath"sv);
    v.UsesAddressLibrary(true);
    v.HasNoStructUse(true);
    v.UsesStructsPost629(false);
    return v;
}();

SKSEPluginLoad(const SKSE::LoadInterface* skse)
{
	InitLogger();

	SKSE::Init(skse);
	logger::info("Loading Hand to Hand");
	PickpocketReplace::Install();
	Settings::LoadSettings();
	Events::Register();
	logger::info("Hand to Hand loaded");
	return true;
}
