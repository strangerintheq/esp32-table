import React, { useEffect, useRef } from 'react';
import {draw} from "./draw";


interface PreviewCanvasProps {
    points: [number, number][];
    targetPointIndex?: number | null;
}

export function PreviewCanvas({points, targetPointIndex = null}) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);

    useEffect(() => {
        if (!points || points.length === 0)
            return;
        draw(canvasRef, points, targetPointIndex);
    }, [points, targetPointIndex]);

    return (
        <canvas
            ref={canvasRef}
            width={800}
            height={800}
            className="preview-canvas"
        />
    );
}
