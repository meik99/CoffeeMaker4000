#ifndef COFFEE_JWT_H
#define COFFEE_JWT_H

#include <CustomJWT.h>

#include "SPIFFS.h"

#include "credentials.h"

class Jwt {
  private:
  String readServiceAccount() {
    File serviceAccountFile = SPIFFS.open("/service-account.json");

    if (!serviceAccountFile) {
      Serial.println("service-account.json file does not exist");
      return "";
    }

    String text = serviceAccountFile.readString();
    
    serviceAccountFile.close();

    return text;
  }
  
  public:
  void getJwt() {
    Serial.println("deriving JWT token");
    
    String serviceAccountText = readServiceAccount();
    CustomJWT jwt(PRIVATE_KEY_ID, 4096);

    jwt.allocateJWTMemory();
    
    /*
    {
  "iss": "123456-compute@developer.gserviceaccount.com",
  "sub": "123456-compute@developer.gserviceaccount.com",
  "aud": "https://firestore.googleapis.com/",
  "iat": 1511900000,
  "exp": 1511903600
}
    */
    
    Serial.println(serviceAccountText);
  }
};

#endif
