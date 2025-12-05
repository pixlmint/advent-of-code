// filepath: AoC.Framework/ISolution.cs
using System;

namespace AoC.Framework
{
    public interface ISolution<TInput, TOutput>
    {
        TInput ParseInput(string rawInput);
        TOutput SolvePart1(TInput input);
        TOutput SolvePart2(TInput input);
    }
}