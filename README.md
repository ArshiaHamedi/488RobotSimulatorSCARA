# 488RobotSimulatorSCARA-1
ROBSIM - SCARA Robot Simulation and Control - ENSC488 Project 1

This project is a comprehensive simulation and control system for a 4-DOF SCARA-type (RRPR) manipulator, developed as part of ENSC 488 - Introduction to Robotics. The system allows users to simulate and test robotic arm functions, perform forward and inverse kinematics, plan trajectories, and implement a basic controller.

The project is designed to work both on a hardware emulator and an actual SCARA robot. The emulator provides a graphical interface to visualize robotic motion, ensuring code safety before deploying it to the real-world hardware.

Features

1. Basic Matrix Computation Routines
-Implements fundamental matrix operations for transformations.
-Includes custom vector and matrix classes to facilitate calculations.
-Provides transformation matrices for rotation and translation operations.

2. Forward and Inverse Kinematics
-Computes the position and orientation of the end effector given joint angles (Forward Kinematics).
-Determines the necessary joint angles to reach a specified position and orientation (Inverse Kinematics).
-Implements functions KIN(), WHERE(), INVKIN(), and SOLVE().

3.Trajectory Planning
-Implements a joint-space trajectory planner.
-Allows movement through intermediate waypoints using parabolic blends or cubic splines.
-Ensures velocity continuity while executing motion paths.

4.Dynamic Simulation
-Uses symbolic derivation for dynamic equations and translates them into C-code.
-Simulates robotic motion under applied torques.

5. Controller Implementation
-Implements a trajectory-following control system.
-Sets servo gains for joint stiffness tuning.
-Applies torque limits to ensure realistic movement constraints.

6. User Interface
-Command-line based interface for selecting operations.
-Supports user input for defining robot configurations and executing movements.
-Displays trajectory plots and joint torque history.

Implementation Details

Kinematics

The Denavit-Hartenberg (D-H) convention is used to define the robotic structure. The transformations are calculated using:

Ti(i): Computes the transformation matrix from frame i-1 to i.

KIN(): Performs forward kinematics to compute the end-effector position.

INVKIN(): Computes the joint angles required to reach a target position.

Motion Execution

The system interacts with the robot/emulator using:

MoveToConfiguration(q): Moves the robot to the specified configuration.

MoveWithConfVelAcc(q, v, a): Moves with velocity and acceleration constraints.

Grasp(close): Controls the gripper to grab or release objects.

Code Structure

ProgrammingDemo.cpp: Contains the main program logic, including user interaction and motion execution.

ensc-488.h: Header file containing predefined APIs for robot control.

Matrix and Vector Classes: Custom implementations for matrix and vector operations to handle transformations.

How to Run

Compile the program using a C++ compiler supporting standard libraries.

Ensure ensc-488.h is included and linked correctly.

Run the executable and follow the on-screen instructions to test different functionalities.
