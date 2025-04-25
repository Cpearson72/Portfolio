from matplotlib import pyplot as plt


# --- Data for Dynamic Programming ---
# Data points from runtime_log_dp.txt (excluding the incomplete entry)
dp_pixels = [1100, 1665, 40000, 80000, 160000, 640000]
dp_times = [0.0058, 0.0066, 0.2471, 0.5647, 1.2035, 6.2750]

plt.figure(figsize=(8, 6))
plt.plot(dp_pixels, dp_times, marker='o', linestyle='-', color='blue')
plt.title("Seam Carving Runtime (Dynamic Programming)")
plt.xlabel("Number of Pixels")
plt.ylabel("Elapsed Time (seconds)")
plt.grid(True)
plt.tight_layout()
plt.show()


# --- Data for Naive Approach ---
naive_pixels = [25, 49, 64, 72, 100, 121, 169, 225]
naive_times = [0.0000, 0.0061, 0.0111, 0.0150, 0.1399, 0.4242, 4.8588, 48.9450]

plt.figure(figsize=(8, 6))
plt.plot(naive_pixels, naive_times, marker='o', linestyle='-', color='orange')
plt.title("Seam Carving Runtime (Naive Approach)")
plt.xlabel("Number of Pixels")
plt.ylabel("Elapsed Time (seconds)")
plt.grid(True)
plt.tight_layout()
plt.show()
