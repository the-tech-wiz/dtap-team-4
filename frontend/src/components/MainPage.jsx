import React from 'react'
import TrackCollection from "./TrackCollection"
import MediaControlBar from './MediaControlBar'
import { CgPokemon } from "react-icons/cg";

export default function MainPage() {
    return (
        <div className="fixed w-full h-screen custom_bg">
            <div className="flex p-4 flex-col justify-center items-center ">
                <CgPokemon className="centeredIcon" />
                <TrackCollection />
            </div>
        </div>
    )
}