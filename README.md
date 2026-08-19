# MathTranslator Plugin for Unreal Engine

**MathTranslator** is a lightweight, high-performance C++ and Blueprint plugin designed to parse, sanitize, and convert mathematical string expressions (LaTeX, Excel, MATLAB, LibreOffice, and standard plaintext math) directly into Unreal Engine-compatible formats and runtime calculations. It also includes geographic conversion utilities for translating latitude/longitude coordinates into Unreal 3D Cartesian coordinates.

---

## Features

- **Multi-Format Expression Translation:** Converts mathematical notation from LaTeX, MATLAB, Excel, and standard string formats into Unreal `FMath` compatible expressions.
- **Geographic Projection Utility:** Built-in `ConvertLatLongToLocalVector` function to map geographic latitude/longitude/altitude data onto Unreal Engine 3D vector space.
- **Full Blueprint & C++ Support:** All functions are exposed via `UMathTranslatorLibrary` as Blueprint Pure/Callable nodes and C++ static utilities.
- **Zero External Dependencies:** Native Unreal Engine C++ architecture with minimal memory overhead.

---

## Compatibility

- **Unreal Engine:** 5.0+ (Tested through 5.8)
- **Platforms:** Windows (Win64), Mac, Linux

---

## Installation Guide

### Option 1: Install as a Project Plugin (Recommended)

1. Download or clone this repository.
2. Locate the `MathTranslator` plugin folder (containing `MathTranslator.uplugin`, `Source/`, and `Resources/`).
3. In your Unreal Engine project directory, create a folder named `Plugins` if one does not already exist:
   ```text
   YourProject/
   ├── Content/
   ├── Source/
   ├── Config/
   ├── Plugins/
   │   └── MathTranslator/
   └── YourProject.uproject
