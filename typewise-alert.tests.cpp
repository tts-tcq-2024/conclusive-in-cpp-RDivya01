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

// Test checkAndAlert function with TO_EMAIL
TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailTooHigh) {
    BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, 41); // This should result in a TOO_HIGH email
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too high\n");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailTooLow) {
    BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, -1); // This should result in a TOO_LOW email
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "To: a.b@c.com\nHi, the temperature is too low\n");
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToEmailNormal) {
    BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_EMAIL, batteryChar, 30); // This should not produce any output
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, ""); // Ensure no output for NORMAL
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertInvalidTarget) {
    BatteryCharacter batteryChar = {MED_ACTIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(static_cast<AlertTarget>(999), batteryChar, 30); // Trigger invalid case
    // No specific output to check, just ensuring the branch is covered
}

// Test sendToController function
TEST(TypeWiseAlertTestSuite, SendsToControllerNormal) {
    testing::internal::CaptureStdout();
    sendToController(NORMAL);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");  // NORMAL corresponds to 0
}

TEST(TypeWiseAlertTestSuite, SendsToControllerTooLow) {
    testing::internal::CaptureStdout();
    sendToController(TOO_LOW);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");  // TOO_LOW corresponds to 1
}

TEST(TypeWiseAlertTestSuite, SendsToControllerTooHigh) {
    testing::internal::CaptureStdout();
    sendToController(TOO_HIGH);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");  // TOO_HIGH corresponds to 2
}

// Test checkAndAlert function
TEST(TypeWiseAlertTestSuite, CheckAndAlertToController) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, 36);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 2\n");  // TOO_HIGH corresponds to 2
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToControllerTooLow) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, -1);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 1\n");  // TOO_LOW corresponds to 1
}

TEST(TypeWiseAlertTestSuite, CheckAndAlertToControllerNormal) {
    BatteryCharacter batteryChar = {PASSIVE_COOLING, "BrandX"};
    testing::internal::CaptureStdout();
    checkAndAlert(TO_CONTROLLER, batteryChar, 30);
    std::string output = testing::internal::GetCapturedStdout();
    EXPECT_EQ(output, "feed : 0\n");  // NORMAL corresponds to 0
}

// Test getCoolingLimits with an invalid CoolingType
TEST(TypeWiseAlertTestSuite, GetCoolingLimitsWithInvalidCoolingType) {
    int lowerLimit = 0;
    int upperLimit = 0;
    getCoolingLimits(static_cast<CoolingType>(999), &lowerLimit, &upperLimit);  // Assuming 999 is an invalid type
    EXPECT_EQ(lowerLimit, 0);  // Assuming default value for invalid type
    EXPECT_EQ(upperLimit, 40); // Assuming default value for invalid type
}
