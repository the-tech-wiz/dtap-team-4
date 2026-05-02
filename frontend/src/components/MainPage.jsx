import React from 'react'
import TrackCollection from "./TrackCollection"

const MainPage = () => {
    return (
        <div className="fixed left-0 w-full h-screen bg-gradient-to-t from-[#00635D] to-[#01172F]">
            <div className="p-4">
                <TrackCollection />
            </div>
        </div>
    )
}

export default MainPage