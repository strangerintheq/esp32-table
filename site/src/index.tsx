import React from 'react';
import { createRoot } from 'react-dom/client';
import { NetworkSettings } from './pages/NetworkSettings/NetworkSettings';
import {Generator} from "./pages/Generator/Generator";
import { BrowserRouter, Routes, Route } from 'react-router-dom';
import { LiveMonitor } from './pages/Monitor/Monitor';
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
