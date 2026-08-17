import React from 'react';
import {Navbar} from "../NavBar/NavBar";

import "./AppLayout.css"

export function AppLayout({children}) {
    return <div className="app-layout">
        <Navbar/>
        {children}
    </div>;
}
