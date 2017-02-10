EXECS=mpi_placas
MPICC?=mpicc

placas.pdf: grafica.py datos.dat # grafica.py crea la imagen a partir de los datos en datos.dat
	python grafica.py
datos.dat: mpi_placas # mpi_placas genera los datos
	mpiexec -n 4 ./mpi_placas > datos.dat
mpi_placas: placas.c # compilo el programa
	${MPICC} -lm placas.c -o mpi_placas

cluster:
	${MPICC} -lm placas.c -o mpi_placas
	qsub submit_job.sh
clean:
	rm -f mpi_placas
