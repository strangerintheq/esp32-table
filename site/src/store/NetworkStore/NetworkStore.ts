interface NetworkStoreState {
    isAP?: boolean;
    wifi_ssid?: string;
    wifi_pass?: string;
    ap_ssid?: string;
    ap_pass?: string;
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
