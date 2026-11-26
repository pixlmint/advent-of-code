# {{{ Setup
from elf import get_puzzle_input

YEAR = 2025
DAY = 1
USE_SAMPLE = False

# }}}

# {{{ Load Input
sample = """\
""".strip()

raw = sample if USE_SAMPLE else get_puzzle_input(YEAR, DAY)
lines = raw.splitlines()

# {{{ Parse Input
# Adapt this to the puzzle.
data = lines

# %% Explore
# Keep scratch variables here so they are easy to inspect and reuse.
# preview = data[:5]


# }}}
# }}}

# %% Part 1
def solve_part1(data):
    # Implement your solution.
    return None


part1_answer = solve_part1(data)
print("Part 1:", part1_answer)


# %% Part 2
def solve_part2(data):
    # Implement your solution, reusing the parsed data where helpful.
    return None


part2_answer = solve_part2(data)
print("Part 2:", part2_answer)

# vim: foldmethod=marker
