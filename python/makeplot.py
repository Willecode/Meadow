from matplotlib import pyplot as plt
import pandas as pd

file = pd.read_csv('benchmarks.csv')

file.plot(0, [1,2,3,4,5])

plt.show()