#!/usr/bin/env python

from itertools import permutations
from typing import Iterator, Tuple


def gen_permutations(
    case_sets: list[list[str]],
) -> Iterator[Tuple[Tuple[str, ...], list[str]]]:
    for case_set in case_sets:
        cases = (case_set[:i] for i in range(1, len(case_set) + 1))
        for case in cases:
            for permutation in permutations(case):
                yield permutation, case
