#!/bin/bash
#SBATCH --job-name mnist 
#SBATCH --output logs/%x-%j.log
#SBATCH --error logs/%x-%j.err
#SBATCH --nodes 1
#SBATCH --gpus tesla:1
date
singularity exec --nv pytorch_latest.sif python examples/mnist/main.py
date
