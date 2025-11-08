# Connected Components Visualizer - Next.js

A modern Next.js application for visualizing the Connected Components Algorithm (Algorithm B).

**By Jayanth P (2023BCS0040)**

## Features

- ✨ Built with Next.js 14 + TypeScript
- 🎨 Tailwind CSS for styling
- 🔤 Geist Mono font
- 🖤 Black & white theme
- 📊 Interactive graph editor
- 🎬 Step-by-step animation
- ⚡ Serial/Parallel algorithm selection
- 📈 Real-time parent vector display

## Installation

```bash
cd connected-components-nextjs
npm install
```

## Running the Application

```bash
# Development mode
npm run dev

# Production build
npm run build
npm start
```

Open [http://localhost:3000](http://localhost:3000) in your browser.

## Usage

1. **Home Page**: Set vertices, choose algorithm (Serial/Parallel), create graph
2. **Visualization Page**: Watch algorithm animate through iterations
3. **Controls**: Play/Pause, Step Forward, Reset

## Project Structure

```
src/
├── app/
│   ├── layout.tsx          # Root layout with Geist Mono font
│   ├── page.tsx            # Home page with graph editor
│   ├── visualization/
│   │   └── page.tsx        # Visualization page
│   └── globals.css         # Global styles
├── components/
│   ├── GraphEditor.tsx     # Graph input component
│   ├── AlgorithmVisualizer.tsx  # Main visualizer
│   ├── GraphVisualization.tsx   # SVG graph rendering
│   └── ParentTable.tsx     # Parent vector table
```

## Algorithm

Time Complexity: **O((|V| + |E|) × log |V|)**

## Tech Stack

- Next.js 14 (App Router)
- React 18
- TypeScript
- Tailwind CSS
- Geist Mono Font
