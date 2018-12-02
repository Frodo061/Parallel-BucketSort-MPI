#!/bin/sh
#PBS -l nodes=2:ppn=32:r641
#PBS -q mei
#PBS -l walltime=2:00:00
#PBS -N pcp
#PBS -e /home/a77070/projects/pcp/PCP-TP2-MPI/logs/errors.txt
#PBS -o /home/a77070/projects/pcp/PCP-TP2-MPI/logs/results.txt

cd /home/a77070/projects/pcp/PCP-TP2-MPI

lscpu > tmp/xeon_cpu_info.txt