#!/bin/sh

#SBATCH -J slurm
#SBATCH -p gpu,cpu -x gpu-0-2,compute-0-1
#SBATCH -N 1
#SBATCH --open-mode=append
#SBATCH -o %x.out
#SBATCH -e %x.error
#SBATCH --ntasks=1
#SBATCH --cpus-per-task=1
#SBATCH --mem=1gb 
#SBATCH --comment python
#SBATCH --time 03:00:00
#SBATCH --hint=compute_bound

era=$1
path=$2
name=$3

python run.py $era $path $name
