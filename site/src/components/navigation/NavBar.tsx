import React, {PropsWithChildren} from 'react';
import {NavLink} from "react-router-dom";
import './Navbar.css';

export function Navbar() {
    return <nav className="main-navbar">
        <Navigation to="/">🖥️ Monitor</Navigation>
        <Navigation to="/generator">🎲 Generator</Navigation>
        <Navigation to="/network">⚙ Settings</Navigation>
    </nav>;
}

function Navigation({children, to}: PropsWithChildren<{ to }>) {
    const className = x => x.isActive ? "nav-item active" : "nav-item";
    return <NavLink to={to} className={className}>{children}</NavLink>
}

