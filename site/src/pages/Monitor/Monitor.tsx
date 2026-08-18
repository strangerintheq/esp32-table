import React, { useEffect, useRef } from 'react';
import {useMonitorStore} from "../../store/MonitorStore/useMonitorStore";
import {PreviewCanvas} from "../../components/PreviewCanvas/PreviewCanvas";
import {sendRequest} from "../../utils/sendRequest";
import {API} from "../../API";

export function LiveMonitor() {
    const {
        screenPoints,
        targetPointIndex,
        temperature,
        systemState
    } = useMonitorStore();

    return <div className="box max-wide compact-form">
        <h2>Live Monitor</h2>

        <div style={{fontSize: '13px', color: '#546e7a', marginBottom: '12px', textAlign: 'center'}}>
            System Temperature: <b>{temperature}</b>°C
        </div>

        <div style={{fontSize: '13px', color: '#546e7a', marginBottom: '12px', textAlign: 'center'}}>
            System State: <b>{systemState}</b>
        </div>

        <PreviewCanvas
            points={screenPoints}
            targetPointIndex={targetPointIndex}
        />
        <div style={{display: 'flex', flexDirection: 'row', gap: 5}}>
            <button onClick={() => sendRequest(API.START)}>START</button>
            <button onClick={() => sendRequest(API.PAUSE)}>PAUSE</button>
            <button onClick={() => sendRequest(API.STOP)}>STOP</button>
        </div>

    </div>;
}
