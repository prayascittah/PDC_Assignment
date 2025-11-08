"use client";

import { useRouter } from "next/navigation";
import { useState, useEffect } from "react";
import GraphEditor from "@/components/GraphEditor";

export default function Home() {
  const router = useRouter();
  const [isDark, setIsDark] = useState(true);

  useEffect(() => {
    const theme = localStorage.getItem("theme") || "dark";
    setIsDark(theme === "dark");
    document.documentElement.setAttribute("data-theme", theme);
  }, []);

  const toggleTheme = () => {
    const newTheme = isDark ? "light" : "dark";
    setIsDark(!isDark);
    localStorage.setItem("theme", newTheme);
    document.documentElement.setAttribute("data-theme", newTheme);
  };

  const handleStartVisualization = (data: {
    vertices: number;
    edges: number[][];
    isParallel: boolean;
  }) => {
    // Store in sessionStorage
    sessionStorage.setItem("graphData", JSON.stringify(data));
    router.push("/visualization");
  };

  const [isParallel, setIsParallel] = useState(false);

  return (
    <main
      className="min-h-screen transition-colors"
      style={{ background: "var(--background)", color: "var(--foreground)" }}
    >
      {/* Top Right Controls */}
      <div className="fixed top-4 right-4 flex flex-col gap-2 z-50">
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

        {/* Algorithm Selector */}
        <div
          className="w-full px-1.5 py-2 border-2 rounded-xl transition-all"
          style={{
            borderColor: "var(--foreground)",
            background: "var(--background)",
          }}
        >
          <div className="flex flex-col items-center gap-1.5">
            <span
              className={`text-[15px] font-medium transition-colors ${
                !isParallel ? "opacity-100" : "opacity-40"
              }`}
              style={{
                writingMode: "vertical-rl",
                textOrientation: "mixed",
              }}
            >
              Serial
            </span>
            <button
              onClick={() => setIsParallel(!isParallel)}
              className="relative w-6 h-10 rounded-full cursor-pointer transition-all duration-300 ease-in-out hover:scale-105"
              style={{
                background: isParallel
                  ? isDark
                    ? "#4b5563"
                    : "#9ca3af"
                  : isDark
                  ? "#1f2937"
                  : "#e5e7eb",
              }}
            >
              <div
                className={`absolute left-0.5 w-5 h-5 rounded-full transition-all duration-300 ease-in-out shadow-md ${
                  isParallel ? "bottom-0.5" : "top-0.5"
                }`}
                style={{
                  background: "var(--foreground)",
                }}
              />
            </button>
            <span
              className={`text-[15px] font-medium transition-colors ${
                isParallel ? "opacity-100" : "opacity-40"
              }`}
              style={{
                writingMode: "vertical-rl",
                textOrientation: "mixed",
              }}
            >
              Parallel
            </span>
          </div>
        </div>
      </div>

      <div className="max-w-4xl mx-auto px-5 py-10">
        <header
          className="text-center mb-10 pb-8 border-b-2"
          style={{ borderColor: isDark ? "#1f2937" : "#e5e7eb" }}
        >
          <h1 className="text-4xl font-bold mb-3 tracking-tight">
            Algorithm B Visualizer
          </h1>
        </header>

        <GraphEditor
          onStartVisualization={handleStartVisualization}
          isDark={isDark}
          isParallel={isParallel}
        />
      </div>
    </main>
  );
}
