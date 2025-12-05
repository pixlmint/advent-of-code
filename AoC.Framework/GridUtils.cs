// filepath: AoC.Framework/GridUtils.cs
using System;
using System.Collections.Generic;
using System.Linq;

namespace AoC.Framework
{
    public static class GridUtils
    {
        // 2D grid utilities
        public static T[,] ParseGrid<T>(string[] lines, Func<char, T> parser)
        {
            var height = lines.Length;
            var width = lines[0].Length;
            var grid = new T[height, width];
            
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    grid[y, x] = parser(lines[y][x]);
                }
            }
            
            return grid;
        }

        public static IEnumerable<(int x, int y)> GetAdjacentCells(int x, int y)
        {
            // 4-directional adjacent cells (up, right, down, left)
            yield return (x, y - 1); // up
            yield return (x + 1, y); // right  
            yield return (x, y + 1); // down
            yield return (x - 1, y); // left
        }

        public static IEnumerable<(int x, int y)> GetAdjacentCells8(int x, int y)
        {
            // 8-directional adjacent cells
            for (int dy = -1; dy <= 1; dy++)
            {
                for (int dx = -1; dx <= 1; dx++)
                {
                    if (dx != 0 || dy != 0) // Skip center cell
                        yield return (x + dx, y + dy);
                }
            }
        }
    }
}