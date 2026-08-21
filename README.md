# Math Translator Plugin for Unreal Engine

**MathTranslator** is a lightweight, high-performance C++ and Blueprint plugin designed to parse, sanitize, and convert mathematical string expressions (LaTeX, Excel, MATLAB, LibreOffice, and standard plaintext math) directly into Unreal Engine-compatible formats and runtime calculations. It also includes geographic conversion utilities for translating real-world GPS latitude/longitude coordinates into Unreal 3D Cartesian coordinates.

---

## Features

* **Multi-Format Expression Translation:** Converts mathematical notation from LaTeX, MATLAB, Excel, LibreOffice Math, and standard string formats into Unreal `FMath`-compatible expressions.
* **Geographic Projection Utility:** Built-in `Convert Lat Long To Local Vector` function to map geographic latitude/longitude/altitude data onto Unreal Engine 3D Cartesian vector space ($X$-Forward, $Y$-Right, $Z$-Up).
* **Full Blueprint & C++ Support:** All functions are exposed via `UMathTranslatorLibrary` as Blueprint Pure nodes and C++ static utilities.
* **Zero External Dependencies:** Built strictly on native Unreal Engine C++ architecture (`CoreMinimal`, `UnrealMathUtility`) with minimal memory overhead and zero garbage collection (GC) impact during calculations.

---

## Compatibility

* **Unreal Engine Versions:** 5.0+ (Tested through 5.8)
* **Target Platforms:** Windows (Win64), Mac, Linux

---

## Quick Start Guide

### 1. Installation & Enabling the Plugin

1. Copy or clone the `MathTranslator` folder into your project's `Plugins/` directory:
`YourProject/Plugins/MathTranslator/`
2. Open your Unreal Engine project.
3. In the main menu, navigate to **Edit > Plugins**.
4. Search for **Math Translator**, check the **Enabled** box, and restart the editor when prompted.

---

### 2. Math Expression Translation (Blueprint Workflow)

1. Open any Actor or Widget Blueprint.
2. In the Event Graph, right-click and search for the **Translate Math Expression** node.
3. Pass a supported mathematical string into the `Expression Input` pin:
* **LaTeX:** `\frac{\sin(x)}{\cos(y)}`
* **MATLAB / Octave:** `power(x, 2) + log10(y)`
* **Excel / Spreadsheets:** `SQRT(x) + POWER(y, 2)`
* **LibreOffice Math:** `{ sin(x) } over { cos(y) }`


4. The node automatically parses Greek variables (`\theta`, `\phi`, `\alpha`, `\beta`, `\gamma`, `\lambda`) and outputs clean, native Unreal Engine syntax (e.g., `FMath::Pow(x, 2) + FMath::Logx(10.0f, y)`).
5. Route the output string into your runtime execution chain, UI text widget, or `Print String` node for validation.

---

### 3. Geographic Coordinate Translation (Blueprint Workflow)

1. In your Actor Blueprint, right-click and place the **Convert Lat Long To Local Vector** node.
2. Provide your real-world coordinates and scale inputs:
* **Latitude:** e.g., `37.7749`
* **Longitude:** e.g., `-122.4194`
* **Globe Radius:** e.g., `6371.0` (or the exact radius of your globe mesh in Unreal units)
* **Altitude:** e.g., `100.0`


3. Connect the output **Return Value (Vector)** to **Set Actor Location** or **Spawn Actor from Class** to align markers, telemetry nodes, or aircraft to Unreal's left-handed ($X$-Forward, $Y$-Right, $Z$-Up) coordinate system.

---

### 4. C++ Implementation

Include the header in your C++ class:

```cpp
#include "MathTranslatorLibrary.h"

// Example: Translate a LaTeX expression string to native syntax
FString NativeExpression = UMathTranslatorLibrary::TranslateMathExpression(TEXT("\\frac{\\sin(x)}{\\cos(y)}"));

// Example: Convert GPS coordinates to Unreal local 3D vector space
FVector LocalPosition = UMathTranslatorLibrary::ConvertLatLongToLocalVector(37.7749f, -122.4194f, 6371.0f, 100.0f);

```
