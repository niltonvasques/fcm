import time

import numpy as np
import matplotlib.pyplot as plt

from sklearn import cluster, datasets
#from sklearn.neighbors import kneighbors_graph
#from sklearn.preprocessing import StandardScaler

np.random.seed(0)

# Generate datasets. We choose the size big enough to see the scalability
# of the algorithms, but not too big to avoid too long running times
n_samples = 1500
noisy_circles = datasets.make_circles(n_samples=n_samples, factor=.5,
                                              noise=.05)
noisy_moons = datasets.make_moons(n_samples=n_samples, noise=.05)
blobs = datasets.make_blobs(n_samples=n_samples, random_state=8)
no_structure = np.random.rand(n_samples, 2), None

#print(blobs[0][1])
#print(blobs[0])
#print(blobs[1])
print n_samples, ' ', 2, 2
print 2.0, ' ', 0.0001 
for x in noisy_moons[0]:
    print x[0], ' ', x[1]

#print
#print('COLUMN 2')
#print
#
#for x in blobs[0]:
#    print(x[1])
#    #print(x[0], [1])


