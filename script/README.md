# LLVM Pass for Rust Programming Language

## Environment
Before you start to use LLVM pass for Rust codes, you need to install LLVM with gold plugin.

## Process
Rust LLVM Pass Libs -> clang++ -> *.so

[Transformation] *.rs -> rustc compiler -> *.bc -> opt (load *.so) -> *.bc -> clang Linker -> executable

[Analysis] *.rs -> rustc compiler -> *.bc -> opt (load .so) -> output
