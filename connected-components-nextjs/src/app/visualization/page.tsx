"use client";

import { useEffect, useState } from "react";
import { useRouter } from "next/navigation";
import AlgorithmVisualizer from "@/components/AlgorithmVisualizer";

export default function VisualizationPage() {
  const router = useRouter();
  const [graphData, setGraphData] = useState<any>(null);
  const [isDark, setIsDark] = useState(true);

  useEffect(() => {
    const theme = localStorage.getItem("theme") || "dark";
    setIsDark(theme === "dark");
    document.documentElement.setAttribute("data-theme", theme);

    const data = sessionStorage.getItem("graphData");
    if (!data) {
      router.push("/");
      return;
    }
    setGraphData(JSON.parse(data));
  }, [router]);

  const toggleTheme = () => {
    const newTheme = isDark ? "light" : "dark";
    setIsDark(!isDark);
    localStorage.setItem("theme", newTheme);
    document.documentElement.setAttribute("data-theme", newTheme);
  };

  if (!graphData) {
    return (
      <div
        className="min-h-screen flex items-center justify-center transition-colors"
        style={{ background: "var(--background)", color: "var(--foreground)" }}
      >
        Loading...
      </div>
    );
  }

  return (
    <main
      className="min-h-screen transition-colors"
      style={{ background: "var(--background)", color: "var(--foreground)" }}
    >
      {/* Top Right Controls */}
      <div className="fixed top-4 right-4 z-50 w-12">
        {/* Theme Toggle Button */}
        <button
          onClick={toggleTheme}
          className="w-full p-2 rounded-xl border-2 transition-all hover:scale-110"
          style={{
            borderColor: "var(--foreground)",
            background: "var(--background)",
            color: "var(--foreground)",
          }}
          aria-label="Toggle theme"
        >
          {isDark ? (
            <svg
              xmlns="http://www.w3.org/2000/svg"
              fill="none"
              viewBox="0 0 24 24"
              strokeWidth={2}
              stroke="currentColor"
              className="w-4 h-4 mx-auto"
            >
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                d="M12 3v2.25m6.364.386l-1.591 1.591M21 12h-2.25m-.386 6.364l-1.591-1.591M12 18.75V21m-4.773-4.227l-1.591 1.591M5.25 12H3m4.227-4.773L5.636 5.636M15.75 12a3.75 3.75 0 11-7.5 0 3.75 3.75 0 017.5 0z"
              />
            </svg>
          ) : (
            <svg
              xmlns="http://www.w3.org/2000/svg"
              fill="none"
              viewBox="0 0 24 24"
              strokeWidth={2}
              stroke="currentColor"
              className="w-4 h-4 mx-auto"
            >
              <path
                strokeLinecap="round"
                strokeLinejoin="round"
                d="M21.752 15.002A9.718 9.718 0 0118 15.75c-5.385 0-9.75-4.365-9.75-9.75 0-1.33.266-2.597.748-3.752A9.753 9.753 0 003 11.25C3 16.635 7.365 21 12.75 21a9.753 9.753 0 009.002-5.998z"
              />
            </svg>
          )}
        </button>
      </div>
      <AlgorithmVisualizer graphData={graphData} isDark={isDark} />
    </main>
  );
}
