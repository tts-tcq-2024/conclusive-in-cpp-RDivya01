#include <gtest/gtest.h>
#include "typewise-alert.h"

// Test inferBreach function
TEST(TypeWiseAlertTestSuite, InfersTooLowBreach) {
    EXPECT_EQ(inferBreach(-1, 0, 35), TOO_LOW);
}

TEST(TypeWiseAlertTestSuite, InfersTooHighBreach) {
    EXPECT_EQ(inferBreach(36, 0, 35), TOO_HIGH);
}

TEST(TypeWiseAlertTestSuite, InfersNormalBreach) {
    EXPECT_EQ(inferBreach(30, 0, 35), NORMAL);
}

// Test getCoolingLimits function (indirectly via classifyTemperatureBreach)
TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachForPassiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 36), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(PASSIVE_COOLING, 30), NORMAL);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachForHiActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 46), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(HI_ACTIVE_COOLING, 40), NORMAL);
}

TEST(TypeWiseAlertTestSuite, ClassifiesTemperatureBreachForMedActiveCooling) {
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 41), TOO_HIGH);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, -1), TOO_LOW);
    EXPECT_EQ(classifyTemperatureBreach(MED_ACTIVE_COOLING, 38), NORMAL);
}

// Test sendToEmail function
TEST(TypeWiseAlertTestSuite, SendsLowTemperatureEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}

TEST(TypeWiseAlertTestSuite, SendsHighTemperatureEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST(TypeWiseAlertTestSuite, DoesNotSendNormalTemperatureEmail) {
    testing::internal::CaptureStdout();
    sendToEmail(NORMAL);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmail) {
    BatteryCharacter batteryChar = {HI_ACTIVE_COOLING, "BrandY"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, 46);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

// Test sendToController function
TEST(TypeWiseAlertTestSuite, SendsToController) {
    testing::internal::CaptureStdout();
    sendToController(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");
}

TEST(TypeWiseAlertTestSuite, SendsToControllerTooHigh) {
    testing::internal::CaptureStdout();
    sendToController(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");
}

TEST(TypeWiseAlertTestSuite, SendsToControllerNormal) {
    testing::internal::CaptureStdout();
    sendToController(NORMAL);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");
}

// Test checkAndAlert function
TEST(TypeWiseAlertTestSuite, CheckAndAlertToController) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, 36);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");  // TOO_HIGH corresponds to 1
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToControllerTooLow) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");  // TOO_LOW corresponds to 0
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToControllerNormal) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, 30);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");  // NORMAL corresponds to 2
}

