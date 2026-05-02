import React from 'react'
import TrackCollection from "./TrackCollection"

export default function MainPage() {
    return (
        <div className="fixed left-0 w-full h-screen custom_bg">
            <div className="p-4">
                <TrackCollection />
            </div>
        </div>
    )
}