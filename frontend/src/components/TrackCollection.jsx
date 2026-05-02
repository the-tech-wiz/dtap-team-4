import React, { useState } from "react";
import Accordion from "./Accordion";
import MediaControlBar from "./MediaControlBar"
import { playTrack, stopPlayback, setDeviceVolume } from "../api";

// important note,
// if the user somehow (quite impossible) pressed the cancel button while no track is selected,
// it creates a problem at because resetTrack does not set 'track'

const TrackButton = ({ label, onClick }) => {
    return (
        <button
            onClick={onClick}
            className="bg-blue-500 text-white px-4 py-2 rounded"
        >
            {label}
        </button>
    );
};

const TrackCollection = () => {
    const [currentTrack, setCurrentTrack] = useState({
        track: "Null",
        isVisible: false
    });

    const [deviceName, setDeviceName] = useState("Jeff");
    const [volume, setVolume] = useState(50);

    const titles = ["Animals", "Laughter"];
    const tracks = [
        ["Dog 1", "Dog 2", "Dog 3", "Cat 1", "Cat 2", "Cat 3", "Cat 4", "Fih :D"],
        ["Deep 1", "Deep 2", "Deep 3", "Deep 4", "Jolly 1", "Jolly 2", "Jolly 3", "Jolly 4"]
    ];

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

    const handleClick = (groupIndex, itemIndex) => {
        let selectedTrack = tracks[groupIndex][itemIndex];

        setCurrentTrack({
            track: selectedTrack,
            isVisible: true
        });
        handlePlay(selectedTrack);

    };
    const resetTrack = () => {
        setCurrentTrack({
            isVisible: false,
        });
        handleStop();
    };

    return (
        <div className="ui_bg">
            <div className="text-black">
                {deviceName}
            </div>
            <MediaControlBar currentTrack={currentTrack} resetter={resetTrack} volume={volume} setVolume={setVolume} handleVolume={handleVolume} />
            {titles.map((title, groupIndex) => (
                <Accordion
                    key={groupIndex}
                    title={title}
                    answer={
                        <div className="flex flex-col gap-2 p-2">
                            {tracks[groupIndex].map((label, itemIndex) => (
                                <TrackButton
                                    key={itemIndex}
                                    label={label}
                                    onClick={() =>
                                        handleClick(groupIndex, itemIndex)
                                    }
                                />
                            ))}
                        </div>
                    }
                />
            ))}
        </div>
    );
};

export default TrackCollection;