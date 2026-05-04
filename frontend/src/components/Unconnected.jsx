import React from 'react'
import { TbMoodSadDizzy } from "react-icons/tb";

export default function Unconnected() {
    return (
        <div className="fixed flex justify-center items-center left-0 w-full h-full custom_bg flex-col">
            <TbMoodSadDizzy className="centeredIcon" />
            <div className="flex justify-center items-center bg-white w-60 h-60">
                No devices detected.
                I am big sad now :(
            </div>
        </div>
    )
}