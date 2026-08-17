export interface MonitorStore {
    screenPoints: { x: number; y: number }[];
    targetPointIndex: number | null;
    temperature: string | null;
    wsConnected: boolean;

    fetchCurrentPoints: (cx: number, cy: number, maxRadius: number) => Promise<void>;
}