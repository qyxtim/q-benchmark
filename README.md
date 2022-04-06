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

The tool has two versions: `main` and `v2`. You are in `v2` now. `v2` version of the logging time function is less accurate than `main`, but it can deal with some complex command line arguments.

Therefore, you can switch to `main` branch if:
1. you don't have complex command line arguments
2. you cares about the actual running time.

To use the `v2` version:
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
./bin/release/qbench -w 3 -l 1 -h 5 "sleep 0.3" "sleep 0.3"
```

Then q-benchmark will output the statistical results:
```
Benchmark: sleep 0.3 (3 runs)
  Time: 315.014667 ms(mean) ± 1.808066 ms(std)
  Range: 312.542000 ms(min) … 316.815000 ms(max)

Benchmark: sleep 0.3 (5 runs)
  Time: 313.883600 ms(mean) ± 2.265409 ms(std)
  Range: 310.041000 ms(min) … 316.447000 ms(max)
```

# License

[MIT License](./LICENSE)