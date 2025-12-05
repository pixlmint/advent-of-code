// filepath: AoC.Framework/InputProcessor.cs
using System;
using System.IO;
using System.Text.RegularExpressions;

namespace AoC.Framework
{
    public static class InputProcessor
    {
        /// <summary>
        /// Validates that an input file exists and is readable
        /// </summary>
        /// <param name="filePath">Path to the input file</param>
        /// <returns>True if file exists, false otherwise</returns>
        public static bool ValidateInputFile(string filePath)
        {
            return File.Exists(filePath);
        }

        /// <summary>
        /// Reads all lines from an input file
        /// </summary>
        /// <param name="filePath">Path to the input file</param>
        /// <returns>Array of strings representing each line in the file</returns>
        public static string[] ReadInputFile(string filePath)
        {
            if (!ValidateInputFile(filePath))
                throw new FileNotFoundException($"Input file not found: {filePath}");

            return File.ReadAllLines(filePath);
        }

        /// <summary>
        /// Gets the default input path for a specific solution day
        /// </summary>
        /// <param name="year">Year of the solution</param>
        /// <param name="day">Day of the solution</param>
        /// <returns>Default file path for input data</returns>
        public static string GetDefaultInputPath(int year, int day)
        {
            return $"./Solutions/Year{year}/Day{day:00}/input.txt";
        }

        /// <summary>
        /// Gets the test input path for a specific solution day
        /// </summary>
        /// <param name="year">Year of the solution</param>
        /// <param name="day">Day of the solution</param>
        /// <returns>Test file path for input data</returns>
        public static string GetTestInputPath(int year, int day)
        {
            return $"./Solutions/Year{year}/Day{day:00}/test-input.txt";
        }

        /// <summary>
        /// Parses input lines into integers
        /// </summary>
        /// <param name="lines">Lines to parse</param>
        /// <returns>Array of integers</returns>
        public static int[] ParseIntLines(string[] lines)
        {
            var result = new int[lines.Length];
            for (int i = 0; i < lines.Length; i++)
            {
                if (!int.TryParse(lines[i], out result[i]))
                    throw new FormatException($"Unable to parse integer from line: {lines[i]}");
            }
            return result;
        }

        /// <summary>
        /// Parses input lines using regex pattern matching
        /// </summary>
        /// <param name="lines">Lines to match</param>
        /// <param name="pattern">Regex pattern for parsing</param>
        /// <returns>Matches from the regex</returns>
        public static MatchCollection ParseWithPattern(string[] lines, string pattern)
        {
            var regex = new Regex(pattern);
            return regex.Matches(string.Join("\n", lines));
        }
    }
}