# Advent of Code Framework Requirements Document

## Project Overview
A C# framework for solving Advent of Code problems, consisting of a CLI tool for local development and a web platform for visualization and telemetry collection.

## Phase 2: Web Platform

### 2.1 Problem Listing

#### 2.1.1 Problem Browser
- **REQ-2.1.1.1**: Web app shall display a list of all available problems organized by year
- **REQ-2.1.1.2**: Web app shall show completion status for each problem
- **REQ-2.1.1.3**: Web app shall provide filtering by year and completion status
- **REQ-2.1.1.5**: Each problem shall display its title and day number

#### 2.1.2 Problem Detail View
- **REQ-2.1.2.1**: Selecting a problem shall navigate to a detail page
- **REQ-2.1.2.2**: Detail page shall display problem metadata (year, day, title)
- **REQ-2.1.2.3**: Detail page shall show available actions (Run Tests, Run Solution)

### 2.2 Test Execution

#### 2.2.1 Test Runner UI
- **REQ-2.2.1.1**: Users shall be able to trigger test execution from the web interface
- **REQ-2.2.1.2**: Test execution shall run asynchronously with progress indication
- **REQ-2.2.1.3**: Users shall be able to cancel long-running test executions

#### 2.2.2 Test Reports
- **REQ-2.2.2.1**: Test results shall be displayed in a structured, readable format
- **REQ-2.2.2.2**: Report shall show overall pass/fail status
- **REQ-2.2.2.3**: Report shall list individual test cases with their status
- **REQ-2.2.2.4**: Report shall display execution time for each test
- **REQ-2.2.2.5**: Failed tests shall display expected vs actual values
- **REQ-2.2.2.6**: Failed tests shall display error messages and stack traces

#### 2.2.3 Test Report Styling
- **REQ-2.2.3.1**: Reports shall use a modern, responsive design
- **REQ-2.2.3.2**: Reports shall be collapsible/expandable for nested test suites
- **REQ-2.2.3.3**: Reports shall highlight performance outliers
- **REQ-2.2.3.4**: Reports shall provide diff view for assertion failures

### 2.3 Solution Execution

#### 2.3.1 Input Upload
- **REQ-2.3.1.1**: Users shall be able to upload their problem input file
- **REQ-2.3.1.2**: Upload shall validate file size (max 10MB)
- **REQ-2.3.1.3**: Upload shall validate file format (text files only)
- **REQ-2.3.1.4**: Users shall be able to paste input directly into a text area
- **REQ-2.3.1.5**: System may not store any of the uploaded inputs

#### 2.3.2 Execution Modes
- **REQ-2.3.2.1**: Users shall choose between "Solve" and "Animate" modes
- **REQ-2.3.2.2**: Users shall be able to select which part (1, 2, or both) to execute

#### 2.3.3 Direct Solve Mode
- **REQ-2.3.3.1**: Solve mode shall execute the solution without animation
- **REQ-2.3.3.2**: Result shall be displayed prominently upon completion
- **REQ-2.3.3.3**: Execution time shall be displayed
- **REQ-2.3.3.4**: Memory usage shall be displayed
- **REQ-2.3.3.5**: Timeout shall be enforced (configurable, default 60 seconds)

### 2.4 Animation Framework

#### 2.4.1 Animation Interface
```csharp
public interface IAnimatableSolution<TInput, TOutput, TState>
{
    IEnumerable<AnimationFrame<TState>> SolveWithAnimation(TInput input);
}

public class AnimationFrame<TState>
{
    public TState State { get; init; }
    public string Description { get; init; }
    public Dictionary<string, object> Metadata { get; init; }
}
```
- **REQ-2.4.1.1**: Solutions may optionally implement IAnimatableSolution
- **REQ-2.4.1.2**: Animation shall yield frames representing intermediate states
- **REQ-2.4.1.3**: Each frame shall contain state data and descriptive metadata

#### 2.4.2 State Representation Standards
- **REQ-2.4.2.1**: Framework shall provide standard state types for common scenarios:
  - Grid/2D array states
  - Graph states (nodes and edges)
  - List/sequence states
  - Tree states
  - Custom visualization states
- **REQ-2.4.2.2**: State types shall be serializable to JSON
- **REQ-2.4.2.3**: State types shall include rendering hints (colors, highlights, annotations)

#### 2.4.3 Animation Player UI
- **REQ-2.4.3.1**: Web app shall provide playback controls (play, pause, step forward, step back)
- **REQ-2.4.3.2**: Web app shall provide speed control (0.5x, 1x, 2x, etc.)
- **REQ-2.4.3.3**: Web app shall display current frame number and total frames
- **REQ-2.4.3.4**: Web app shall display frame description/metadata
- **REQ-2.4.3.5**: Web app shall allow jumping to specific frames
- **REQ-2.4.3.6**: Web app shall automatically render state based on type

