import {create} from 'zustand';
import {GeneratorStore} from "./generatorStore";
import {API} from "../../API";

const RANDOM_FORMULAS = [
    'Math.sin(2 * a)',
    'Math.cos(3 * a)',
    '1 + 0.5 * Math.sin(6 * a)',
    'a * 0.05', // Изменен шаг для спирали
    'Math.sin(4 * a) + Math.cos(2 * a)'
];

export const useGeneratorStore = create<GeneratorStore>((set, get) => ({
    formula: 'Math.sin(2 * a)',
    turns: 3,
    fixedStep: 0.05, // Дефолтное расстояние между точками
    points: [],
    isLoading: false,
    status: null,
    isError: false,
    calculatedPointsCount: 0,

    setField: (field, value) => {
        set({ [field]: value });
        get().generatePoints();
    },

    setRandomFormula: () => {
        const randomIndex = Math.floor(Math.random() * RANDOM_FORMULAS.length);
        set({ formula: RANDOM_FORMULAS[randomIndex] });
        get().generatePoints();
    },

    generatePoints: () => {
        const { formula, turns, fixedStep } = get();
        const generated: [number, number][] = [];

        const maxAngle = turns * 2 * Math.PI;

        // Микро-шаг для численного поиска следующей точки
        const microStep = 0.0005;
        // Защита от вечного цикла, если шаг слишком мал, а витков много
        const maxPointsLimit = 3000;

        try {
            const evalFormula = new Function('a', `return ${formula};`);

            // Функция вычисления декартовых координат для угла a
            const getCartesian = (angle: number): [number, number] => {
                const r = evalFormula(angle);
                return [r * Math.cos(angle), r * Math.sin(angle)];
            };

            // Добавляем самую первую точку при a = 0
            let currentAngle = 0;
            let [lastX, lastY] = getCartesian(currentAngle);
            generated.push([lastX, lastY]);

            // Цикл генерации точек с фиксированным линейным шагом
            while (currentAngle < maxAngle && generated.length < maxPointsLimit) {
                let distance = 0;

                // Наращиваем угол микро-шагами, пока не пройдем расстояние fixedStep
                while (distance < fixedStep && currentAngle < maxAngle) {
                    currentAngle += microStep;
                    const [nextX, nextY] = getCartesian(currentAngle);

                    // Считаем декартово расстояние (теорема Пифагора)
                    const dx = nextX - lastX;
                    const dy = nextY - lastY;
                    distance = Math.sqrt(dx * dx + dy * dy);
                }

                // Запоминаем найденную точку, если мы не вышли за границы углов
                if (currentAngle < maxAngle) {
                    const [targetX, targetY] = getCartesian(currentAngle);
                    generated.push([targetX, targetY]);
                    lastX = targetX;
                    lastY = targetY;
                }
            }

            set({
                points: generated,
                calculatedPointsCount: generated.length,
                status: null,
                isError: false
            });
        } catch (err) {
            set({ points: [], calculatedPointsCount: 0, status: 'Invalid formula syntax', isError: true });
        }
    },

    uploadPoints: async () => {
        const { points } = get();
        if (points.length === 0)
            return;

        set({ isLoading: true, status: 'Uploading data to ESP32...', isError: false });

        const binaryData = new Float32Array(points.flat());
        const body = binaryData.buffer;
        const headers = {'Content-Type': 'application/octet-stream' }
        const method = "POST"
        const response = await fetch(API.UPLOAD_POINTS, {method, body, headers});
        set({
            isLoading: false,
            status: response.ok ? 'Success! Pattern sent to ESP32' : 'Failed to upload coordinates',
            isError: !response.ok
        });
    }
}));
