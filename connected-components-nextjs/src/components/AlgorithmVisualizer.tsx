"use client";

import { useState, useEffect } from "react";
import { useRouter } from "next/navigation";
import GraphVisualization from "./GraphVisualization";
import ParentTable from "./ParentTable";

interface Step {
  iteration: number;
  phase: string;
  parent: number[];
  edges: number[][];
  processingElements: string[];
}

interface GraphData {
  vertices: number;
  edges: number[][];
  isParallel: boolean;
}

export default function AlgorithmVisualizer({
  graphData,
  isDark,
}: {
  graphData: GraphData;
  isDark: boolean;
}) {
  const router = useRouter();
  const [currentStepIndex, setCurrentStepIndex] = useState(0);
  const [steps, setSteps] = useState<Step[]>([]);
  const [isPlaying, setIsPlaying] = useState(false);

  useEffect(() => {
    // Generate animation steps
    const generatedSteps = generateAnimationSteps(graphData);
    setSteps(generatedSteps);
  }, [graphData]);

  useEffect(() => {
    if (isPlaying && currentStepIndex < steps.length - 1) {
      // Use 1500ms (1.5 seconds) for better visibility of edge processing
      const timer = setTimeout(() => {
        setCurrentStepIndex(currentStepIndex + 1);
      }, 1500);
      return () => clearTimeout(timer);
    } else if (currentStepIndex >= steps.length - 1) {
      setIsPlaying(false);
    }
  }, [isPlaying, currentStepIndex, steps.length]);

  const currentStep = steps[currentStepIndex] || {
    iteration: 0,
    phase: "Initialization",
    parent: Array.from({ length: graphData.vertices }, (_, i) => i),
    edges: graphData.edges,
    processingElements: [],
  };

  const borderColor = isDark ? "#1f2937" : "#e5e7eb";
  const bgColor = isDark ? "#030712" : "#f9fafb";
  const cardBg = isDark ? "#000000" : "#ffffff";

  // Helper: get dropped edges for current step (only in Alter phase)
  const prevStep = steps[currentStepIndex - 1] || currentStep;
  const droppedEdges =
    currentStep.phase === "Alter"
      ? prevStep.edges.filter(
          (e) =>
            !currentStep.edges.some((ce) => ce[0] === e[0] && ce[1] === e[1])
        )
      : [];

  return (
    <div className="grid grid-cols-[400px_1fr_300px] grid-rows-[auto_1fr] gap-5 p-5 h-screen">
      {/* Header */}
      <div
        className="col-span-3 text-center pb-5 border-b-2"
        style={{ borderColor }}
      >
        <h2 className="text-3xl font-bold mb-2">
          {graphData.isParallel ? "Parallel" : "Serial"} Connected Components
        </h2>
      </div>

      {/* Left Panel - Parent Vector and Edges */}
      <div className="flex flex-col gap-5">
        {/* Parent Vector Container */}
        <div
          className="border rounded-lg p-5"
          style={{ borderColor, background: cardBg }}
        >
          <h3 className="font-semibold mb-3 text-lg">Parent Vector</h3>
          <div
            className="grid gap-2"
            style={{
              gridTemplateColumns: `repeat(${Math.min(
                currentStep.parent.length,
                5
              )}, minmax(36px, 1fr))`,
            }}
          >
            {currentStep.parent.map((p, i) => (
              <div
                key={i}
                className="flex flex-col items-center gap-1 px-2 py-2 rounded-lg shadow-sm"
                style={{
                  background: isDark ? "#1f2937" : "#f3f4f6",
                  minWidth: 36,
                  maxWidth: 60,
                  transition: "background 0.2s",
                }}
              >
                <div className="text-xs opacity-60 font-semibold">v{i + 1}</div>
                <div className="text-lg font-extrabold tracking-tight">
                  {p + 1}
                </div>
              </div>
            ))}
          </div>
        </div>

        {/* Edges Container */}
        <div
          className="border rounded-lg p-5 flex-1"
          style={{ borderColor, background: cardBg }}
        >
          <h3 className="font-semibold mb-3 text-lg">Edges</h3>
          <div className="flex flex-wrap gap-2">
            {graphData.edges.map(([u, v], idx) => {
              const isCurrentlyActive = currentStep.edges.some(
                ([cu, cv]) => cu === u && cv === v
              );
              const isDropped = !isCurrentlyActive;
              return (
                <span
                  key={idx}
                  className={`px-3 py-2 rounded-lg text-sm font-mono border ${
                    isDropped ? "line-through opacity-60" : ""
                  }`}
                  style={{
                    background: isDark ? "#1f2937" : "#f3f4f6",
                    borderColor: isDark ? "#374151" : "#d1d5db",
                    color: isDark ? "#f9fafb" : "#374151",
                    textDecoration: isDropped ? "line-through" : "none",
                  }}
                >
                  ({u + 1}, {v + 1})
                </span>
              );
            })}
          </div>
        </div>
      </div>

      {/* Center - Graph Visualization */}
      <div
        className="border rounded-lg p-5 overflow-hidden relative"
        style={{ borderColor, background: bgColor }}
      >
        <GraphVisualization
          vertices={graphData.vertices}
          edges={currentStep.edges}
          parent={currentStep.parent}
          processingElements={currentStep.processingElements}
          isDark={isDark}
          phase={currentStep.phase}
        />
      </div>

      {/* Right Panel - Control Panel */}
      <div className="flex flex-col gap-5">
        {/* Iteration Info */}
        <div
          className="text-center p-4 border rounded"
          style={{ background: cardBg, borderColor }}
        >
          <div className="text-2xl font-bold mb-1">{currentStep.iteration}</div>
          <div className="text-sm opacity-60">{currentStep.phase}</div>
        </div>

        {/* Parent Table */}
        <div className="flex-1 overflow-hidden flex flex-col">
          <h3 className="font-semibold mb-3">Parent Table</h3>
          <div className="flex-1 overflow-auto">
            <ParentTable parent={currentStep.parent} isDark={isDark} />
          </div>
        </div>

        {/* Control Buttons */}
        <div className="flex flex-col gap-2">
          <button
            onClick={() => setIsPlaying(!isPlaying)}
            className="px-4 py-2 border rounded transition-colors text-sm"
            style={{ borderColor: isDark ? "#4b5563" : "#9ca3af" }}
            onMouseEnter={(e) =>
              (e.currentTarget.style.background = isDark
                ? "#1f2937"
                : "#e5e7eb")
            }
            onMouseLeave={(e) =>
              (e.currentTarget.style.background = "transparent")
            }
          >
            {isPlaying ? "⏸ Pause" : "▶ Play"}
          </button>
          <button
            onClick={() => {
              if (currentStepIndex < steps.length - 1) {
                setCurrentStepIndex(currentStepIndex + 1);
              }
            }}
            disabled={currentStepIndex >= steps.length - 1}
            className="px-4 py-2 border rounded transition-colors text-sm disabled:opacity-40"
            style={{ borderColor: isDark ? "#4b5563" : "#9ca3af" }}
            onMouseEnter={(e) =>
              !e.currentTarget.disabled &&
              (e.currentTarget.style.background = isDark
                ? "#1f2937"
                : "#e5e7eb")
            }
            onMouseLeave={(e) =>
              (e.currentTarget.style.background = "transparent")
            }
          >
            Step Forward
          </button>
          <button
            onClick={() => {
              setCurrentStepIndex(0);
              setIsPlaying(false);
            }}
            className="px-4 py-2 border rounded transition-colors text-sm"
            style={{ borderColor: isDark ? "#4b5563" : "#9ca3af" }}
            onMouseEnter={(e) =>
              (e.currentTarget.style.background = isDark
                ? "#1f2937"
                : "#e5e7eb")
            }
            onMouseLeave={(e) =>
              (e.currentTarget.style.background = "transparent")
            }
          >
            ↻ Reset
          </button>
          <button
            onClick={() => router.push("/")}
            className="px-4 py-2 border rounded transition-colors text-sm"
            style={{ borderColor: isDark ? "#4b5563" : "#9ca3af" }}
            onMouseEnter={(e) =>
              (e.currentTarget.style.background = isDark
                ? "#1f2937"
                : "#e5e7eb")
            }
            onMouseLeave={(e) =>
              (e.currentTarget.style.background = "transparent")
            }
          >
            ← Back to Editor
          </button>
        </div>
      </div>
    </div>
  );
}

