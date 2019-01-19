# Goal-oriented MCTS (G-MCTS) for Urban Planning


## Contents

- [Overview](#overview)
- [System Requirements](#system-requirements)
- [Installation Guide](#installation-guide)
- [Instructions for Use](#instructions-for-use)

# Overview

The development of an artificial intelligence agent with the ability to integrate heterogeneous expert systems and scenarios for complex urban planning issues is challenging. Here, we introduce an artificial intelligence (AI) system with a general systematizing function for democratic decision making by combining the traditional scenario planning with a novel goal-oriented Monte Carlo tree search (G-MCTS) method. The core processes are decomposing goals into subheuristics and substituting land cell rollout searching, which is dominated by the local optimum, with goal-oriented searching, which is dominated by the global optimum, step by step in the G-MCTS. This methodology integrate heterogeneous expert systems with decision makers and build a bridge between subjective computations and objective judgments. Additionally, our planning AI with G-MCTS automatically provides strategies for satisfying multiple goals without deliberate policy programming. In general, we provide an approach for making the most comprehensive planning decisions via human-AI interaction at the political level in complex real-world systems.

## Contents

- StepGoal.exe: the demo software to find the best solution with strategies
- Instruction for use.pdf: how to run the software using the test data
- Data: package data for tests and corresponding results
- DL1.cpp: the main code of the G-MCTS for urban planning

The "StepGoal.exe" "Instruction for use.pdf" and "Data" can be downloaded for Reproduction instruction. The DL1 can be downloaded as a main framework or a search function for further research.

# System Requirements

## Hardware Requirements

The demo `G-MCTS for urban planning` softwore requires only a standard computer with enough RAM to support the operations defined by a user. For minimal performance, this will be a computer with about 1 GB of RAM. For optimal performance, we recommend a computer with the following specs:

RAM: 16+ GB  
CPU: 4+ cores, 3.3+ GHz/core

## Software Requirements

### OS Requirements

This package is supported for *Windows* operating systems. 

# Installation Guide

The demo `G-MCTS for urban planning` software is an EXE softwore run directly without need to install on a computer.

# Instructions for Use

Please see the help file: Instruction for use.pdf
