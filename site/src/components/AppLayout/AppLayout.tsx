import React from 'react';
import {Navbar} from "../navigation/NavBar";
import {Outlet} from 'react-router-dom';

import "./AppLayout.css"

export function AppLayout() {
    return <div className="app-layout">
        <Navbar/>
        <Outlet/>
    </div>;
}
