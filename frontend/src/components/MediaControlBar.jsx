import React from "react";
import { FaPause } from "react-icons/fa";
import VolumeSlider from "./VolumeSlider";



// height goes from 0 to 12
export default function MediaControlBar({ currentTrack, resetter, volume, setVolume, handleVolume }) {
    return (
        <div className="w-full flex justify-center transition-all duration-300 h-20 -translate-y-1 pointer-events-auto">
            <div className="w-full py-1 px-5 text-black rounded-lg ui_secondary_bg">
                <div className="flex justify-between">
                    <div className="p-1">
                        <span>Now Playing: </span>
                        <span>{currentTrack.track}</span>
                    </div>
                    <button onClick={resetter} className="p-1">
                        <FaPause />
                    </button>
                </div>
                <VolumeSlider value={volume} setVolume={setVolume} handleVolume={handleVolume} />
            </div>
        </div >
    );
};