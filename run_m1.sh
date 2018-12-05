#!/bin/sh
#PBS -l nodes=1:ppn=32:r641
#PBS -q mei
#PBS -l walltime=2:00:00
#PBS -N pcp_m1
#PBS -e /home/a77070/projects/pcp/PCP-TP2-MPI/logs/errors_m1.txt
#PBS -o /home/a77070/projects/pcp/PCP-TP2-MPI/logs/results_m1.txt

cd /home/a77070/projects/pcp/PCP-TP2-MPI

lscpu > tmp/xeon_cpu_info.txt

module load gcc/4.9.0
module load gnu/openmpi_eth/1.8.4

make clean

make

for size in 2048 16384 1048576 2097152
do
    echo ""
    echo "Sizeof input $size:"
    for i in {0..7}
    do
        mpirun -np 16 -mca btl self,sm,tcp bin/main $size
    done

done