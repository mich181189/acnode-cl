#include <gtest/gtest.h>
#include <core/Logger.h>
#include "MockLogger.h"

using testing::Return;

TEST(Logger, TestLogMacros)
{
	MockLogger logger;

	EXPECT_CALL(logger, shouldLog(Logger::LEVEL_INFO)).WillOnce(Return(true));
	EXPECT_CALL(logger, shouldLog(Logger::LEVEL_WARN)).WillOnce(Return(false));
	EXPECT_CALL(logger, logResult(Logger::LEVEL_INFO, "Meep"));

	LOG_INFO(&logger, "Meep");

	// should not log since it's not enabled
	LOG_WARN(&logger, "Meep");
}
