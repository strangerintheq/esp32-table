import { create } from 'zustand';
import {API} from "../../types/API";
import {sendRequest} from "../../utils/sendRequest";
import {NetworkSettings} from "../../types/NetworkSettings";

export const useNetworkStore = create<NetworkStore>((
    set,
    get
) => {
    fetchSettings()
    async function fetchSettings () {
        set({isLoading: true, error: null});
        try {
            const data = await sendRequest<NetworkSettings>(API.NETWORK_GET);
            set({
                network_mode: data.network_mode,
                wifi_ssid: data.wifi_ssid,
                wifi_password: data.wifi_password,
                ap_ssid: data.ap_ssid,
                ap_password: data.ap_password,
                isLoading: false
            });
        } catch (err) {
            set({error: err.message, isLoading: false});
        }
    }

    return ({

        toggleMode() {
            set((s) => ({network_mode: s.network_mode === "ap" ? "wifi" : "ap"}))
        },

        setField(field, value) {
            set({[field]: value})
        },


        saveSettings: async () => {
            set({isLoading: true, error: null});
            const {network_mode, wifi_ssid, wifi_password, ap_ssid, ap_password} = get();
            const res = await fetch(API.NETWORK_SET, {
                method: 'POST',
                headers: {'Content-Type': 'application/json'},
                body: JSON.stringify({
                    network_mode,
                    wifi_ssid,
                    wifi_password,
                    ap_ssid,
                    ap_password
                })
            });
            set(res.ok ? {isSaved: true, isLoading: false} : {error: 'Failed to save settings', isLoading: false});
        }
    });
});