function generateAnimationSteps(graphData: GraphData): Step[] {
  let tempParent = Array.from({ length: graphData.vertices }, (_, i) => i);
  let tempEdges = [...graphData.edges];
  let iteration = 0;

  const steps: Step[] = [
    {
      iteration: 0,
      phase: "Initialization",
      parent: [...tempParent],
      edges: [...tempEdges],
      processingElements: [],
    },
  ];

  let changed = true;
  while (changed && iteration < 10) {
    iteration++;
    changed = false;

    // Direct-Connect - Process each edge individually with a step
    let connectChanged = false;

    for (const [u, v] of tempEdges) {
      // Add step showing this edge being processed
      steps.push({
        iteration,
        phase: "Direct-Connect",
        parent: [...tempParent],
        edges: [...tempEdges],
        processingElements: [`edge-${u}-${v}`],
      });

      if (u > v) {
        const minVal = Math.min(tempParent[u], v);
        if (tempParent[u] !== minVal) {
          tempParent[u] = minVal;
          connectChanged = true;
        }
      } else {
        const minVal = Math.min(tempParent[v], u);
        if (tempParent[v] !== minVal) {
          tempParent[v] = minVal;
          connectChanged = true;
        }
      }

      // Add step showing the result after processing this edge
      steps.push({
        iteration,
        phase: "Direct-Connect",
        parent: [...tempParent],
        edges: [...tempEdges],
        processingElements: [],
      });
    }

    changed = changed || connectChanged;

    // Shortcut - Animate each vertex update as a separate step
    let shortcutChanged = false;
    const oldParent = [...tempParent];
    for (let i = 0; i < tempParent.length; i++) {
      const newParent = oldParent[oldParent[i]];
      // Step: highlight vertex before update
      steps.push({
        iteration,
        phase: "Shortcut",
        parent: [...tempParent],
        edges: [...tempEdges],
        processingElements: [`vertex-${i}`],
      });
      if (tempParent[i] !== newParent) {
        tempParent[i] = newParent;
        shortcutChanged = true;
      }
      // Step: show result after update
      steps.push({
        iteration,
        phase: "Shortcut",
        parent: [...tempParent],
        edges: [...tempEdges],
        processingElements: [],
      });
    }
    changed = changed || shortcutChanged;

    // Alter
    const newEdges: number[][] = [];
    for (const [u, v] of tempEdges) {
      const pu = tempParent[u];
      const pv = tempParent[v];
      if (pu !== pv) {
        newEdges.push([pu, pv].sort((a, b) => a - b));
      }
    }
    tempEdges = newEdges;

    steps.push({
      iteration,
      phase: "Alter",
      parent: [...tempParent],
      edges: [...tempEdges],
      processingElements: [],
    });
  }

  return steps;
}
