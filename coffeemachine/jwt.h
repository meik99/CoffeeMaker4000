#ifndef COFFEE_JWT_H
#define COFFEE_JWT_H

#include "time.h"
#include <CustomJWT.h>
#include <ArduinoJson.h>
#include <string.h>

#include "credentials.h"

class Jwt {
private:
  unsigned long getTime() {
    time_t now;
    struct tm timeinfo;

    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
      return (0);
    }
    time(&now);

    return now;
  }

  void configureTime() {
    const char* ntpServer = "pool.ntp.org";
    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 3600;

    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  }

  String buildJwtToken() {
    String payload;
    StaticJsonDocument<4096> document;
    CustomJWT jwt(PRIVATE_KEY, 4096);

    unsigned long iat = getTime();
    unsigned long exp = iat + 3600;

    document["iss"] = SERVICE_ACCOUNT_EMAIL;
    document["sub"] = SERVICE_ACCOUNT_EMAIL;
    document["aud"] = AUDIENCE;
    document["iat"] = iat;
    document["exp"] = exp;

    serializeJson(document, payload);

    jwt.allocateJWTMemory();
    jwt.encodeJWT(strdup(payload.c_str()));

    String jwtToken = jwt.out;

    jwt.clear();

    return jwtToken;
  }
public:
  String getJwt() {
    configureTime();
    return buildJwtToken();
  }
};

#endif
