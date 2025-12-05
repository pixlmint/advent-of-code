// filepath: AoC.Framework/TestHelpers.cs
using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;

namespace AoC.Framework
{
    public static class TestHelpers
    {
        /// <summary>
        /// Loads test input data from a file path
        /// </summary>
        /// <param name="filePath">Path to the input file</param>
        /// <returns>Array of lines as strings</returns>
        public static string[] LoadTestData(string filePath)
        {
            if (!File.Exists(filePath))
                throw new FileNotFoundException($"Test data file not found: {filePath}");

            return File.ReadAllLines(filePath);
        }

        /// <summary>
        /// Asserts that two values are equal with a tolerance for floating point numbers
        /// </summary>
        /// <typeparam name="T">Type of value to compare</typeparam>
        /// <param name="expected">Expected value</param>
        /// <param name="actual">Actual computed value</param>
        public static void AssertEqual<T>(T expected, T actual)
        {
            if (!EqualityComparer<T>.Default.Equals(expected, actual))
                throw new InvalidOperationException($"Expected {expected}, but got {actual}");
        }

        /// <summary>
        /// Creates a mock input for testing purposes
        /// </summary>
        /// <param name="lines">Test lines to use</param>
        /// <returns>String representation of the test data</returns>
        public static string CreateMockInput(params string[] lines)
        {
            return string.Join("\n", lines);
        }
    }
}