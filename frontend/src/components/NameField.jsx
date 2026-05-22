import React, { useEffect, useRef, useState } from 'react'
import { GoPencil } from "react-icons/go";

export default function NameField() {
    const [text, setText] = useState("Jeff");
    const [isEditing, setIsEditing] = useState(false);

    const inputRef = useRef(null);

    useEffect(() => {
        if (isEditing) {
            inputRef.current?.focus();
        }
    }, [isEditing]);

    function save() {
        setIsEditing(false);
    }

    return (
        <div className="flex h-10 justify-center items-center">
            <div className="flex items-center -translate-x-5">

                <div className="w-8 flex justify-center">
                    {!isEditing && (
                        <button onClick={() => setIsEditing(true)}>
                            <GoPencil />
                        </button>
                    )}
                </div>

                <div className="w-2" />

                {isEditing ? (
                    <input
                        ref={inputRef}
                        className="text-center border"
                        style={{ width: `${Math.max(text.length + 1, 4)}ch` }}
                        value={text}
                        onChange={(e) => setText(e.target.value)}
                        onBlur={save}
                        onKeyDown={(e) => {
                            if (e.key === "Enter") {
                                save();
                            }
                        }}
                    />
                ) : (
                    <p className="text-center">
                        {text}
                    </p>
                )}
            </div>
        </div>
    );
}