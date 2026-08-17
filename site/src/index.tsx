import React from 'react';
import { createRoot } from 'react-dom/client';
import { NetworkSettings } from './pages/network/NetworkSettings';
import {Generator} from "./pages/generator/Generator";
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { LiveMonitor } from './pages/monitor/Monitor';
import {AppLayout} from "./components/AppLayout/AppLayout";

import "./styles/index.css";

const root = document.querySelector("#root");
createRoot(root).render(<App />);

function App() {
    return <BrowserRouter>
        <Routes>
            <Route path="/" element={<AppLayout />}>
                <Route index element={<LiveMonitor />} />
                <Route path="generator" element={<Generator />} />
                <Route path="network" element={<NetworkSettings />} />
            </Route>
        </Routes>
    </BrowserRouter>;
}
