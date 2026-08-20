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

## Installation Guide

### Option 1: Install as a Project Plugin (Recommended)

1. Download or clone this repository.
2. Locate the `MathTranslator` plugin folder (containing `MathTranslator.uplugin`, `Source/`, and `Resources/`).
3. In your Unreal Engine project root directory, create a folder named `Plugins` if one does not already exist:
   ```text
   YourProject/
   ├── Content/
   ├── Source/
   ├── Config/
   ├── Plugins/
   │   └── MathTranslator/
   │       ├── Resources/
   │       ├── Source/
   │       └── MathTranslator.uplugin
   └── YourProject.uproject
