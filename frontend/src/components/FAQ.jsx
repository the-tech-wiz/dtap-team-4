import React, { useState } from "react";
import Accordion from "./Accordion";
import BottomBar from "./Bottom_Bar"
import { playTrack, stopPlayback, setDeviceVolume } from "../api";

// important note,
// if the user somehow (quite impossible) pressed the cancel button while no track is selected,
// it creates a problem at because trackResetter does not set 'track'



const FAQ = () => {
    const [currentTrack, setCurrentTrack] = useState({
        track: "Null",
        isVisible: false
    });

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

    const clickHandler = (groupIndex, itemIndex) => {
        let selectedTrack = tracks[groupIndex][itemIndex];

        setCurrentTrack({
            track: selectedTrack,
            isVisible: true
        });
        handlePlay(selectedTrack);

    };
    const trackResetter = () => {
        setCurrentTrack({
            isVisible: false,
        });
        handleStop();
    };

    return (
        <div className="p-4 bg-[#9AC2C9] rounded-lg space-y-4">
            {titles.map((title, groupIndex) => (
                <Accordion
                    key={groupIndex}
                    title={title}
                    answer={
                        <div className="flex flex-col gap-2 p-2">
                            {tracks[groupIndex].map((label, itemIndex) => (
                                <CustomButton
                                    key={itemIndex}
                                    label={label}
                                    onClick={() =>
                                        clickHandler(groupIndex, itemIndex)
                                    }
                                />
                            ))}
                        </div>
                    }
                />
            ))}
            <div>{volume}</div>
            <BottomBar currentTrack={currentTrack} resetter={trackResetter} volume={volume} setVolume={setVolume} handleVolume={handleVolume} />
        </div>
    );
};

const CustomButton = ({ label, onClick }) => {
    return (
        <button
            onClick={onClick}
            className="bg-blue-500 text-white px-4 py-2 rounded"
        >
            {label}
        </button>
    );
};

export default FAQ;