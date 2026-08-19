import {SystemState} from "../../types/SystemState";

export interface PreviewCanvasProps {
    points: [number, number][];
    targetPointIndex?: number | null;
    temp?: number;
    state?: SystemState;
}