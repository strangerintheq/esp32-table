#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif
    uint8_t temprature_sens_read();
#ifdef __cplusplus
}
#endif


int getTemp() {
    // Fahrenheit -> Celsuis
    return (temprature_sens_read() - 32) / 1.8;
}
