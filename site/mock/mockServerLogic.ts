import {readFileSync} from "fs";
import {writeFile} from "node:fs/promises";
import {SystemState} from "../src/types/SystemState";
import {WsMessage} from "../src/types/WsMessage";

export function mockServerLogic() {
    let systemState = SystemState.INITIALIZING
    const pointsBin = 'mock/points.bin';
    const networkJson = 'mock/network.json';

    const networkSettings = JSON.parse(readFileSync(networkJson).toString())
    let points = readFileSync(pointsBin);

    let targetPointIndex = null;
    let emit;
    let outgoingMsg : WsMessage = {}

    setInterval(async () => {
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
        } else if (systemState === SystemState.REBOOTING) {
            await new Promise((resolve) => setTimeout(resolve,900))
            console.log('reboot')
            updateSystemState(SystemState.IDLE)
        }

        updateTemp();
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

    function updateTemp(){
        if (Math.random()<0.9)
            return
        outgoingMsg.temp = 50 + (Math.random()*5|0)
    }

    function updateTargetPointIndex(x) {
        targetPointIndex = x
        outgoingMsg.targetPointIndex = targetPointIndex;
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
           updateSystemState(SystemState.REBOOTING)
        }
    }

    return {
        setEmit(callback) {
            emit = callback
            callback && callback(JSON.stringify({
                systemState,
                targetPointIndex,
                temp: 52
            }))
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
            // updateSystemState(SystemState.REBOOTING)
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