#### 2.4.4 Visualization Renderers
- **REQ-2.4.4.1**: Framework shall provide built-in renderers for standard state types
- **REQ-2.4.4.2**: Grid renderer shall display 2D arrays with cell coloring
- **REQ-2.4.4.3**: Graph renderer shall display nodes and edges with layout algorithm
- **REQ-2.4.4.4**: List renderer shall display sequences with highlighting
- **REQ-2.4.4.5**: Renderers shall support zooming and panning (optional)
- **REQ-2.4.4.6**: Renderers shall support responsive sizing (optional)

### 2.5 Telemetry Collection

#### 2.5.1 Metrics to Collect
- **REQ-2.5.1.1**: System shall collect execution time (total and per-part)
- **REQ-2.5.1.2**: System shall collect peak memory usage
- **REQ-2.5.1.3**: System shall collect average memory usage
- **REQ-2.5.1.4**: System shall collect GC statistics (collections per generation)
- **REQ-2.5.1.5**: System shall collect CPU usage percentage
- **REQ-2.5.1.6**: System shall collect input file size
- **REQ-2.5.1.7**: System shall collect frame count (for animations)
- **REQ-2.5.1.8**: System shall collect timestamp and execution date

#### 2.5.2 Telemetry Storage
- **REQ-2.5.2.1**: Telemetry shall be stored in a time-series database
- **REQ-2.5.2.2**: Telemetry shall be associated with solution version/commit hash
- **REQ-2.5.2.3**: Telemetry shall be queryable by year, day, user, and date range
- **REQ-2.5.2.4**: Telemetry shall be retained for at least 1 year

#### 2.5.3 Telemetry Display
- **REQ-2.5.3.1**: Web app shall display telemetry summary after each execution
- **REQ-2.5.3.2**: Web app shall show historical performance graphs
- **REQ-2.5.3.3**: Web app shall compare current run against historical averages
- **REQ-2.5.3.4**: Web app shall highlight performance improvements/regressions
- **REQ-2.5.3.5**: Web app shall provide leaderboard by execution time
- **REQ-2.5.3.6**: Web app shall provide leaderboard by memory efficiency

#### 2.5.4 Telemetry Collection Implementation
- **REQ-2.5.4.1**: Telemetry collection shall not significantly impact performance
- **REQ-2.5.4.2**: Telemetry collection shall use System.Diagnostics APIs
- **REQ-2.5.4.3**: Memory profiling shall use GC.GetTotalMemory and GC.CollectionCount
- **REQ-2.5.4.4**: CPU profiling shall use Process.TotalProcessorTime
- **REQ-2.5.4.5**: Telemetry shall be collected in a separate thread/task

### 2.6 Web Architecture

#### 2.6.1 Backend API
- **REQ-2.6.1.1**: Backend shall be built with ASP.NET Core Web API
- **REQ-2.6.1.2**: API shall provide RESTful endpoints for all operations
- **REQ-2.6.1.4**: API shall implement rate limiting per user (session based)
- **REQ-2.6.1.5**: API shall validate all inputs and sanitize outputs

#### 2.6.2 Frontend
- **REQ-2.6.2.1**: Frontend shall be a Single Page Application (SPA)
- **REQ-2.6.2.2**: Frontend shall use Blazor WebAssembly
- **REQ-2.6.2.3**: Frontend shall be responsive and mobile-friendly
- **REQ-2.6.2.4**: Frontend shall provide real-time updates via SignalR

---

## Cross-Phase Requirements

### 3.1 Documentation
- **REQ-3.1.1**: Framework shall include comprehensive API documentation
- **REQ-3.1.5**: CLI shall provide help text for all commands

### 3.2 Extensibility
- **REQ-3.2.1**: Framework shall support custom utility libraries
- **REQ-3.2.2**: Framework shall support custom visualization renderers
- **REQ-3.2.3**: Framework shall support plugins for additional metrics
- **REQ-3.2.4**: Framework shall use dependency injection throughout

### 3.3 Performance
- **REQ-3.3.1**: CLI shall start up in under 500ms
- **REQ-3.3.2**: Solution scaffolding shall complete in under 1 second
- **REQ-3.3.3**: Web page load time shall be under 2 seconds
- **REQ-3.3.4**: Test report generation shall complete within 5 seconds

### 3.4 Compatibility
- **REQ-3.4.1**: Framework shall target .NET 9.0 or later
- **REQ-3.4.2**: Framework shall support Windows, macOS, and Linux
- **REQ-3.4.4**: Framework shall follow C# coding conventions and style guidelines
