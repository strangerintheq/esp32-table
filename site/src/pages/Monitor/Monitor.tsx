import * as React from 'preact';
import {useMonitorStore} from "../../store/MonitorStore/useMonitorStore";
import {PreviewCanvas} from "../../components/PreviewCanvas/PreviewCanvas";
import {sendRequest} from "../../utils/sendRequest";
import {API} from "../../types/API";
import {SystemState} from "../../types/SystemState";

export function LiveMonitor() {
    const {
        screenPoints,
        targetPointIndex,
        temperature,
        systemState
    } = useMonitorStore();

    return <div className="box max-wide compact-form">
        <h2>Live Monitor</h2>

        <PreviewCanvas
            points={screenPoints}
            targetPointIndex={targetPointIndex}
            state={systemState}
            temp={temperature}
        />
        <div style={{display: 'flex', flexDirection: 'row', gap: 5}}>
            {canStart(systemState) && <button onClick={() => sendRequest(API.START)}>START</button>}
            {canPause(systemState) && <button onClick={() => sendRequest(API.PAUSE)}>PAUSE</button>}
            {canStop(systemState) && <button onClick={() => sendRequest(API.STOP)}>STOP</button>}
            {canClear(systemState) && <button onClick={() => sendRequest(API.CLEAR)}>CLEAR</button>}
        </div>

    </div>;
}

function canStart(systemState?: SystemState) {
    return systemState === SystemState.PAUSED || systemState == SystemState.IDLE
}

function canPause(systemState?: SystemState) {
    return systemState === SystemState.RUNNING
}

function canStop(systemState?: SystemState) {
    return systemState === SystemState.PAUSED || systemState === SystemState.RUNNING;
}

function canClear(systemState?: SystemState) {
    return systemState === SystemState.COMPLETED || systemState === SystemState.ERROR;
}
