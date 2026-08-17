import React from 'react';
import "./Input.css"

export function Input({value, onChange, label}: {value, onChange, label}) {
    return <div className="floating-group">
        <input
            type="text"
            value={value}
            placeholder=" "
            onChange={onChange}
        />
        <label>
            {label}
        </label>
    </div>
}