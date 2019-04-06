compilar:
	mpicc -lm ./src/MaximoHipercubo.c -o ./src/MaximoHipercubo
	mpicc ./src/MinimoToroide.c -o ./src/MinimoToroide

MaximoHipercubo:
	(cd src && mpirun -n $(PROCESOS) MaximoHipercubo $(DIMENSIONES))

MinimoToroide:
	(cd src && mpirun -n $(PROCESOS) MinimoToroide $(LADO))

limpiar:
	rm ./src/MaximoHipercubo
	rm ./src/MinimoToroide
