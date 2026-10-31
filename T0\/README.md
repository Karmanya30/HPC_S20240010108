
# HPC26/T0 - Performance Profiling and Optimization

## 1. Objective

To analyze and compare the performance of Bubble Sort and Quick Sort using Linux performance analysis tools such as GPROF and PERF.

## 2. Software Requirements

- Ubuntu Linux
- GCC Compiler
- GNU GPROF
- Linux PERF

## 3. Algorithms Implemented

1. Bubble Sort
2. Quick Sort

Input size: 18,250 random integers.

## 4. Compilation and Execution

### O0 - Without Optimization

```bash
gcc -O0 -pg mysort.c -o mysort
time ./mysort
gprof mysort gmon_O0.out > myreport.txt
perf stat ./mysort
```

### O2 - Standard Optimization

```bash
gcc -O2 -pg mysort.c -o mysort_O2
time ./mysort_O2
gprof mysort_O2 gmon_O2.out > report_O2.txt
perf stat ./mysort_O2
```

### O3 - Aggressive Optimization

```bash
gcc -O3 -pg mysort.c -o mysort_O3
time ./mysort_O3
gprof mysort_O3 gmon_O3.out > report_O3.txt
perf stat ./mysort_O3
```

## 5. Execution Time Results

| Algorithm | O0 (seconds) | O2 (seconds) | O3 (seconds) |
|---|---:|---:|---:|
| Bubble Sort | 1.314244 | 0.771482 | 0.792940 |
| Quick Sort | 0.003002 | 0.001006 | 0.001001 |

## 6. GPROF Analysis

### O0
- bubbleSort: 60.42% CPU time
- swap: 39.58% CPU time
- swap calls: 84,595,270

### O2
- bubbleSort: 100% CPU time

### O3
- bubbleSort: 100% CPU time

Bubble Sort was the major hotspot in all three profiling reports.

## 7. PERF Analysis

PERF successfully collected basic performance statistics.

However, hardware performance counters were unavailable in GitHub Codespaces.

Available metrics:
- Task clock
- Context switches
- CPU migrations
- Page faults
- Execution time

Unavailable metrics:
- CPU cycles
- Instructions
- Branches
- Branch misses

## 8. Performance Profiling Analysis

### 1. Which sorting algorithm performed better?

Quick Sort performed significantly better than Bubble Sort. Quick Sort has an average time complexity of O(n log n), whereas Bubble Sort has O(n²) complexity.

### 2. Which function consumed maximum execution time?

According to GPROF, bubbleSort was the major hotspot.

- O0: 60.42% CPU time.
- O2: 100% CPU time.
- O3: 100% CPU time.

The swap function also consumed 39.58% CPU time in the O0 profile.

### 3. How does execution time change with increasing input size?

Bubble Sort execution time increases approximately quadratically with input size. Quick Sort generally increases at O(n log n) average complexity.

### 4. How did compiler optimizations improve performance?

Compiler optimizations reduced execution time.

O2 and O3 improved the performance of both sorting algorithms compared with O0. However, O3 did not consistently outperform O2 in our measurements.

### 5. Which algorithm would you recommend for large datasets?

Quick Sort is recommended because its average time complexity is O(n log n), making it significantly more efficient than Bubble Sort for large datasets.

## 9. Conclusion

The experiment demonstrates that Quick Sort is significantly faster than Bubble Sort for the given input size.

Compiler optimizations using O2 and O3 improved execution performance compared with O0.

GPROF identified Bubble Sort as the primary performance bottleneck. PERF collected basic execution statistics, although hardware performance counters were restricted by the GitHub Codespaces environment.

## 10. Files

- mysort.c - C implementation of sorting algorithms.
- myreport.txt - GPROF report for O0.
- report_O2.txt - GPROF report for O2.
- report_O3.txt - GPROF report for O3.
- report.pdf - Complete experiment report.
- screenshots/ - Execution and profiling screenshots.
