import * as React from 'preact';
import {draw} from "./draw";
import {PreviewCanvasProps} from "./PreviewCanvasProps";
import {useEffect, useRef} from "preact/compat";

export function PreviewCanvas(props: PreviewCanvasProps) {
    const canvasRef = useRef<HTMLCanvasElement | null>(null);

    useEffect(() => {
        draw(canvasRef, props);
    }, [props.points, props.targetPointIndex, props.leftText, props.rightText]);

    return (
        <canvas
            ref={canvasRef}
            width={800}
            height={800}
            className="preview-canvas"
        />
    );
}
