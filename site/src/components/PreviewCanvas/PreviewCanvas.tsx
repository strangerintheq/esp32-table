import React, {useEffect, useRef} from 'react';
import {draw} from "./draw";
import {PreviewCanvasProps} from "./PreviewCanvasProps";

export function PreviewCanvas(props: PreviewCanvasProps) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);

    useEffect(() => {
        draw(canvasRef, props);
    }, [props.points, props.targetPointIndex, props.state, props.temp]);

    return (
        <canvas
            ref={canvasRef}
            width={800}
            height={800}
            className="preview-canvas"
        />
    );
}
