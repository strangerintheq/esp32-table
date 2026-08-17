import {create} from 'zustand';
import {MonitorStore} from "./monitorStore";

export const useMonitorStore = create<MonitorStore>((set) => {
    let socket: WebSocket | null = null;
    let reconnectTimeout: ReturnType<typeof setTimeout> | null = null;

    const connect = () => {
        // В продакшене window.location.hostname автоматически подставит IP вашей ESP32
        const gateway = `ws://${window.location.hostname}/ws`;
        console.log('Trying to open a WebSocket connection...');

        socket = new WebSocket(gateway);

        socket.onopen = () => {
            console.log('WebSocket connection opened');
            set({ wsConnected: true });
        };

        socket.onclose = () => {
            console.log('WebSocket connection closed');
            set({ wsConnected: false, targetPointIndex: null });
            // Автоматический реконнект каждые 2 секунды при потере связи
            reconnectTimeout = setTimeout(connect, 2000);
        };

        socket.onmessage = (event: MessageEvent) => {
            try {
                const data = JSON.parse(event.data);
                console.log('Data received via WebSocket:', data);

                if (data.targetPoinitIndex !== undefined) {
                    set({ targetPointIndex: data.targetPoinitIndex });
                }
                if (data.temp !== undefined) {
                    set({ temperature: data.temp });
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

    return {
        screenPoints: [],
        targetPointIndex: null,
        temperature: "?",
        wsConnected: false,

        fetchCurrentPoints: async (cx: number, cy: number, maxRadius: number) => {
            try {
                const res = await fetch('/points', { method: 'POST' });
                if (!res.ok) throw new Error('Error fetching points from ESP32');

                // Читаем бинарный поток данных
                const buffer = await res.arrayBuffer();
                const rawPoints = new Float32Array(buffer);
                if (rawPoints.length === 0) return;

                const calculatedPoints: { x: number; y: number }[] = [];

                // Итерируемся по парам [угол, радиус]
                for (let i = 0; i < rawPoints.length; i += 2) {
                    const angle = rawPoints[i];
                    const radius = rawPoints[i + 1];

                    // Проекция полярных координат в декартовы координаты холста
                    const drawX = cx + (radius * maxRadius) * Math.cos(angle);
                    const drawY = cy + (radius * maxRadius) * Math.sin(angle);

                    calculatedPoints.push({ x: drawX, y: drawY });
                }

                set({ screenPoints: calculatedPoints });
            } catch (err) {
                console.error('Failed to load buffer array:', err);
            }
        }
    };
});
