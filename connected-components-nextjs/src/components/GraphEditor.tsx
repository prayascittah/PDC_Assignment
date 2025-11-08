"use client";

import { useState } from "react";

interface GraphEditorProps {
  onStartVisualization: (data: {
    vertices: number;
    edges: number[][];
    isParallel: boolean;
  }) => void;
  isDark: boolean;
  isParallel: boolean;
}

export default function GraphEditor({
  onStartVisualization,
  isDark,
  isParallel,
}: GraphEditorProps) {
  const [vertices, setVertices] = useState(6);
  const [verticesInput, setVerticesInput] = useState("6");
  const [edges, setEdges] = useState<number[][]>([]);
  const [selectedVertex, setSelectedVertex] = useState<number | null>(null);

  const handleVertexClick = (vertexId: number) => {
    if (selectedVertex === null) {
      setSelectedVertex(vertexId);
    } else if (selectedVertex === vertexId) {
      setSelectedVertex(null);
    } else {
      const edge = [selectedVertex, vertexId].sort((a, b) => a - b);
      const edgeExists = edges.some(
        (e) => e[0] === edge[0] && e[1] === edge[1]
      );

      if (!edgeExists) {
        setEdges([...edges, edge]);
      }

      setSelectedVertex(null);
    }
  };

  const clearGraph = () => {
    setEdges([]);
    setSelectedVertex(null);
  };

  const handleStart = () => {
    if (edges.length === 0) {
      alert("Please add at least one edge to the graph!");
      return;
    }
    onStartVisualization({ vertices, edges, isParallel });
  };

  const handleVerticesChange = (value: string) => {
    setVerticesInput(value);
    const num = parseInt(value);
    if (!isNaN(num) && num >= 2 && num <= 10) {
      setVertices(num);
      setEdges([]);
      setSelectedVertex(null);
    }
  };

  const borderColor = isDark ? "#1f2937" : "#e5e7eb";
  const bgColor = isDark ? "#030712" : "#f9fafb";
  const inputBg = isDark ? "#000000" : "#ffffff";
  const inputBorder = isDark ? "#4b5563" : "#d1d5db";

  return (
    <div className="space-y-8">
      {/* Vertices Input */}
      <div
        className="p-5 border rounded-lg"
        style={{ borderColor, background: bgColor }}
      >
        <div className="flex items-center gap-4">
          <label className="font-medium min-w-[180px]">
            Number of Vertices:
          </label>
          <input
            type="text"
            value={verticesInput}
            onChange={(e) => handleVerticesChange(e.target.value)}
            onBlur={() => {
              const num = parseInt(verticesInput);
              if (isNaN(num) || num < 2) {
                setVerticesInput("2");
                setVertices(2);
              } else if (num > 10) {
                setVerticesInput("10");
                setVertices(10);
              }
            }}
            placeholder="Enter 2-10"
            className="px-4 py-2 rounded focus:outline-none transition-all w-32 text-center"
            style={{
              background: inputBg,
              border: `2px solid ${inputBorder}`,
              color: "var(--foreground)",
            }}
          />
        </div>
      </div>

      {/* Graph Editor */}
      <div
        className="p-5 border rounded-lg"
        style={{ borderColor, background: bgColor }}
      >
        <h3 className="text-xl font-semibold mb-2">Graph Editor</h3>
        <p className="text-sm mb-5 opacity-60">
          Click on vertices to connect them with edges
        </p>

        <div
          className="relative border rounded-lg flex items-center justify-center"
          style={{
            background: inputBg,
            borderColor: inputBorder,
            height: "450px",
            width: "100%",
          }}
        >
          {/* Circular arrangement container */}
          <div className="relative" style={{ width: "400px", height: "400px" }}>
            {/* Draw edges first (underneath vertices) */}
            <svg
              className="absolute inset-0 w-full h-full pointer-events-none"
              viewBox="0 0 400 400"
              style={{ zIndex: 1 }}
            >
              {edges.map(([u, v], idx) => {
                const centerX = 200;
                const centerY = 200;
                const radius = 150;

                const angle1 = (2 * Math.PI * u) / vertices - Math.PI / 2;
                const angle2 = (2 * Math.PI * v) / vertices - Math.PI / 2;

                const x1 = centerX + radius * Math.cos(angle1);
                const y1 = centerY + radius * Math.sin(angle1);
                const x2 = centerX + radius * Math.cos(angle2);
                const y2 = centerY + radius * Math.sin(angle2);

                return (
                  <line
                    key={idx}
                    x1={x1}
                    y1={y1}
                    x2={x2}
                    y2={y2}
                    stroke={isDark ? "#4b5563" : "#9ca3af"}
                    strokeWidth="2"
                  />
                );
              })}
            </svg>

            {/* Vertices positioned absolutely */}
            {Array.from({ length: vertices }, (_, i) => {
              const isSelected = selectedVertex === i;
              const isConnected = edges.some((e) => e.includes(i));

              // Calculate position in circle
              const angle = (2 * Math.PI * i) / vertices - Math.PI / 2; // Start from top
              const radius = 150; // Distance from center
              const centerX = 200;
              const centerY = 200;

              const x = centerX + radius * Math.cos(angle) - 20; // -20 to center the button (40px / 2)
              const y = centerY + radius * Math.sin(angle) - 20;

              return (
                <button
                  key={i}
                  onClick={() => handleVertexClick(i)}
                  className="absolute w-10 h-10 rounded-full border-2 font-semibold transition-all flex items-center justify-center hover:scale-125"
                  style={{
                    left: `${x}px`,
                    top: `${y}px`,
                    background: isSelected
                      ? "var(--foreground)"
                      : "var(--background)",
                    color: isSelected
                      ? "var(--background)"
                      : "var(--foreground)",
                    borderColor:
                      isSelected || !isConnected
                        ? "var(--foreground)"
                        : isDark
                        ? "#6b7280"
                        : "#9ca3af",
                    zIndex: 2,
                  }}
                >
                  {i + 1}
                </button>
              );
            })}
          </div>
        </div>

        <div className="mt-5">
          <h4 className="font-medium mb-2">Current Edges:</h4>
          <div
            className="p-4 border rounded min-h-[50px] text-sm"
            style={{ background: inputBg, borderColor: inputBorder }}
          >
            {edges.length === 0 ? (
              <span className="opacity-60">No edges added</span>
            ) : (
              <div className="flex flex-wrap gap-2">
                {edges.map((edge, i) => (
                  <span
                    key={i}
                    className="inline-block px-2 py-1 rounded text-xs"
                    style={{ background: isDark ? "#1f2937" : "#e5e7eb" }}
                  >
                    ({edge[0] + 1}, {edge[1] + 1})
                  </span>
                ))}
              </div>
            )}
          </div>
        </div>
      </div>

      {/* Control Buttons */}
      <div className="flex gap-4 justify-center">
        <button
          onClick={clearGraph}
          className="px-6 py-3 border-2 rounded font-medium transition-all"
          style={{
            borderColor: isDark ? "#4b5563" : "#9ca3af",
            color: isDark ? "#9ca3af" : "#6b7280",
          }}
          onMouseEnter={(e) => {
            e.currentTarget.style.background = isDark ? "#4b5563" : "#9ca3af";
            e.currentTarget.style.color = isDark ? "#ffffff" : "#000000";
          }}
          onMouseLeave={(e) => {
            e.currentTarget.style.background = "transparent";
            e.currentTarget.style.color = isDark ? "#9ca3af" : "#6b7280";
          }}
        >
          Clear Graph
        </button>
        <button
          onClick={handleStart}
          className="px-6 py-3 border-2 rounded font-medium transition-all"
          style={{
            borderColor: "var(--foreground)",
            color: "var(--foreground)",
          }}
          onMouseEnter={(e) => {
            e.currentTarget.style.background = "var(--foreground)";
            e.currentTarget.style.color = "var(--background)";
          }}
          onMouseLeave={(e) => {
            e.currentTarget.style.background = "transparent";
            e.currentTarget.style.color = "var(--foreground)";
          }}
        >
          Start Visualization
        </button>
      </div>
    </div>
  );
}
