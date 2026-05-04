import React, { useState } from "react";
import Unconnected from "./components/Unconnected";
import MainPage from "./components/MainPage";

export default function App() {
  const [isConnected, setConnectionStatus] = useState(true);
  return (
    <div className="w-full h-full fixed left-0">
      {isConnected ? <MainPage /> : <Unconnected />}
    </div>
  );
}
