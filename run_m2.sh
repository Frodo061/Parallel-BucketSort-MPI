#!/bin/sh
#PBS -l nodes=2:ppn=32:r641
#PBS -q mei
#PBS -l walltime=2:00:00
#PBS -N pcp_m2
#PBS -e /home/a77070/projects/pcp/PCP-TP2-MPI/logs/errors_m2.txt
#PBS -o /home/a77070/projects/pcp/PCP-TP2-MPI/logs/results_m2.txt

cd /home/a77070/projects/pcp/PCP-TP2-MPI

cat /proc/cpuinfo > tmp/cpuinfo.txt

module load gcc/4.9.0
module load gnu/openmpi_eth/1.8.4

options=('time' 'cc' 'wl')

make clean

make

for size in 2048 16384 1048576 2097152
do
    echo ""
    echo "Sizeof input $size:"
    
    for p in 3 5 9 16 32
    do
        echo ""
        echo "#buckets: $p-1"
        
        for opt in ${options[@]}
        do
        
            for i in {0..7}
            do
                mpirun -np $p -mca btl self,sm,tcp ./bin/main $size $opt
            done

        done
    
    done

done