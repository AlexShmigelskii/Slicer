# CLI .STL → .goo Slicer (C)

This project is an experimental command-line slicer written in C.  
Its goal is to load a 3D model in **binary STL**, slice it into layers and export the result into an **Elegoo `.goo`** file suitable for resin 3D printers.

## Implemented Features
- Internal geometry structures
- ASCII/Binary STL writer
- Binary STL reader
- Mesh initialization & cleanup
- Mesh statistics

## Building
```bash
make
```

## Cleaning built outputs
```bash
make clean
```

## Comments
- Tested on macOS M1
