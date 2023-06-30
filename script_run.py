import os
import subprocess
import csv
import time

# Directory containing the .o files
directory = "/path/to/your/directory"

# Number of iterations
num_iterations = 1000

# Output CSV file
output_file = "results.csv"

# List to store results
results = []

# Iterate over .o files in the directory
for filename in os.listdir(directory):
    if filename.endswith(".o"):
        file_path = os.path.join(directory, filename)

        # Run the command for specified iterations
        start_time = time.time()
        for _ in range(num_iterations):
            subprocess.run(["./" + file_path], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        end_time = time.time()

        # Calculate average time
        average_time = (end_time - start_time) / num_iterations

        # Store filename and average time in results list
        results.append((filename, average_time))

# Write results to CSV file
with open(output_file, "w", newline="") as csvfile:
    writer = csv.writer(csvfile)
    writer.writerow(["Filename", "Average Time"])

    for result in results:
        writer.writerow(result)
