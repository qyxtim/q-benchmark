# q-benchmark

q-benchmark is a command-line benchmark tool inspired by an awesome command-line benchmark tool [hyperfine](https://github.com/sharkdp/hyperfine).

This tool uses about `250` lines of code to build a simple replica of hyperfine.

# Features

- Support for any shell commands
- Support for defining min/max number of test runs
- Report mean/std/range for tests
- warmup before running the benchmark

# Usage

q-benchmark only supports macOS/Linux now.

1. Make sure you have `g++ >= 4.8.1` or `clang++ >= 3.3` installed.
2. Run `make` in the folder.
3. Run benchmark by using `./qbench [options] command1 command2 ...`
    - Options:
      - `-w` times: assign the number of warmup function calls to the times. The default number of warmup function calls is `3`.
      - `-l` lower: lower bound of the number of test runs.
      - `-h` upper: upper bound of the number of test runs.

# Example

To benchmark the `sleep` function, you can run:

```
./qbench -w 3 -l 1 -h 5 "sleep 0.3" "sleep 0.3"
```

Then q-benchmark will output the statistical results:
```
Benchmark: sleep
  Time: 307.800000 ms(mean) ± 1.326650 ms(std)
  Range: 306.000000 ms(min) … 310.000000 ms(max) 5 runs

Benchmark: sleep
  Time: 311.000000 ms(mean) ± 0.000000 ms(std)
  Range: 311.000000 ms(min) … 311.000000 ms(max) 1 runs
```

# License

[MIT License](./LICENSE)