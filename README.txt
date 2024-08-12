README.txt

Project Overview
================
This project is a terminal-based graphing calculator developed in C++ with a Text User Interface (TUI) implemented using the ncurses library. The calculator is designed for users to input complex mathematical expressions, adjust various parameters such as the domain and the number of sample points, and visualize the resulting graph directly within the terminal environment. The software is especially useful for educational purposes, mathematical analysis, and scenarios where graphical environments are limited or unavailable.

Features Overview
-----------------
- Expression Input: Support for a wide range of mathematical functions and operations.
- Domain Customization: Flexible adjustment of the evaluation domain.
- Variable Configuration: Ability to change the independent variable.
- Graph Visualization: Real-time plotting within the terminal.
- File Output: Export results to a text file for further analysis.

Building the Software
=====================
Prerequisites
-------------
Before proceeding with the build, ensure that the following dependencies are installed on your system:

- CMake: Version 3.29 or later. CMake is used to generate the makefiles needed to compile the project.
- C++ Compiler: Must support C++20, as the project utilizes features from this standard.
- ncurses Library: Required for rendering the TUI components. Ensure it is properly installed and linked during the build process.

Build Instructions
------------------
Follow these steps to build the software:

1. Clone the Repository:
   git clone <repository-url>
   cd project-directory

2. Create a Build Directory:
   mkdir build
   cd build

3. Run CMake to Generate Makefiles:
   cmake ..
   This step configures the project based on your environment, checks for required dependencies, and generates the necessary makefiles.

4. Compile the Project:
   make
   This command compiles the source code into an executable. Any compilation errors related to dependencies or code issues will be displayed here.

Installing the Software
-----------------------
To install the software to your system, execute the following command after building:

make install

This command installs the calculator executable to the ${CMAKE_INSTALL_PREFIX}/bin directory and copies the necessary header files to ${CMAKE_INSTALL_PREFIX}/include. Ensure that your environment’s PATH variable includes the installation directory so that the calculator can be run from any location in the terminal.

Running the Software
====================
Starting the Calculator
-----------------------
To start the calculator, navigate to the installation directory (or ensure it is in your PATH) and run:

./calculator

This command launches the TUI, allowing you to interact with the calculator's features.

Main Features and Options
-------------------------
1. Run Calculations
   This feature allows you to execute the currently loaded mathematical expression over the specified domain. The results can be displayed on the screen and optionally saved to a file.

   - Saving Output to a File: When you choose to save the results, the calculator will prompt you to enter a filename. The output file will contain the input values (within the domain) and the corresponding output values, separated by whitespace. Special values like 'nan' and 'inf' are used to represent undefined results, such as those arising from invalid operations like division by zero.

   - Output Format Example:
     -10 0.544021
     -9 0.412118
     -8 -0.989358
     ...

2. Input Function
   The input function option lets you define the mathematical expression to be evaluated. The default function is sin(x), but you can input any valid expression using supported functions, operations, and constants.

   Supported Mathematical Functions:
   - Trigonometric: sin(x), cos(x), tan(x), arcsin(x), arccos(x), arctan(x)
   - Logarithmic: ln(x), log(x)
   - Square root: sqrt(x)

   Supported Arithmetic Operations:
   - Addition: x + 3
   - Subtraction: x - 3
   - Multiplication: x * 3
   - Division: x / 3
   - Exponentiation: x ^ 3

   Supported Constants:
   - e: Euler's Number (2.71828)
   - pi: Pi (3.14159)
   - c: Speed of light in a vacuum (299792458 m/s)
   - g: Acceleration due to gravity (9.80665 m/s²)
   - h: Planck's Constant (6.62607015e-34 J*s)
   - k: Boltzmann Constant (1.380649e-23 J/K)
   - G: Gravitational Constant (6.67430e-11 m³/kg/s²)
   - R: Universal Gas Constant (8.314462618 J/(mol*K))

   Function Example:
   sin(pi) + 2 * ln(3 / x) - sqrt(4)

3. Change Domain
   This option allows you to set the domain for the function evaluation. The domain must be specified as an integer range and must have a minimum length of 10 units. The default domain is [-100, 100]. If the start value is set greater than the end value or if the length is less than 10, an error is triggered. For example, if the start value is set to 120, which is greater than 100, the calculator will automatically set the start value to 90.

   Default:
   Domain: [-100, 100]

4. Change Independent Variable
   You can change the independent variable in the function. The variable must be a single alphabetic character and cannot be one of the reserved characters (e, c, g, h, k, G, R, i, j). If an invalid variable is entered, an error is raised, and the variable will revert to the last valid value.

5. Set Sample Points
   This option allows you to adjust the number of sample points used during the function evaluation. More sample points provide a higher resolution graph but require more computational resources.

   Valid Range: 100 to 100,000
   Example:
   Number of Samples: 10,000

6. Set Export Directory
   You can specify the directory where output files will be saved. The directory path must exist and be writable. If an invalid path is entered, the calculator will display an error, and the previous valid path will be retained.

   Example:
   /home/user/calculator/output/

7. View Help
   The help option provides a multi-page guide to using the calculator, including detailed explanations of each feature, supported functions, and error messages. You can navigate between help pages using the arrow keys.

8. Quit
   This option safely exits the calculator, ensuring that any open files are properly closed and memory is freed.

Graph Visualization
===================
The calculator includes a feature to visualize the function graph within the terminal. By default, the graph window covers the range [-10, 10] on the x-axis and [-5, 5] on the y-axis. You can adjust the graph window size by changing the domain and range.

- Adjusting the Window:
  When viewing the graph, you can press 'W' to enter new domain and range values. These values must be within the overall domain set earlier, and the minimums must be less than the maximums.

- Plotting Points:
  The graph is plotted using asterisks (*) for the function values. Undefined values (e.g., division by zero) are skipped during plotting.

- Axis and Origin Display:
  The graph will display x and y axes if the range includes zero. The origin is marked with a '+' sign where the axes intersect.

Error Handling and Restrictions
===============================
Expression Errors:
------------------
- Invalid Expressions: If you input an expression with syntax errors (e.g., unmatched parentheses, unsupported functions, or invalid variables), the calculator will reset to the default expression (sin(x)) and display an error message.
  
Domain Errors:
--------------
- Invalid Domain: The domain must be an integer range with a minimum length of 10. If you attempt to set a domain where the start value is greater than the end value, or the length is less than 10, the calculator will reset to a default domain and display an error. For example, if the start value is set to 120, which is greater than 100, the calculator will automatically set the start value to 90.

Variable Errors:
----------------
- Invalid Variable: The independent variable must be a single alphabetic character. Reserved characters cannot be used as the variable. If an invalid character is entered, the calculator will display an error and retain the previous valid variable.

File Handling Errors:
---------------------
- Invalid Export Path: If the specified directory for saving output files does not exist or is not writable, the calculator will display an error message, and the output will not be saved. The previous valid directory will be retained.

Example Workflow
================
1. Start the calculator:
   ./calculator

2. Change the domain to [0, 100]:
   - Navigate to "Change Domain" and set the start to 0 and the end to 100.

3. Input a new function:
   - Navigate to "Input Function" and enter cos(x).

4. Run the calculation:
   - Select "Run," then choose whether to view the graph or save the output to a file.

5. Save the output:
   - When prompted, enter output as the filename. The output will be saved as output.txt in the specified directory.

6. Exit the calculator:
   - Select "Quit" to safely exit the program.