// filepath: AoC.Framework/SolutionTemplate.cs
using System;
using System.Collections.Generic;
using System.Linq;

namespace AoC.Framework
{
    public class SolutionTemplate<TInput, TOutput> : ISolution<TInput, TOutput>
    {
        /// <summary>
        /// Parses the raw input string into a structured format
        /// </summary>
        /// <param name="rawInput">Raw text from input file</param>
        /// <returns>Parsed input in appropriate data structure</returns>
        public TInput ParseInput(string rawInput)
        {
            // This is the default implementation that can be overridden
            throw new NotImplementedException();
        }

        /// <summary>
        /// Solves Part 1 of the problem
        /// </summary>
        /// <param name="input">Parsed input</param>
        /// <returns>Solution output for part 1</returns>
        public TOutput SolvePart1(TInput input)
        {
            // This is a placeholder implementation
            throw new NotImplementedException();
        }

        /// <summary>
        /// Solves Part 2 of the problem
        /// </summary>
        /// <param name="input">Parsed input</param>
        /// <returns>Solution output for part 2</returns>
        public TOutput SolvePart2(TInput input)
        {
            // This is a placeholder implementation
            throw new NotImplementedException();
        }
    }
}