import React, { useState } from "react";
import Unconnected from "./components/Unconnected";
import MainPage from "./components/MainPage";

export default function App() {
  const [isConnected, setConnectionStatus] = useState(false);
  return (
    <div>
      {isConnected ? <MainPage /> : <Unconnected />}
    </div>
  );
}
