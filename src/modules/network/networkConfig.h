#ifndef NETWORK_CONFIG_H
#define NETWORK_CONFIG_H

#include <utils/ConfigurationParameter.h>

extern ConfigurationParameter* network_mode;
extern ConfigurationParameter* wifi_ssid;
extern ConfigurationParameter* wifi_password;
extern ConfigurationParameter* ap_ssid;
extern ConfigurationParameter* ap_password;

void initNetworkConfiguration();

#endif // NETWORK_CONFIG_H
