import {WebSocketServer} from 'ws';
import { writeFile } from 'node:fs/promises';
import {readFileSync} from 'fs'
import {API} from "../src/API.js";
import {mockServerLogic} from "./mockServerLogic.js";

export default function serverPlugin() {

    const mock = mockServerLogic();
    const wss = new WebSocketServer({port: 8089});

    wss.on('connection', (ws) => {
        console.log('ws connected');
        mock.setEmit((obj) => ws.send(obj))
        ws.on('close', () => {
            mock.setEmit(null);
            console.log('ws disconnected');
        });
        ws.send(JSON.stringify({systemState: mock.getSystemState()}))
    });
    return {
        name: 'server-plugin',
        configureServer(server) {
            const OK = {result: 'ok'};
            post(server, API.DOWNLOAD_POINTS, () => {
                return mock.getPoints();
            });
            post(server, API.UPLOAD_POINTS, async (buf) => {
                await mock.uploadPoints(buf);
                return OK
            });
            post(server, API.NETWORK_GET, () => {
                return mock.getNetworkSettings();
            });
            post(server, API.NETWORK_SET, async x => {
                await mock.setNetworkSettings(x)
                return OK
            });
            Object.entries(API)
                .filter(([_, route]) => route.includes("signal"))
                .forEach(([name, route]) => {
                    post(server, route, () => {
                        mock.sendSignal(name);
                        return OK
                    });
                })
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

