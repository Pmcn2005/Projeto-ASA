import time
import subprocess
import matplotlib.pyplot as plt
import numpy as np

# Path to the executable
executable = "./proj2"


# Create a list to store the execution times and the values of f(n,m,l)
times = []
f_nml = []

# Teste the speed of the executable with all the tests
for i in range(0, 50):
   with open(f"tests/test_{i:02d}.in", "r") as f:
      line = f.readline()
      line = line.split()
      n = int(line[0])
      m = int(line[1])
      l = int(line[2])
      f_nml.append(l*m+(l*n)*l)

      f.close()

   start = time.time()
   
   # Run the executable with the input file
   with open(f"tests/test_{i:02d}.in", "r") as input_file:
      subprocess.run([executable], stdin=input_file, stdout=subprocess.DEVNULL)
   
   end = time.time()

   # Calculate the execution time
   execution_time = end - start
   
   times.append(execution_time)

   print(f"Teste {i:02d} concluído. Tempo de execução: {execution_time:.4f} segundos")
   

# Plot all the data
# plt.scatter(f_nml, times, label="Dados experimentais", alpha=0.5, color="blue")

# Ajust a curve of degree 2
degree = 2
coef = np.polyfit(f_nml, times, degree)
poly_fn = np.poly1d(coef)

# Plot the curve
sorted_nml_values = sorted(f_nml)
plt.plot(sorted_nml_values, poly_fn(sorted_nml_values), '-', label="Tendência global", color="red")

plt.xlabel("f(n,m,l) = l*n^2 + l*m")
plt.ylabel("Time(s)")
plt.title("Curva de tendência para tempo de execução em função de f(n,m,l)")
plt.legend()
plt.show()
