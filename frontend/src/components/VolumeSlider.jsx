import React from "react"

export default function VolumeSlider({ value, setVolume, handleVolume }) {
    return (
        <div>
            <input
                type="range"
                min="0"
                max="100"
                value={value}
                onChange={(e) => setVolume(e.target.value)}
                onMouseUp={handleVolume}
                className="bg-white appearance-none cursor-pointer"
            />
        </div>
    )
}
