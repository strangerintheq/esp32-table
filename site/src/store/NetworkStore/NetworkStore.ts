interface NetworkStoreState {
    network_mode?: string;
    wifi_ssid?: string;
    wifi_password?: string;
    ap_ssid?: string;
    ap_password?: string;
}

interface NetworkStore extends NetworkStoreState {
    isLoading?: boolean;
    isSaved?: boolean;
    error?: string;
    toggleMode: () => void;
    setField: (field: keyof NetworkStoreState, value: string) => void;
    fetchSettings: () => Promise<void>;
    saveSettings: () => Promise<void>;
}
