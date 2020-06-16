import argparse
import random

# Default values.
d = 2
n = 10000
filename = "points.dat"

# Generate numbers inside a square of side length `sl`.
sl = 20

parser = argparse.ArgumentParser()
parser.add_argument("-d", "--dimension", help="the dimension of the point set to generate", type=int)
parser.add_argument("-n", "--size", help="the number of points to generate", type=int)
parser.add_argument("-o", "--output", help="the file to write the generated points")
args = parser.parse_args()
if args.dimension:
  d = args.dimension
if args.size:
  n = args.size
if args.output:
  filename = args.output

print("Writing {} {}-dimensional points to file {}...".format(n, d, filename))

with open(filename, "w") as f:
  f.write("{}\n".format(d))
  f.write("{}\n".format(n))
  f.flush()
  
  for i in range(n): 
    f.write("{} ".format(d))
    for j in range(d):
      coord = random.random() * sl
      if (j < d-1):
        f.write("{} ".format(coord))
      else:
        f.write("{}\n".format(coord))

