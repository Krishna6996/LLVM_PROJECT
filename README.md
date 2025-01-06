# LLVM Project

## Overview
This project leverages LLVM to implement two custom passes for analyzing and optimizing code:

1. **SpeculativeKernelSafety.cpp**: 
   - Analyzes speculative execution paths to ensure kernel safety.
   - Identifies potential speculative vulnerabilities and inserts safeguards.

2. **VulnerableMask.cpp**:
   - Detects and masks vulnerable code patterns.
   - Focuses on mitigating risks associated with data leaks and side-channel attacks.

## Prerequisites
- **LLVM**: Ensure LLVM is installed on your system. You can download it from [LLVM Official Website](https://llvm.org/).
- **C++ Compiler**: LLVM requires a modern C++ compiler (e.g., `clang++` or `g++`).
- **CMake**: Used for building the project.
