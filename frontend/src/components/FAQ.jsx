import React, { useState } from "react";
import Accordion from "./Accordion";
import BottomBar from "./Bottom_Bar"

// important note,
// if the user somehow (quite impossible) pressed the cancel button while no track is selected,
// it creates a problem at because trackResetter does not set 'track'



const FAQ = () => {
    const [currentTrack, setCurrentTrack] = useState({
        track: "Null",
        isVisible: false
    });

    const [volume, setVolume] = useState({
        value: 0
    });

    const titles = ["Animals", "Laughter"];
    const tracks = [
        ["Dog 1", "Dog 2", "Dog 3", "Cat 1", "Cat 2", "Cat 3", "Cat 4", "Fih :D"],
        ["Deep 1", "Deep 2", "Deep 3", "Deep 4", "Jolly 1", "Jolly 2", "Jolly 3", "Jolly 4"]
    ];

    const clickHandler = (groupIndex, itemIndex) => {
        setCurrentTrack({
            track: tracks[groupIndex][itemIndex],
            isVisible: true
        });
    };
    const trackResetter = () => {
        setCurrentTrack({
            isVisible: false,
        });
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
            <BottomBar currentTrack={currentTrack} resetter={trackResetter} volume={volume} setVolume={setVolume} />
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