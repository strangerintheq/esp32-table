import {WebSocketServer} from 'ws';
import { writeFile } from 'node:fs/promises';
import {readFileSync} from 'fs'

const pointsBin = 'mock/points.bin';
const networkJson = 'mock/network.json';

const networkSettings = JSON.parse(readFileSync(networkJson).toString())
let points = readFileSync(pointsBin);

let targetPointIndex = 0;
let emit;

setInterval(() => {
    if (points.byteLength === 0)
        return;
    targetPointIndex = (targetPointIndex + 1) % (points.byteLength / 8);
    emit && emit(JSON.stringify({targetPointIndex}))
}, 100)

const wss = new WebSocketServer({port: 8089});

wss.on('connection', (ws) => {
    console.log('ws connected');
    emit = (obj) => ws.send(obj)
    ws.on('close', () => {
        emit = null
        console.log('ws disconnected');
    });
});

export default function serverPlugin() {
    return {
        name: 'server-plugin',
        configureServer(server) {
            post(server, '/points', () => {
                return points
            });
            post(server, '/upload', async (buf) => {
                points = buf
                await writeFile(pointsBin, Buffer.from(buf));
                targetPointIndex = 0
                return {result: 'ok'}
            });
            post(server, '/network/get', () => {
                return networkSettings;
            });
            post(server, '/network/set', async x => {
                Object.assign(networkSettings, x);
                console.log(networkSettings)
                await writeFile(networkJson, JSON.stringify(x, null ,4))
                return {result: 'ok'}
            });
        }
    };
}

function post(server, route, callback) {
    server.middlewares.use(route, async (req, res) => {
        if (req.method === 'POST') {
            res.statusCode = 200;
            const chunks = [];
            for await (const chunk of req) {
                chunks.push(chunk);
            }
            const contentType = req.headers['content-type'] || '';
            let body;
            const applicationOctetStream = 'application/octet-stream';
            const applicationJson = 'application/json';
            if (contentType.startsWith(applicationOctetStream)) {
                const b = Buffer.concat(chunks), i = b.byteOffset;
                body = b.buffer.slice(i, i + b.byteLength);
            } else if (contentType.startsWith(applicationJson)) {
                body = JSON.parse(chunks.join(""))
            }
            let response = callback(body);
            if (response instanceof ArrayBuffer) {
                response = Buffer.from(response);
            } else if (typeof response === "object" && response !== null && !Buffer.isBuffer(response)) {
                response = JSON.stringify(response);
            }
            res.setHeader('Content-Type', typeof response === "string" ?
                applicationJson : applicationOctetStream);
            res.end(response);
        }
    });
}

