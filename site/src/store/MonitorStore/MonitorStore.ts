export interface MonitorStore {
    systemState: string;
    screenPoints: [ number, number ][];
    targetPointIndex: number | null;
    temperature: string | null;
    wsConnected: boolean;

    fetchCurrentPoints: () => Promise<void>;
}