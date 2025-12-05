## Appendix A: Example Solution Structure

```csharp
namespace AoC.Y2025.D01;

public class Solution : ISolution<int[], int>, IAnimatableSolution<int[], int, GridState>
{
    public int[] ParseInput(string rawInput) =>
        rawInput.Split('\n')
            .Select(int.Parse)
            .ToArray();

    public int SolvePart1(int[] input) =>
        input.Sum();

    public int SolvePart2(int[] input) =>
        input.Where(x => x % 2 == 0).Sum();

    public IEnumerable<AnimationFrame<GridState>> SolveWithAnimation(int[] input)
    {
        var sum = 0;
        for (int i = 0; i < input.Length; i++)
        {
            sum += input[i];
            yield return new AnimationFrame<GridState>
            {
                State = new GridState { /* ... */ },
                Description = $"Added {input[i]}, sum is now {sum}"
            };
        }
    }
}
```

## Appendix B: CLI Usage Examples

```bash
# Create new solution
aoc new 2025 1

# Run solution
aoc run 2025 1 1 --input inputs/day01.txt
aoc run 2025 1 both

# Run tests
aoc test 2025 1
aoc test --all --verbose

# List solutions
aoc list
aoc list --year 2025 --incomplete
```
