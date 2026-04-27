import React, { useState } from 'react'

export default function Slider({ value, onChange }) {
    return (
        <div className="bg-gray-800 justify-center">
            <input
                type="range"
                min="0"
                max="100"
                value={value}
                onChange={(e) => onChange(Number(e.target.value))}
                className="bg-gray-300 appearance-none cursor-pointer"
            />
        </div>
    )
}
