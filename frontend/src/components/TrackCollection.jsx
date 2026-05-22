import React, { useState } from "react";
import Accordion from "./Accordion";
import NameField from "./NameField";
import MediaControlBar from "./MediaControlBar"
import { playTrack, stopPlayback, setDeviceVolume } from "../api";

// important note,
// if the user somehow (quite impossible) pressed the cancel button while no track is selected,
// it creates a problem at because resetTrack does not set 'track'

const TrackButton = ({ label, onClick }) => {
    return (
        <button
            onClick={onClick}
            className="panel_secondary_background h-20 w-20 text-white px-4 py-2 rounded flex items-center justify-center"
        >
            <span className="buttonIcons">
                {label}
            </span>
        </button>
    );
};

const TrackCollection = () => {
    const [currentTrack, setCurrentTrack] = useState({
        track: "",
        isVisible: false
    });

    const [deviceName, setDeviceName] = useState("John");
    const [volume, setVolume] = useState(50);


    const names = ["😄", "😆", "😂", "🤣", "🤪", "😜", "🤩", "😹", "😸", "😛"];

    const handlePlay = async (itemIndex) => {
        try {
            await playTrack(itemIndex);
        } catch (err) {
            console.error('Failed to play track', err);
        }
    };

    const handleStop = async () => {
        try {
            await stopPlayback();
        } catch (err) {
            console.error('Failed to stop track', err);
        }
    }

    const handleVolume = async () => {
        try {
            await setDeviceVolume(volume);
        } catch (err) {
            console.error('Failed to set volume', err);
        }
    }

    const handleClick = (itemIndex) => {
        let selectedTrack = names[itemIndex];

        setCurrentTrack({
            track: selectedTrack,
            isVisible: true
        });
        handlePlay(itemIndex);

    };
    const resetTrack = () => {
        setCurrentTrack({
            track: "",
            isVisible: false,
        });
        handleStop();
    };

    return (
        <div className="panel_background">
            <div className="text-black">
                <NameField />
            </div>

            <MediaControlBar currentTrack={currentTrack} resetter={resetTrack} volume={volume} setVolume={setVolume} handleVolume={handleVolume} />

            <div className="flex grid gap-2 p-2">
                {names.map((label, itemIndex) => (
                    <TrackButton
                        key={itemIndex}
                        label={label}
                        onClick={() =>
                            handleClick(itemIndex)
                        }
                    />
                ))}
            </div>
        </div>
    );
};

export default TrackCollection;