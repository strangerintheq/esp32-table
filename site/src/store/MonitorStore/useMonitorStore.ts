import {create} from 'zustand';
import {MonitorStore} from "./MonitorStore";
import {API} from "../../API";

export const useMonitorStore = create<MonitorStore>((
    set,
    get
) => {
    let socket: WebSocket | null = null;
    let reconnectTimeout: ReturnType<typeof setTimeout> | null = null;

    const connect = () => {
        const gateway = `ws://${window.location.hostname}:${window.location.port}/ws`;
        console.log('Trying to open a WebSocket connection...');

        socket = new WebSocket(gateway);

        socket.onopen = () => {
            console.log('WebSocket connection opened');
            set({ wsConnected: true });
        };

        socket.onclose = () => {
            console.log('WebSocket connection closed');
            set({ wsConnected: false, targetPointIndex: null, systemState: null });
            // Автоматический реконнект каждые 2 секунды при потере связи
            reconnectTimeout = setTimeout(connect, 2000);
        };

        socket.onmessage = (event: MessageEvent) => {
            try {
                const data = JSON.parse(event.data);
                if (data.targetPointIndex !== undefined) {
                    set({ targetPointIndex: data.targetPointIndex });
                }
                if (data.temp !== undefined) {
                    set({ temperature: data.temp });
                }
                if (data.systemState !== undefined) {
                    set({ systemState: data.systemState });
                }
            } catch (err) {
                console.error('Failed to parse WebSocket JSON:', err);
            }
        };

        socket.onerror = (event: Event) => {
            console.error('WebSocket error observed:', event);
        };
    };

    connect();

    async function fetchCurrentPoints () {
        try {
            const res = await fetch(API.DOWNLOAD_POINTS, { method: 'POST' });
            if (!res.ok)
                throw new Error('Error fetching points from ESP32');

            // Читаем бинарный поток данных
            const buffer = await res.arrayBuffer();
            const rawPoints = new Float32Array(buffer);
            if (rawPoints.length === 0)
                return;
            const screenPoints: [number,number][] = [];
            for (let i = 0; i < rawPoints.length; i += 2) {
                const angle = rawPoints[i];
                const radius = rawPoints[i + 1];
                screenPoints.push([angle, radius]);
            }
            set({ screenPoints });
        } catch (err) {
            console.error('Failed to load buffer array:', err);
        }
    }


    fetchCurrentPoints()

    return {
        systemState: null,
        screenPoints: [],
        targetPointIndex: null,
        temperature: "?",
        wsConnected: false,

        fetchCurrentPoints
    };
});
