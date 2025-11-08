"use client";

import { useEffect, useRef } from "react";

interface GraphVisualizationProps {
  vertices: number;
  edges: number[][];
  parent: number[];
  processingElements: string[];
  isDark: boolean;
  phase: string;
}

export default function GraphVisualization({
  vertices,
  edges,
  parent,
  processingElements,
  isDark,
  phase,
}: GraphVisualizationProps) {
  const svgRef = useRef<SVGSVGElement>(null);

  useEffect(() => {
    if (!svgRef.current) return;

    const width = 800;
    const height = 600;
    const centerX = width / 2;
    const centerY = height / 2;
    const radius = Math.min(width, height) * 0.35; // 35% of the smaller dimension

    // Calculate positions in a circle
    const positions = new Map<number, { x: number; y: number }>();

    for (let i = 0; i < vertices; i++) {
      const angle = (2 * Math.PI * i) / vertices - Math.PI / 2; // Start from top
      const x = centerX + radius * Math.cos(angle);
      const y = centerY + radius * Math.sin(angle);
      positions.set(i, { x, y });
    }

    // Clear SVG
    svgRef.current.innerHTML = "";

    // Draw edges
    edges.forEach(([u, v]) => {
      const pos1 = positions.get(u)!;
      const pos2 = positions.get(v)!;

      const line = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "line"
      );
      line.setAttribute("x1", String(pos1.x));
      line.setAttribute("y1", String(pos1.y));
      line.setAttribute("x2", String(pos2.x));
      line.setAttribute("y2", String(pos2.y));

      const edgeId = `edge-${u}-${v}`;
      const isProcessing = processingElements.includes(edgeId);
      const isDirectConnect = phase === "Direct-Connect";

      // Enhanced highlighting for edges in Direct-Connect phase
      if (isProcessing && isDirectConnect) {
        line.setAttribute("stroke", "#22c55e"); // Bright green for active edge
        line.setAttribute("stroke-width", "5");
        line.classList.add("animate-pulse");
        // Add glow effect
        line.setAttribute("filter", "drop-shadow(0 0 8px #22c55e)");
      } else if (isProcessing) {
        line.setAttribute("stroke", isDark ? "#fff" : "#000");
        line.setAttribute("stroke-width", "3");
        line.classList.add("animate-pulse");
      } else {
        line.setAttribute("stroke", isDark ? "#666" : "#9ca3af");
        line.setAttribute("stroke-width", "2");
      }

      line.setAttribute("id", edgeId);

      svgRef.current!.appendChild(line);
    });

    // Component colors
    const colors = [
      "#ff6b6b",
      "#4ecdc4",
      "#45b7d1",
      "#96ceb4",
      "#ffeaa7",
      "#dda0dd",
      "#98fb98",
      "#f4a460",
      "#87ceeb",
      "#deb887",
    ];

    // Draw vertices
    for (let i = 0; i < vertices; i++) {
      const pos = positions.get(i)!;
      const component = parent[i];

      const g = document.createElementNS("http://www.w3.org/2000/svg", "g");
      g.setAttribute("id", `vertex-${i}`);

      const circle = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "circle"
      );
      circle.setAttribute("cx", String(pos.x));
      circle.setAttribute("cy", String(pos.y));
      circle.setAttribute("r", "20");
      circle.setAttribute("fill", isDark ? "#000" : "#fff");

      const vertexId = `vertex-${i}`;
      const isProcessing = processingElements.includes(vertexId);
      const isShortcut = phase === "Shortcut";

      // Enhanced highlighting for vertices in Shortcut phase
      if (isProcessing && isShortcut) {
        circle.setAttribute("stroke", "#fbbf24"); // Bright yellow/orange for active vertex
        circle.setAttribute("stroke-width", "4");
        circle.classList.add("animate-pulse");
        // Add glow effect for shortcut phase
        circle.setAttribute("filter", "drop-shadow(0 0 12px #fbbf24)");
        circle.setAttribute("fill", isDark ? "#1f2937" : "#fef3c7");
      } else if (isProcessing) {
        circle.setAttribute("stroke", colors[component % colors.length]);
        circle.setAttribute("stroke-width", "3");
        circle.classList.add("animate-pulse");
      } else {
        circle.setAttribute("stroke", colors[component % colors.length]);
        circle.setAttribute("stroke-width", "2");
      }

      const text = document.createElementNS(
        "http://www.w3.org/2000/svg",
        "text"
      );
      text.setAttribute("x", String(pos.x));
      text.setAttribute("y", String(pos.y));
      text.setAttribute("text-anchor", "middle");
      text.setAttribute("dominant-baseline", "central");
      text.setAttribute("fill", isDark ? "#fff" : "#000");
      text.setAttribute("font-family", "monospace");
      text.setAttribute("font-size", "14");
      text.setAttribute("font-weight", "600");
      text.textContent = String(i + 1);

      g.appendChild(circle);
      g.appendChild(text);
      svgRef.current!.appendChild(g);
    }
  }, [vertices, edges, parent, processingElements, isDark, phase]);

  return <svg ref={svgRef} viewBox="0 0 800 600" className="w-full h-full" />;
}
