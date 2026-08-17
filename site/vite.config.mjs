import { defineConfig } from 'vite';
import react from '@vitejs/plugin-react';
import serverPlugin from "./mock/serverMock.js";
import { viteCommonjs } from '@originjs/vite-plugin-commonjs';
import { visualizer } from 'rollup-plugin-visualizer';

const preactReactRouterFix = () => ({
    name: 'preact-react-router-fix',
    enforce: 'pre',
    transform(code, id) {
        if (id.includes('preact/compat') || id.includes('preact-compat')) {
            return {
                code: code + `
                    export const useOptimistic = () => { return [{}, () => {}]; };
                    export const use = (promise) => { 
                        if (promise && typeof promise.then === 'function') {
                            if (promise.status === 'fulfilled') return promise.value;
                            if (promise.status === 'rejected') throw promise.reason;
                            throw promise;
                        }
                        return promise;
                    };
                `,
                map: null
            };
        }
    }
});

export default defineConfig({
    build: {
        // Change the output directory (default is 'dist')
        outDir: '../data/site/', 
    },
    plugins: [
        preactReactRouterFix(),
        viteCommonjs(),
        react({
            include: "**/*.tsx",
        }),
        serverPlugin(),
        visualizer({
            filename: 'stats.html', // Файл появится в папке site
            open: false, // Выключаем автоматическое открытие в браузере (чтобы не ломать консоль PIO)
            gzipSize: true, // Покажет размер и в сжатом виде тоже
            brotliSize: true
        })
    ],
    server: {
        proxy: {
            '/ws': {
                target: 'ws://localhost:8089',
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

