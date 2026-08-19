import { RefObject } from 'preact';
import {PreviewCanvasProps} from "./PreviewCanvasProps";
import {SystemState} from "../../types/SystemState";

const traversedPathColor = '#2196f3';
const remainingPathColor = '#b7b7b7';

export function draw(
    canvasRef: RefObject<HTMLCanvasElement | null>,
    props: PreviewCanvasProps
): void {
    const canvas = canvasRef.current;
    if (!canvas) return;
    const ctx = canvas.getContext('2d');
    if (!ctx) return;
    const maxRadius = (canvas.width / 2) - 40;
    ctx.save();
    ctx.clearRect(0, 0, canvas.width, canvas.height);
    ctx.translate(canvas.width / 2, canvas.height / 2);
    drawBg(ctx, maxRadius);
    drawLine(ctx, props, maxRadius);
    drawPoints(ctx, props, maxRadius);
    drawState(ctx, props.state)
    drawTemp(ctx, props.temp)
    ctx.restore();
}

function drawBg(ctx: CanvasRenderingContext2D, maxRadius: number) {
    ctx.strokeStyle = '#e0e0e0';
    ctx.lineWidth = 2;
    ctx.beginPath();
    ctx.arc(0, 0, maxRadius * 1.05, 0, 2 * Math.PI);
    // ctx.moveTo(-maxRadius, 0); ctx.lineTo(maxRadius, 0);
    // ctx.moveTo(0, -maxRadius); ctx.lineTo(0, maxRadius);
    ctx.stroke();
}

function drawLine(ctx: CanvasRenderingContext2D, props: PreviewCanvasProps, maxRadius: number) {
    const {points,targetPointIndex} = props;
    if (!points || points.length === 0)
        return
    ctx.strokeStyle = targetPointIndex ? traversedPathColor : remainingPathColor;
    ctx.lineWidth = 3;
    ctx.lineCap = 'round';
    ctx.lineJoin = 'round';
    ctx.beginPath();
    ctx.moveTo(points[0][0] * maxRadius, points[0][1] * maxRadius);
    for (let i = 1; i < points.length; i++) {
        const [x, y] = points[i]
        ctx.lineTo(x * maxRadius, y * maxRadius);
        if (targetPointIndex !== null && i === targetPointIndex) {
            ctx.stroke();
            ctx.beginPath();
            ctx.lineWidth = 1;
            ctx.strokeStyle = remainingPathColor;
            ctx.moveTo(x * maxRadius, y * maxRadius);
        }
    }
    ctx.stroke();
}

function drawPoints(ctx: CanvasRenderingContext2D, props: PreviewCanvasProps, maxRadius: number) {
    const {points, targetPointIndex} = props;
    if (!points)
        return
    ctx.fillStyle = '#ff5722';
    points.forEach(([x, y], i) => {
        if (targetPointIndex && i < targetPointIndex)
            return
        ctx.beginPath();
        ctx.arc(x*maxRadius, y*maxRadius, 2, 0, 2 * Math.PI);
        ctx.fill();
    });
}

function drawState(ctx: CanvasRenderingContext2D, state?: SystemState) {
    if (!state)
        return
    ctx.fillStyle = "black"
    ctx.font = '28px Arial'
    ctx.fillText(state, -ctx.canvas.width/2+20,-ctx.canvas.height/2+40)
}

function drawTemp(ctx: CanvasRenderingContext2D, temp?: number) {
    if (!temp)
        return
    ctx.fillStyle = "black"
    ctx.font = '28px Arial'
    ctx.fillText(temp+"°C", ctx.canvas.width/2-70,-ctx.canvas.height/2+40)
}