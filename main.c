#include <stdio.h>
#include <mpi.h>
#include <time.h>
#include <stdlib.h>

#define N 0
#define M 100

int main(int argc, char **argv) {
    int rank, size, namelen, version, subversion, n_aleatorio, par;
    char processor_name[MPI_MAX_PROCESSOR_NAME];

    MPI_Init(&argc, &argv);    /* starts MPI */
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);    /* get current process id */
    MPI_Comm_size(MPI_COMM_WORLD, &size);    /* get number of processes */
    MPI_Group pares, impares, grupo_original;
    MPI_Comm_group(MPI_COMM_WORLD, &grupo_original);
    int * ranks_pares = (int *) malloc(size* sizeof(int));
    int * ranks_impares = (int *) malloc(size* sizeof(int));
    MPI_Get_processor_name(processor_name, &namelen);
    MPI_Get_version(&version, &subversion);
    printf( "Hello world from process %d of %d\n", rank, size );

    srand(time(NULL)+rank);
    n_aleatorio = M + rand() / (RAND_MAX / (N-M+1) +1 );


    if (n_aleatorio % 2 == 0) {
        par =1;
    }
    else {
        par = 0;
    }

    printf("proceso %d con número %d es par/impar: %d\n", rank, n_aleatorio, par);

    int buffer[size];
    MPI_Allgather(&par, 1, MPI_INT, buffer, 1, MPI_INT, MPI_COMM_WORLD);



        int i=0;
        int j=0;
        int k=0;
        int menor_par = M;
        int menor_impar=M;
        int menor_par_rank;
        int menor_impar_rank;
        for (i=0;i<size;i++){
            if (buffer[i]==1) { // proceso i es par
                ranks_pares[j]=i;
                if (buffer[i] < menor_par) {
                    menor_par = buffer[i];
                    menor_par_rank=i;
                }
                j++;
            }
            else {
                ranks_impares[k]=i;
                if (buffer[i] < menor_impar) {
                    menor_impar = buffer[i];
                    menor_impar_rank=i;
                }
                k++;

            }

        }



    MPI_Group_incl(grupo_original, j, ranks_pares, &pares);
    MPI_Group_incl(grupo_original, k, ranks_impares, &impares);

    int new_rank;

    if (par == 1) {
        MPI_Group_rank(pares, &new_rank);
    }
    else {
        MPI_Group_rank(impares, &new_rank);
    }
    char * grupo;
    if (par == 1) {
        grupo = "PAR";
    }
    else {
        grupo = "IMPAR";
    }
    if (rank == menor_par_rank || rank == menor_impar_rank) {
        printf("Soy el proceso de rango %d, pertenezco al grupo %s y tengo el valor %d que es el más bajo de dicho grupo\n",
               rank, grupo, n_aleatorio);
    }
        MPI_Finalize();
}