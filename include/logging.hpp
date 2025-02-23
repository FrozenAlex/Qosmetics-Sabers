#pragma once
#include "beatsaber-hook/shared/utils/logging.hpp"
#include "beatsaber-hook/shared/utils/typedefs.h"
#include <string_view>

#include "paper2_scotland2/shared/logger.hpp"

static constexpr auto Logger = Paper::ConstLoggerContext(MOD_ID);


#define INFO(str, ...) Paper::Logger::fmtLogTag<Paper::LogLevel::INF>(str, "Qosmetics Sabers" __VA_OPT__(, __VA_ARGS__))
#define ERROR(str, ...) Paper::Logger::fmtLogTag<Paper::LogLevel::ERR>(str, "Qosmetics Sabers" __VA_OPT__(, __VA_ARGS__))
#define WARNING(str, ...) Paper::Logger::fmtLogTag<Paper::LogLevel::WRN>(str, "Qosmetics Sabers" __VA_OPT__(, __VA_ARGS__))
#define CRITICAL(str, ...) Paper::Logger::fmtLogTag<Paper::LogLevel::CRIT>(str, "Qosmetics Sabers" __VA_OPT__(, __VA_ARGS__))
#define DEBUG(str, ...) Paper::Logger::fmtLogTag<Paper::LogLevel::DBG>(str, "Qosmetics Sabers" __VA_OPT__(, __VA_ARGS__))
// #define DEBUG(...)
