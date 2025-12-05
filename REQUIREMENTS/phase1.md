# Advent of Code Framework Requirements Document

## Project Overview
A C# framework for solving Advent of Code problems, consisting of a CLI tool for local development and a web platform for visualization and telemetry collection.

## Phase 1: CLI Tool & Core Framework

### 1.1 Solution Execution

#### 1.1.1 Input File Processing
- **REQ-1.1.1.1**: The CLI shall accept a file path as input containing the problem data
- **REQ-1.1.1.2**: The CLI shall validate that the input file exists and is readable
- **REQ-1.1.1.3**: The CLI shall support both relative and absolute file paths
- **REQ-1.1.1.4**: The CLI shall provide clear error messages when file operations fail

#### 1.1.2 Solution Execution
- **REQ-1.1.2.1**: The CLI shall execute the specified solution against the provided input
- **REQ-1.1.2.2**: The CLI shall print the solution result to stdout
- **REQ-1.1.2.3**: The CLI shall catch and display exceptions with stack traces
- **REQ-1.1.2.4**: The CLI shall support both Part 1 and Part 2 solutions for each day
- **REQ-1.1.2.5**: The CLI shall allow users to specify which part(s) to run

#### 1.1.3 Command Structure
```
aoc run <year> <day> <part> --input <filepath>
```
- **REQ-1.1.3.1**: Year parameter shall be a 4-digit integer
- **REQ-1.1.3.2**: Day parameter shall be an integer between 1-25
- **REQ-1.1.3.3**: Part parameter shall be 1, 2, or "both"
- **REQ-1.1.3.4**: Input parameter shall be optional with a default location convention

### 1.2 Testing Facilities

#### 1.2.1 Test Runner
- **REQ-1.2.1.1**: The framework shall integrate with a standard C# testing framework (xUnit/NUnit)
- **REQ-1.2.1.2**: The CLI shall provide a command to run tests for a specific solution
- **REQ-1.2.1.3**: The CLI shall display test results with pass/fail status
- **REQ-1.2.1.4**: The CLI shall support running all tests or filtering by year/day

#### 1.2.2 Test Command Structure
```
aoc test <year> <day>
aoc test --all
```
- **REQ-1.2.2.1**: Test command shall execute unit tests for the specified solution
- **REQ-1.2.2.2**: Test command shall display execution time for each test
- **REQ-1.2.2.3**: Test command shall return appropriate exit codes (0 for success, non-zero for failures)

#### 1.2.3 Test Helpers
- **REQ-1.2.3.1**: Framework shall provide helper methods for common test assertions
- **REQ-1.2.3.2**: Framework shall provide utilities for loading test input files
- **REQ-1.2.3.3**: Framework shall support inline test data and file-based test data

### 1.3 Solution Scaffolding

#### 1.3.1 New Solution Command
```
aoc new <year> <day>
```
- **REQ-1.3.1.1**: Command shall create a new directory structure for the solution
- **REQ-1.3.1.2**: Command shall generate a solution file with minimal boilerplate
- **REQ-1.3.1.3**: Command shall generate a test file with example test cases
- **REQ-1.3.1.4**: Command shall create placeholder input files (input.txt, test-input.txt)
- **REQ-1.3.1.5**: Command shall prevent overwriting existing solutions without a force flag

#### 1.3.2 Directory Structure
```
/Solutions
  /Year2025
    /Day01
      - Solution.cs
      - Tests.cs
      - input.txt
      - test-input.txt
```
- **REQ-1.3.2.1**: Solutions shall be organized by year and day
- **REQ-1.3.2.2**: Each solution shall have its own namespace following the pattern `AoC.Y{year}.D{day}`

#### 1.3.3 Generated Solution Template
- **REQ-1.3.3.1**: Solution file shall implement a standard interface `ISolution`
- **REQ-1.3.3.2**: Template shall include placeholder methods for Part1 and Part2
- **REQ-1.3.3.4**: Template shall include a parsing method stub

#### 1.3.4 Generated Test Template
- **REQ-1.3.4.2**: Test file shall reference the test-input.txt file
- **REQ-1.3.4.3**: Test file shall follow standard naming conventions

### 1.4 Functional Programming Standards

#### 1.4.1 Solution Interface
```csharp
public interface ISolution<TInput, TOutput>
{
    TInput ParseInput(string rawInput);
    TOutput SolvePart1(TInput input);
    TOutput SolvePart2(TInput input);
}
```
- **REQ-1.4.1.1**: All solutions shall implement the ISolution interface
- **REQ-1.4.1.2**: Solutions shall separate parsing from solving logic
- **REQ-1.4.1.3**: Solutions shall be stateless and pure where possible

#### 1.4.2 Functional Utilities
- **REQ-1.4.2.1**: Framework shall provide common functional utilities (Map, Filter, Reduce, etc.)
- **REQ-1.4.2.2**: Framework shall provide immutable collection helpers
- **REQ-1.4.2.3**: Framework shall provide parsing combinators for common patterns
- **REQ-1.4.2.4**: Framework shall provide grid/matrix utilities for 2D problems

### 1.5 CLI Infrastructure

#### 1.5.1 Configuration
- **REQ-1.5.1.1**: CLI shall support a configuration file (aoc.json) for default settings
- **REQ-1.5.1.2**: Configuration shall specify default input file locations
- **REQ-1.5.1.3**: Configuration shall specify solution directory structure
- **REQ-1.5.1.4**: Command-line arguments shall override configuration file settings

#### 1.5.2 Output Formatting
- **REQ-1.5.2.1**: CLI shall use color coding for different message types (info, success, error)
- **REQ-1.5.2.2**: CLI shall support verbose mode for detailed logging
- **REQ-1.5.2.3**: CLI shall support quiet mode for minimal output
- **REQ-1.5.2.4**: CLI shall display execution time after solution completion

#### 1.5.3 Error Handling
- **REQ-1.5.3.1**: CLI shall catch and display parsing errors clearly
- **REQ-1.5.3.2**: CLI shall catch and display runtime errors with context
- **REQ-1.5.3.3**: CLI shall provide suggestions for common errors (file not found, etc.)
- **REQ-1.5.3.4**: CLI shall validate all parameters before execution
