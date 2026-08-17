import React, { useEffect, useRef } from 'react';
import {useMonitorStore} from "../../store/monitor/useMonitorStore";
import {PreviewCanvas} from "../../components/preview/PreviewCanvas";

export function LiveMonitor() {
    const {
        screenPoints,
        targetPointIndex,
        temperature,
    } = useMonitorStore();

    return <div className="box max-wide compact-form">
        <h2>Live Monitor</h2>

        <div style={{fontSize: '13px', color: '#546e7a', marginBottom: '12px', textAlign: 'center'}}>
            System Temperature: <b>{temperature}</b>°C
        </div>

        <PreviewCanvas
            points={screenPoints}
            targetPointIndex={targetPointIndex}
        />

    </div>;
}
