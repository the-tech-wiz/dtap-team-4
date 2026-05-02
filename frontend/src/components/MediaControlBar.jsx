import React from "react";
import { FaPause } from "react-icons/fa";
import VolumeSlider from "./VolumeSlider";



// height goes from 0 to 12
export default function MediaControlBar({ currentTrack, resetter, volume, setVolume, handleVolume }) {
    return (
        <div className={`
            fixed bottom-0 left-0 w-full
            flex justify-center transition-all duration-300 h-12 
            ${(!currentTrack.isVisible)
                ? "opacity-0 translate-y-1 pointer-events-none"
                : "opacity-100 -translate-y-1 pointer-events-auto"
            }`} >
            <div className="bg-gray-900 text-white px-4 py-3 w-100 overflow-hidden">
                <div className="flex justify-between">
                    <span>Now Playing:</span>
                    <span>{currentTrack.track}</span>
                    <button onClick={resetter}>
                        <FaPause />
                    </button>
                    <VolumeSlider value={volume} setVolume={setVolume} handleVolume={handleVolume} />
                </div>

            </div>
        </div >
    );
};