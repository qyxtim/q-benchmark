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

The tool has two versions: `main` and `v2`. You are in `main` now. `main` version of the logging time function is more accurate than v2, but it cannot deal with some complex command line arguments.

Therefore, you can switch to `v2` branch if:
1. `main` cannot parse your command line arguments
2. you cares more about the time comparison than the actual running time.

To use the `main` version:
1. Make sure you have `g++ >= 4.8.1` or `clang++ >= 3.3` installed.
2. Run `make`.
3. Run benchmark by using `./bin/release/qbench [options] command1 command2 ...`
    - Options:
      - `-w` times: assign the number of warmup function calls to the times. The default number of warmup function calls is `3`.
      - `-l` lower: lower bound of the number of test runs. The default number is 100.
      - `-h` upper: upper bound of the number of test runs. The default number is 1000.

# Example

To benchmark the `sleep` function, you can run:

```
./qbench -w 3 -l 1 -h 5 "sleep 0.3" "sleep 0.3"
```

Then q-benchmark will output the statistical results:
```
Benchmark: sleep
  Time: 307.686333 ms(mean) ± 1.484943 ms(std)
  Range: 306.007000 ms(min) … 309.618000 ms(max) 3 runs

Benchmark: sleep
  Time: 306.958600 ms(mean) ± 1.506971 ms(std)
  Range: 304.997000 ms(min) … 309.218000 ms(max) 5 runs
```

# License

[MIT License](./LICENSE)