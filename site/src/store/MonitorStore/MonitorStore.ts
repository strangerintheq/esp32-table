import {SystemState} from "../../types/SystemState";

export interface MonitorStore {
    systemState?: SystemState;
    screenPoints?: [ number, number ][];
    targetPointIndex?: number;
    temperature?: number;
    wsConnected: boolean;
}