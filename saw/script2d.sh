#!/bin/bash
#SBATCH -J saw2d                            # job name
#SBATCH -N 1                                 # number of nodes
#SBATCH -n 1                                   # number of MPI processes, here 16 per node
#SBATCH --partition=compute         # choose nodes from partition
#SBATCH -o %j.out                            # stdout file name (%j: job ID)
#SBATCH -e %j.err                             # stderr file name (%j: job ID)
#SBATCH -t 72:00:00                        # max run time (hh:mm:ss), max 72h!
#SBATCH --mail-type=END
#SBATCH --mail-user=s-jamiha@uni-greifswald.de


module load gcc
module load boost

g++ -std=c++17 -O3 saw.cpp -o walk.run

for i in 1000 2000 4000 8000 16000 32000 64000 128000
do
  ./walk.run 2 $i 1000000 2 
done