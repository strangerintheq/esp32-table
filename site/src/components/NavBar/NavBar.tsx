import * as React from 'preact';
import './Navbar.css';
import {PropsWithChildren} from "preact/compat";
import {NavLink} from 'react-router-dom';

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

