import { create } from 'zustand';

export const useNetworkStore = create<NetworkStore>((
    set,
    get
) => ({

    toggleMode() {
        set((s) => ({ isAP: !s.isAP }))
    },

    setField(field, value){
        set({ [field]: value })
    },

    fetchSettings: async () => {
        set({ isLoading: true, error: null });
        try {
            const res = await fetch('/network/get', { method: 'POST' });
            if (!res.ok)
                throw new Error('Failed to fetch network settings');
            const data = await res.json();
            set({
                isAP: data.network_mode !== 'wifi',
                wifi_ssid: data.wifi_ssid || '',
                wifi_pass: data.wifi_password || '',
                ap_ssid: data.ap_ssid || '',
                ap_pass: data.ap_password || '',
                isLoading: false
            });
        } catch (err) {
            set({ error: err.message, isLoading: false });
        }
    },

    saveSettings: async () => {
        set({ isLoading: true, error: null });
        const { isAP, wifi_ssid, wifi_pass, ap_ssid, ap_pass } = get();
        const params = new URLSearchParams({
            mode: isAP ? 'ap' : 'wifi', wifi_ssid, wifi_pass, ap_ssid, ap_pass
        });
        const res = await fetch('/network/set', {
            method: 'POST',
            headers: { 'Content-Type': 'application/x-www-form-urlencoded' },
            body: params.toString()
        });
        set(res.ok ? { isSaved: true, isLoading: false } : { error: 'Failed to save settings', isLoading: false });
    }
}));
