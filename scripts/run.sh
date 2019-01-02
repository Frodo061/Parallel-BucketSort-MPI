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

mappings=("--map-by core" "--map-by node" "--map-by socket")
options=('time' 'cc' 'wl')

make clean

make

for mapping in "${mappings[@]}"
do
    echo ""
    echo "mapping: $mapping"
    
    for p in 2 4 8 16 32
    do
        echo ""
        echo "#buckets: $p"

        for size in 2048 16384 1048576 2097152
        do
            echo ""
            echo "Sizeof input $size:"

            for opt in ${options[@]}
            do

                for i in {0..7}
                do
                    if [ "$p" -gt 16 && "$mapping" == "--map-by core" ]; then
                        mpirun -np $p ppr:1:core -report-bindings -mca btl self,sm,tcp ./bin/main $size $opt
                    else
                        mpirun -np $p $mapping -report-bindings -mca btl self,sm,tcp ./bin/main $size $opt
                    fi
                done

            done

        done

    done

done