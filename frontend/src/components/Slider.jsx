import React, { useState } from 'react'

export default function Slider() {
    const [value, setValue] = useState(50);
    return (
        <div className="">
            <input
                type="range"
                min="0"
                max="100"
                value={value}
                class="slider"
                id="volume"
                onChange={(e) => setValue(e.target.value)}
                className="rounded-full"
            />
        </div>
    )
}
