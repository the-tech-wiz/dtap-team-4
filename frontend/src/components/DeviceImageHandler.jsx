import { useState } from "react";
import { CgPokemon } from "react-icons/cg";
import { FaCamera } from "react-icons/fa";

export default function DeviceImageHandler() {
    const [previewUrl, setPreviewUrl] = useState(null);

    function handleImageChange(e) {
        const file = e.target.files[0];

        if (!file) return;

        setPreviewUrl(URL.createObjectURL(file));
    }

    return (
        <div className="relative w-fit">
            {previewUrl ? (
                <img
                    src={previewUrl}
                    alt="Profile preview"
                    className="h-32 w-32 rounded-full object-cover border"
                />
            ) : (
                <div className="h-32 w-32 rounded-full border flex items-center justify-center">
                    <CgPokemon className="text-6xl" />
                </div>
            )}

            <label className="absolute top-0 right-0 cursor-pointer rounded-full bg-white border px-2 py-1 text-sm shadow">
                <FaCamera />

                <input
                    type="file"
                    accept="image/*"
                    capture="user"
                    onChange={handleImageChange}
                    className="hidden"
                />
            </label>
        </div>
    );
}