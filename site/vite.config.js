import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import serverPlugin from "./mock/serverMock.js";
import { viteCommonjs } from '@originjs/vite-plugin-commonjs';

export default defineConfig({
    plugins: [
        viteCommonjs(),
        react({
            include: "**/*.tsx",
        }),
        serverPlugin()
    ],
    server: {
        proxy: {
            '/ws': {
                target: 'ws://localhost:8080',
                ws: true,
                changeOrigin: true,
                rewrite: (path) => path.replace(/^\/ws/, ''),
            },
        },
    },
    resolve: {
        alias: {
            'react': 'preact/compat',
            'react-dom/test-utils': 'preact/test-utils',
            'react-dom': 'preact/compat',
            'react/jsx-runtime': 'preact/jsx-runtime',
        },
    },
});

