#include <utils/ConfigurationParameter.h>

ConfigurationParameter* network_mode = nullptr;
ConfigurationParameter* wifi_ssid = nullptr;
ConfigurationParameter* wifi_password = nullptr;
ConfigurationParameter* ap_ssid = nullptr;
ConfigurationParameter* ap_password = nullptr;

void initNetworkConfiguration() {
    network_mode = new ConfigurationParameter("network.mode");
    wifi_ssid = new ConfigurationParameter("wifi.ssid");
    wifi_password = new ConfigurationParameter("wifi.password");
    ap_ssid = new ConfigurationParameter("ap.ssid");
    ap_password = new ConfigurationParameter("ap.password"); 
}
