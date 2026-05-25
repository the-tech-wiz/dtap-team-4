import React, { useState, useEffect } from "react";
import Unconnected from "./components/Unconnected";
import MainPage from "./components/MainPage";
import { getDeviceState } from "./api";

const POLL_INTERVAL_MS = 3000;

export default function App() {
  const [isConnected, setConnectionStatus] = useState(null); // null = still loading

  useEffect(() => {
    async function checkConnection() {
      try {
        const data = await getDeviceState();
        setConnectionStatus(data?.state?.status?.online === true);
      } catch {
        setConnectionStatus(false);
      }
    }

    checkConnection();
    const interval = setInterval(checkConnection, POLL_INTERVAL_MS);
    return () => clearInterval(interval);
  }, []);

  if (isConnected === null) {
    return null; // first fetch in flight — render nothing to avoid a flash
  }

  return (
    <div className="flex w-full h-full left-0">
      {isConnected ? <MainPage /> : <Unconnected />}
    </div>
  );
}
