import { RefObject } from 'react';

type Point2D = [number, number];

export function draw(
    canvasRef: RefObject<HTMLCanvasElement | null>,
    points: Point2D[],
    targetPointIndex: number | null = null
): void {
    const canvas = canvasRef.current;
    if (!canvas || points.length === 0) return;

    const ctx = canvas.getContext('2d');
    if (!ctx) return;

    const maxRadius = (canvas.width / 2) - 40;

    ctx.save();

    ctx.clearRect(0, 0, canvas.width, canvas.height);

    ctx.translate(canvas.width / 2, canvas.height / 2);

    ctx.strokeStyle = '#e0e0e0';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(0, 0, maxRadius, 0, 2 * Math.PI);
    ctx.moveTo(-maxRadius, 0); ctx.lineTo(maxRadius, 0);
    ctx.moveTo(0, -maxRadius); ctx.lineTo(0, maxRadius);
    ctx.stroke();

    const toScreen = (pt: Point2D): Point2D => [
        pt[0] * maxRadius,
        -pt[1] * maxRadius
    ];

    ctx.strokeStyle = '#2196f3';
    ctx.lineWidth = 2;
    ctx.lineCap = 'round';
    ctx.lineJoin = 'round';
    ctx.beginPath();

    const [startX, startY] = toScreen(points[0]);
    ctx.moveTo(startX, startY);

    for (let i = 1; i < points.length; i++) {
        const [screenX, screenY] = toScreen(points[i]);
        ctx.lineTo(screenX, screenY);

        // Разделение цвета по индексу выполнения для Монитора
        if (targetPointIndex !== null && i === targetPointIndex) {
            ctx.stroke();
            ctx.beginPath();
            ctx.lineWidth = 1;
            ctx.strokeStyle = '#ff1961'; // Розовый — оставшийся путь
            ctx.moveTo(screenX, screenY);
        }
    }
    ctx.stroke();

    // 6. Отрисовка маркерных точек шага
    ctx.fillStyle = '#ff5722';
    points.forEach((pt) => {
        const [screenX, screenY] = toScreen(pt);
        ctx.beginPath();
        ctx.arc(screenX, screenY, 1.5, 0, 2 * Math.PI);
        ctx.fill();
    });

    ctx.restore();
}
