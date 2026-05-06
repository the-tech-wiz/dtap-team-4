import React, { useState } from 'react'

export default function NameField() {
    const [text, setText] = useState("Jeff");
    const [isEditing, setIsEditing] = useState(false);

    return (
        <div
            className="h-10 p-2 flex justify-between"
        >
            {isEditing ? (
                <>
                    <input
                        className="w-50"
                        value={text}
                        onChange={(e) => setText(e.target.value)}
                    />

                    <button
                        className="w-50 text-right"
                        onClick={() => setIsEditing(false)}
                    >
                        Save
                    </button>
                </>
            ) : (
                <>
                    <p
                        className="w-50 text-left"
                    >
                        {text}
                    </p>

                    <button
                        className="w-50 text-right"
                        onClick={() => setIsEditing(true)}
                    >
                        Edit
                    </button>
                </>
            )}
        </div>
    );
}