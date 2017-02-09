#PBS -N placas_exec_scheduler
#PBS -l nodes=2:ppn=4
#PBS -M jf.ariza10@uniandes.edu.co
#PBS -m abe

module load rocks-openmpi_ib
cd $PBS_O_WORKDIR
NPROCS=`wc -l < $PBS_NODEFILE`
mpiexec -v -n $NPROCS ./mpi_placas
