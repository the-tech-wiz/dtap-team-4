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
        <div
            className="h-10 p-2 flex justify-between"
        >
            {isEditing ? (
                <>
                    <input
                        ref={inputRef}
                        className="w-50"
                        value={text}
                        onChange={(e) => setText(e.target.value)}
                        onBlur={save}
                    />

                    <span className="w-50 flex justify-end">
                        <GoPencil />
                    </span>
                </>
            ) : (
                <>
                    <p
                        className="w-50 text-left"
                    >
                        {text}
                    </p>

                    <button
                        className="w-50 flex justify-end"
                        onClick={() => setIsEditing(true)}
                        disabled={isEditing}
                    >
                        <GoPencil />
                    </button>
                </>
            )}
        </div>
    );
}