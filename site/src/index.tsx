import * as React from 'preact';
import { NetworkSettings } from './pages/NetworkSettings/NetworkSettings';
import {Generator} from "./pages/Generator/Generator";
import { LiveMonitor } from './pages/Monitor/Monitor';
import {AppLayout} from "./components/AppLayout/AppLayout";

import "./styles/index.css";
import {Route, Switch} from 'wouter';
import {createRoot} from "preact/compat/client";

const root = document.querySelector("#root");
root && createRoot(root).render(<App />);

function App() {
    return <AppLayout>
        <Switch>
            <Route path="/" component={LiveMonitor } />
            <Route path="generator" component={Generator } />
            <Route path="network" component={NetworkSettings } />
        </Switch>
    </AppLayout>;
}
