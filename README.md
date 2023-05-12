# A minimal example for playing with fuzzing

I created this repository to learn about fuzzing. It contains a very small C++
program created for the express purpose of being fuzzed. The program is a
hand-written parser and evaluator for a simple stack-based language. All it can
do is add integers. The program has no dependencies beyond the C++ standard
library, so it should be easy to build on any platform.

## Intentional bugs

The code has some intentionally inserted bugs that cause it to crash under very
specific circumstances, e.g., if the final result evaluates to 42, or if an
addition is executed where the first operand is 191. Search the code for
`assert` to find them all.

There may also, of course, be some unintentional bugs in the code. I'm no C++
expert.

## Please excuse my C++ style

The more recent years of my career have been spent with OCaml, Rust and Python.
I know this code reeks of ML-isms, particularly the structs for simulating
variant types, and the way failures are returned using (simulated) variants. I
understand this isn't idiomatic C++. Hopefully the code is easy to follow: it
uses a finite state machine for parsing, and a stack-based evaluator that
processes the stream of tokens in order.

The reason I wrote this in C++ is because the fuzzing tools seem to support C
and C++ best, and I didn't want to write pure C because of the lack of built-in
dynamically-growing `vector` and `string` types.

## How to run it

### Natively on macOS M1

Follow the instructions in Built AFL++ following the instructions in [AFL++'s
docs](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/INSTALL.md) to
build and install AFL++. Then, run

`make clean && make all`

to build both instrumented and uninstrumented binaries.

Run

`./run-afl-instrumented`

and

`./run-afl-uninstrumented`

to fuzz the instrumented and uninstrumented binaries respectively.

### Using an x86 Linux container

I also built and fuzzed the program in an x86 Linux VM running on my M1 Mac. To
do that, install Docker Desktop. Run

`./run-container`

to start the container.

`cd /src` to change into the directory with this repo from within the container.
Then, run

`make clean && make all`

to build both instrumented and uninstrumented binaries.

Run

`./run-afl-instrumented`

and

`./run-afl-uninstrumented`

to fuzz the instrumented and uninstrumented binaries respectively.

## Gotchas

Remember to `make clean` before running in Docker if you've built binaries on
the native platform, and vice-versa.

Install LLVM via brew as suggested in AFL++'s docs.

This part is important for getting rid of shm errors when running `sudo gmake install`:

```
sudo sysctl kern.sysv.shmmax=8388608
sudo sysctl kern.sysv.shmall=4096
```

## Fuzzing results

Fuzzing found all the bugs I inserted within minutes, and in some
configurations, in a few seconds, starting from a minimal non-crashing seed
input.

I fuzzed it using [AFL++](https://github.com/AFLplusplus/AFLplusplus) on my M1
MacBook Pro. Based on some cursory googling, it seemed like fuzzing on M1 macs
is a bit less supported and more complicated than on Linux x86, but I was able
to fuzz this program successfully in four different configurations:

| OS    | architecture | instrumented? | how I ran it                                                                                                                     | Performance (`exec speed` reported by AFL++) |
|-------|--------------|---------------|----------------------------------------------------------------------------------------------------------------------------------|----------------------------------------------|
| macOS | arm64        | yes           | Built AFL++ following the instructions in [AFL++'s docs](https://github.com/AFLplusplus/AFLplusplus/blob/stable/docs/INSTALL.md) | As expected, this was fastest: 3.6k/sec      |
| macOS | arm64        | no            | Using FRIDA mode (simply passing `-O` to afl-fuzz)                                                                               | 1700/sec                                     |
| Linux | x86          | yes           | Using Docker Desktop for mac, and AFL++'s official image                                                                         | 165/sec                                      |
| Linux | x86          | no            | Using Docker Desktop for mac, and AFL++'s official image; using FRIDA mode (simply passing `-O` to afl-fuzz)                                                                         | 110/sec                                      |
