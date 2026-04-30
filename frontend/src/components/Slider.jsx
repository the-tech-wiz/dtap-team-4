import React from "react"

export default function Slider({ value, setVolume, handleVolume }) {

    return (
        <div className="bg-gray-800 justify-center">
            <input
                type="range"
                min="0"
                max="100"
                value={value}
                onChange={(e) => setVolume(e.target.value)}
                onMouseUp={handleVolume}
                className="bg-gray-300 appearance-none cursor-pointer"
            />
        </div>
    )
}
