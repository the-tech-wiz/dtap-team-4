import React from "react";
import { FaPause } from "react-icons/fa";
import VolumeSlider from "./VolumeSlider";



// height goes from 0 to 12
export default function MediaControlBar({ currentTrack, resetter, volume, setVolume, handleVolume }) {
    return (
        <div className={`transition w-full transition-all duration-300 pointer-events-auto ${currentTrack.track == "" ? "opacity-0 h-0 pointer-events-none" : "opacity-100 h-20 pointer-events-auto"}`}>
            <div className="flex flex-col justify-center w-full gap-2 py-2 px-5 text-black rounded-lg panel_secondary_background">
                <div className="flex flex-row justify-between w-full gap-1 p-1">
                    <div>Playing: </div>

                    <div className="absolute left-1/2 -translate-x-1/2">
                        {currentTrack.track}
                    </div>
                    <div>
                        <button onClick={resetter}>
                            <FaPause />
                        </button>
                    </div>
                </div>
                <VolumeSlider value={volume} setVolume={setVolume} handleVolume={handleVolume} />
            </div>
        </div >
    );
};