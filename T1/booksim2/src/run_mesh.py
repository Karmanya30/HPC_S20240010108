import subprocess
import re
import csv

config_file = "examples/torus88_transpose"
booksim = "./booksim"

rates = [
    0.02,
0.04,
0.06,
0.08,
0.10,
0.12,
0.14,
0.16,
0.18,
0.20,
]

# Save original configuration
with open(config_file, "r") as f:
    original = f.read()

results = []

try:

    for rate in rates:

        print("\n==============================")
        print("Injection rate:", rate)
        print("==============================")

        # Change injection rate
        new_config = re.sub(
            r"injection_rate\s*=\s*[^;]+;",
            f"injection_rate = {rate};",
            original
        )

        with open(config_file, "w") as f:
            f.write(new_config)

        # Run BookSim
        process = subprocess.run(
            [booksim, config_file],
            stdout=subprocess.PIPE,
            stderr=subprocess.PIPE,
            text=True
        )

        output = process.stdout + process.stderr

        # Find all packet latency values
        matches = re.findall(
            r"Packet latency average\s*=\s*([0-9.]+)",
            output
        )

        if matches:

            # Last value = final overall result
            latency = float(matches[-1])

            results.append((rate, latency))

            print("LATENCY:", latency)

        else:
            print("LATENCY NOT FOUND")

finally:

    # Restore original configuration
    with open(config_file, "w") as f:
        f.write(original)


# Save results
with open("torus_transpose_results.csv", "w", newline="") as f:

    writer = csv.writer(f)

    writer.writerow([
        "Injection Rate",
        "Packet Latency"
    ])

    for rate, latency in results:
        writer.writerow([
            rate,
            latency
        ])


print("\n==============================")
print("Simulation complete")
print("==============================")

print("Data points collected:", len(results))
print("Saved to: torus_transpose_results.csv")

print("\nResults:")

for rate, latency in results:
    print(f"{rate:.4f} -> {latency:.4f}")