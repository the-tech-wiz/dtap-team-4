import React, { useState } from "react";
import { IoChevronDownOutline } from "react-icons/io5";

const Accordion = ({ title, answer }) => {
    const [accordionOpen, setAccordionOpen] = useState(false);

    return (
        <div className="w-full py-1 text-black rounded-lg bg-[#78b7c2]">
            <div className="flex justify-center w-full">
                <button
                    onClick={() => setAccordionOpen(!accordionOpen)}
                    className="flex justify-between w-29/30"
                >
                    <span>{title}</span>
                    <IoChevronDownOutline
                        className={`text-blue-400 text-xl transition-transform duration-300 ${accordionOpen ? "rotate-180" : "rotate-0"
                            }`}
                    />
                </button>
            </div>
            <div
                className={`grid overflow-hidden transition-all duration-300 ease-in-out text-slate-600 text-sm ${accordionOpen
                    ? "grid-rows-[1fr] opacity-100 py-3"
                    : "grid-rows-[0fr] opacity-0"
                    }`}
            >
                <div className="overflow-hidden">{answer}</div>
            </div>

        </div >
    );
};

export default Accordion;