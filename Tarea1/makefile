EXECS=mpi_placas
MPICC?=mpicc

placas.pdf: grafica.py datos.dat # grafica.py crea la imagen a partir de los datos en datos.dat
	python grafica.py
datos.dat: aux_mpi_placas # aux_mpi_placas genera los datos
	mpiexec -n 4 ./aux_mpi_placas > datos.dat
aux_mpi_placas: aux_placas.c
	${MPICC} -lm aux_placas.c -o aux_mpi_placas
mpi_placas: placas.c
	${MPICC} -lm placas.c -o mpi_placas
	mpiexec -n 2 ./mpi_placas

cluster:
	${MPICC} -lm aux_placas.c -o aux_mpi_placas
	qsub submit_job.sh
clean:
	rm -f mpi_placas
	rm -f aux_mpi_placas
