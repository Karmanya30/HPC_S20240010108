import csv
import matplotlib.pyplot as plt

rates = []
latencies = []

with open("mesh_transpose_results.csv", "r") as f:
    reader = csv.DictReader(f)

    for row in reader:
        rates.append(float(row["Injection Rate"]))
        latencies.append(float(row["Packet Latency"]))

plt.figure(figsize=(8, 5))

plt.plot(
    rates,
    latencies,
    marker="o"
)

plt.xlabel("Injection Rate (Offered Load)")
plt.ylabel("Average Packet Latency (cycles)")
plt.title("8×8 Mesh_transpose: Latency vs Injection Rate")

plt.grid(True)
plt.tight_layout()

plt.savefig(
    "mesh_transpose_latency_vs_injection_rate.png",
    dpi=300
)

plt.show()
