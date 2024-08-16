#include "typewise-alert.h"
#include <stdio.h>

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
  if(value < lowerLimit) {
    return TOO_LOW;
  }
  if(value > upperLimit) {
    return TOO_HIGH;
  }
  return NORMAL;
}

void getCoolingLimits(CoolingType coolingType, int* lowerLimit, int* upperLimit) {
  *lowerLimit = 0;
  
  if (coolingType == PASSIVE_COOLING) {
    *upperLimit = 35;
  } else if (coolingType == HI_ACTIVE_COOLING) {
    *upperLimit = 45;
  } else if (coolingType == MED_ACTIVE_COOLING) {
    *upperLimit = 40;
  }
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
  int lowerLimit = 0, upperLimit = 0;
  getCoolingLimits(coolingType, &lowerLimit, &upperLimit);
  return inferBreach(temperatureInC, lowerLimit, upperLimit);
}

void checkAndAlert(
    AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {

  BreachType breachType = classifyTemperatureBreach(
    batteryChar.coolingType, temperatureInC
  );

  switch(alertTarget) {
    case TO_CONTROLLER:
      sendToController(breachType);
      break;
    case TO_EMAIL:
      sendToEmail(breachType);
      break;
  }
}

void sendToController(BreachType breachType) {
  const unsigned short header = 0xfeed;
  printf("%x : %x\n", header, breachType);
}

void sendLowOrHighEmail(BreachType breachType, const char* recepient) {
  const char* message = breachType == TOO_LOW ? 
                        "Hi, the temperature is too low\n" : 
                        "Hi, the temperature is too high\n";
  printf("To: %s\n%s", recepient, message);
}

void sendToEmail(BreachType breachType) {
  const char* recepient = "a.b@c.com";
  if (breachType != NORMAL) {
    sendLowOrHighEmail(breachType, recepient);
  }
}
