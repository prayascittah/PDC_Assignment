"use client";

interface ParentTableProps {
  parent: number[];
  isDark: boolean;
}

export default function ParentTable({ parent, isDark }: ParentTableProps) {
  // Build component groups
  const components = new Map<number, number[]>();
  parent.forEach((p, i) => {
    if (!components.has(p)) {
      components.set(p, []);
    }
    components.get(p)!.push(i + 1);
  });

  // Create component strings
  const componentStrings = new Map<number, string>();
  components.forEach((vertices, root) => {
    if (vertices.length === 1) {
      componentStrings.set(root, "Singleton");
    } else {
      componentStrings.set(root, `{${vertices.join(", ")}}`);
    }
  });

  const borderColor = isDark ? "#1f2937" : "#e5e7eb";
  const headerBg = isDark ? "#1f2937" : "#f3f4f6";
  const tableBg = isDark ? "#000000" : "#ffffff";

  return (
    <div
      className="border rounded overflow-hidden"
      style={{ background: tableBg, borderColor }}
    >
      <table className="w-full text-xs">
        <thead>
          <tr style={{ background: headerBg }}>
            <th className="p-2 text-center border-b" style={{ borderColor }}>
              Vertex
            </th>
            <th className="p-2 text-center border-b" style={{ borderColor }}>
              Parent
            </th>
            <th className="p-2 text-center border-b" style={{ borderColor }}>
              Component
            </th>
          </tr>
        </thead>
        <tbody>
          {parent.map((p, i) => (
            <tr
              key={i}
              className="border-b last:border-b-0"
              style={{ borderColor }}
            >
              <td className="p-2 text-center">{i + 1}</td>
              <td className="p-2 text-center">{p + 1}</td>
              <td className="p-2 text-center">{componentStrings.get(p)}</td>
            </tr>
          ))}
        </tbody>
      </table>
    </div>
  );
}
