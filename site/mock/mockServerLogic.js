import {readFileSync} from "fs";
import {writeFile} from "node:fs/promises";
import {SystemState} from "../src/types/SystemState";

export function mockServerLogic() {
    let systemState = SystemState.INITIALIZING
    const pointsBin = 'mock/points.bin';
    const networkJson = 'mock/network.json';

    const networkSettings = JSON.parse(readFileSync(networkJson).toString())
    let points = readFileSync(pointsBin);

    let targetPointIndex = null;
    let emit;
    let outgoingMsg = {}

    setInterval(() => {
        if (systemState === SystemState.INITIALIZING) {
            updateSystemState(SystemState.IDLE)
        } else if (systemState === SystemState.STARTING) {
            updateSystemState(SystemState.RUNNING)
        } else if (systemState === SystemState.PAUSING) {
            updateSystemState(SystemState.PAUSED)
        } else if (systemState === SystemState.STOPPING) {
            updateSystemState(SystemState.IDLE)
            updateTargetPointIndex(null)
        } else if (systemState === SystemState.UNPAUSING) {
            updateSystemState(SystemState.RUNNING)
        }
    }, 1000)

    setInterval(() => {
        if (systemState === SystemState.RUNNING) {
            if (targetPointIndex >= points.byteLength/8) {
                updateSystemState(SystemState.COMPLETED)
                updateTargetPointIndex(null)
            } else {
                updateTargetPointIndex(targetPointIndex + 1);
            }
        }
        emit && Object.keys(outgoingMsg).length && emit(JSON.stringify(outgoingMsg))
        outgoingMsg = {}
    }, 100)

    function updateTargetPointIndex(x) {
        targetPointIndex = x
        outgoingMsg.targetPointIndex = targetPointIndex
    }

    function updateSystemState(state) {
        systemState = state;
        outgoingMsg.systemState = state;
    }

    const signalHandlers = {
        start() {
            if (systemState === SystemState.IDLE)
                updateSystemState(SystemState.STARTING)
            if (systemState === SystemState.PAUSED)
                updateSystemState(SystemState.UNPAUSING)
        },
        stop() {
            if (systemState === SystemState.RUNNING || systemState === SystemState.PAUSED)
                updateSystemState(SystemState.STOPPING)
        },
        pause() {
            if (systemState === SystemState.RUNNING)
                updateSystemState(SystemState.PAUSING)
        },
        clear() {
            if (systemState === SystemState.COMPLETED || systemState === SystemState.ERROR)
                updateSystemState(SystemState.PAUSING)
        },
        reboot() {
           updateSystemState(SystemState.INITIALIZING)
        }
    }

    async function startTask() {
        setInterval(() => {
            if (points.byteLength === 0)
                return;
            targetPointIndex = (targetPointIndex + 1) % (points.byteLength / 8);
            emit && emit(JSON.stringify({targetPointIndex}))
        }, 100)
    }

    return {
        setEmit(callback) {
            emit = callback
        },
        async uploadPoints(buf) {
            points = buf;
            console.log('upload points', buf.byteLength/8)
            await writeFile(pointsBin, Buffer.from(buf));
            targetPointIndex = 0;
        },
        getNetworkSettings() {
            return networkSettings;
        },
        async setNetworkSettings(settings) {
            Object.assign(networkSettings, settings);
            console.log('update network settings', networkSettings)
            await writeFile(networkJson, JSON.stringify(settings, null ,4))
        },
        getPoints() {
            return points;
        },
        sendSignal(signal) {
            signalHandlers[signal.toLowerCase()]()
        },
        getSystemState(){
            return systemState
        }
    }
}