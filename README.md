## An minimal example for playing with fuzzing

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

There may, of course, be other unintentional bugs in the code. I'm no C++
expert.

## Fuzzing

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
| Linux | x86          | no            | Using Docker Desktop for mac, and AFL++'s official image                                                                         | 110/sec                                      |
