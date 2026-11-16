#include<stdio.h>
#include<conio.h>

#define MAX 5       
#define LARGO 30    


char minuscula(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}


int sonIguales(char a[], char b[]) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (minuscula(a[i]) != minuscula(b[i])) {
            return 0;
        }
        i++;
    }
   
    return a[i] == '\0' && b[i] == '\0';
}


void copiarTexto(char destino[], char origen[]) {
    int i = 0;
    while (origen[i] != '\0') {
        destino[i] = origen[i];
        i++;
    }
    destino[i] = '\0';
}


int buscarProducto(char nombres[][LARGO], int cantidad, char buscado[]) {
    int i;
    for (i = 0; i < cantidad; i++) {
        if (sonIguales(nombres[i], buscado)) {
            return i;
        }
    }
    return -1;
}


int ingresarProducto(char nombres[][LARGO], int demanda[], double tiempoU[],
                     double recursoU[], int cantidad) {
    char nombre[LARGO];

    if (cantidad == MAX) {
        printf("Ya registro los %d productos permitidos.\n", MAX);
        return cantidad;
    }

    printf("Nombre del producto: ");
    scanf(" %29[^\n]", nombre);   

    if (buscarProducto(nombres, cantidad, nombre) != -1) {
        printf("Ese producto ya existe.\n");
        return cantidad;
    }

    copiarTexto(nombres[cantidad], nombre);

    do {
        printf("Cantidad demandada (unidades): ");
        scanf("%d", &demanda[cantidad]);
    } while (demanda[cantidad] < 0);

    do {
        printf("Tiempo de fabricacion por unidad (horas): ");
        scanf("%lf", &tiempoU[cantidad]);
    } while (tiempoU[cantidad] <= 0);

    do {
        printf("Recursos necesarios por unidad: ");
        scanf("%lf", &recursoU[cantidad]);
    } while (recursoU[cantidad] <= 0);

    printf("Producto registrado.\n");
    return cantidad + 1;
}


void listarProductos(char nombres[][LARGO], int demanda[], double tiempoU[],
                     double recursoU[], int cantidad) {
    int i;
    if (cantidad == 0) {
        printf("No hay productos registrados.\n");
        return;
    }
    printf("\n%-3s %-20s %8s %10s %10s\n", "#", "Producto", "Demanda", "Horas/u", "Recurso/u");
    for (i = 0; i < cantidad; i++) {
        printf("%-3d %-20s %8d %10.2f %10.2f\n",
               i + 1, nombres[i], demanda[i], tiempoU[i], recursoU[i]);
    }
}


void editarProducto(char nombres[][LARGO], int demanda[], double tiempoU[],
                    double recursoU[], int cantidad) {
    char buscado[LARGO];
    int pos, opcion;

    if (cantidad == 0) {
        printf("No hay productos para editar.\n");
        return;
    }

    printf("Nombre del producto a editar: ");
    scanf(" %29[^\n]", buscado);
    pos = buscarProducto(nombres, cantidad, buscado);

    if (pos == -1) {
        printf("Producto no encontrado.\n");
        return;
    }

    printf("Que desea editar de '%s'?\n", nombres[pos]);
    printf("1. Nombre\n2. Cantidad demandada\n3. Tiempo por unidad\n4. Recursos por unidad\n");
    printf("Opcion: ");
    scanf("%d", &opcion);

    if (opcion == 1) {
        printf("Nuevo nombre: ");
        scanf(" %29[^\n]", nombres[pos]);
    } else if (opcion == 2) {
        do {
            printf("Nueva cantidad demandada: ");
            scanf("%d", &demanda[pos]);
        } while (demanda[pos] < 0);
    } else if (opcion == 3) {
        do {
            printf("Nuevo tiempo por unidad (horas): ");
            scanf("%lf", &tiempoU[pos]);
        } while (tiempoU[pos] <= 0);
    } else if (opcion == 4) {
        do {
            printf("Nuevos recursos por unidad: ");
            scanf("%lf", &recursoU[pos]);
        } while (recursoU[pos] <= 0);
    } else {
        printf("Opcion invalida.\n");
        return;
    }
    printf("Producto actualizado.\n");
}

int eliminarProducto(char nombres[][LARGO], int demanda[], double tiempoU[],
                     double recursoU[], int cantidad) {
    char buscado[LARGO];
    int pos, i;

    if (cantidad == 0) {
        printf("No hay productos para eliminar.\n");
        return cantidad;
    }

    printf("Nombre del producto a eliminar: ");
    scanf(" %29[^\n]", buscado);
    pos = buscarProducto(nombres, cantidad, buscado);

    if (pos == -1) {
        printf("Producto no encontrado.\n");
        return cantidad;
    }

    for (i = pos; i < cantidad - 1; i++) {
        copiarTexto(nombres[i], nombres[i + 1]);
        demanda[i] = demanda[i + 1];
        tiempoU[i] = tiempoU[i + 1];
        recursoU[i] = recursoU[i + 1];
    }

    printf("Producto eliminado.\n");
    return cantidad - 1;
}

void calcular(char nombres[][LARGO], int demanda[], double tiempoU[],
              double recursoU[], int cantidad,
              double tiempoDisp, double recursosDisp) {
    int i;
    double tiempoTotal = 0, recursoTotal = 0;
    double tiempoProd, recursoProd;

    if (cantidad == 0) {
        printf("No hay productos registrados.\n");
        return;
    }

    printf("\n%-20s %12s %12s\n", "Producto", "Tiempo (h)", "Recursos");
    for (i = 0; i < cantidad; i++) {
        tiempoProd = demanda[i] * tiempoU[i];
        recursoProd = demanda[i] * recursoU[i];
        tiempoTotal = tiempoTotal + tiempoProd;
        recursoTotal = recursoTotal + recursoProd;
        printf("%-20s %12.2f %12.2f\n", nombres[i], tiempoProd, recursoProd);
    }

    printf("\nTiempo total requerido:   %.2f h (disponible: %.2f h)\n", tiempoTotal, tiempoDisp);
    printf("Recursos totales:         %.2f (disponibles: %.2f)\n", recursoTotal, recursosDisp);

    if (tiempoTotal <= tiempoDisp && recursoTotal <= recursosDisp) {
        printf("\nSI se puede cumplir con la demanda.\n");
    } else {
        printf("\nNO se puede cumplir con la demanda.\n");
        if (tiempoTotal > tiempoDisp) {
            printf("Faltan %.2f horas.\n", tiempoTotal - tiempoDisp);
        }
        if (recursoTotal > recursosDisp) {
            printf("Faltan %.2f unidades de recursos.\n", recursoTotal - recursosDisp);
        }
    }
}

int main(int argc, char const *argv[])
{
    char nombres[MAX][LARGO];   
    int demanda[MAX];           
    double tiempoU[MAX];        
    double recursoU[MAX];       
    int cantidad = 0;           
    int opcion;
    double tiempoDisp = 0, recursosDisp = 0;

    do {
        printf("\n===== PRODUCCION DE COMPONENTES ELECTRONICOS =====\n");
        printf("1. Ingresar producto\n");
        printf("2. Listar productos\n");
        printf("3. Editar producto\n");
        printf("4. Eliminar producto\n");
        printf("5. Ingresar tiempo y recursos disponibles\n");
        printf("6. Calcular y verificar demanda\n");
        printf("0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        if (opcion == 1) {
            cantidad = ingresarProducto(nombres, demanda, tiempoU, recursoU, cantidad);
        } else if (opcion == 2) {
            listarProductos(nombres, demanda, tiempoU, recursoU, cantidad);
        } else if (opcion == 3) {
            editarProducto(nombres, demanda, tiempoU, recursoU, cantidad);
        } else if (opcion == 4) {
            cantidad = eliminarProducto(nombres, demanda, tiempoU, recursoU, cantidad);
        } else if (opcion == 5) {
            do {
                printf("Tiempo de produccion disponible (horas): ");
                scanf("%lf", &tiempoDisp);
            } while (tiempoDisp <= 0);
            do {
                printf("Recursos disponibles: ");
                scanf("%lf", &recursosDisp);
            } while (recursosDisp <= 0);
        } else if (opcion == 6) {
            if (tiempoDisp <= 0 || recursosDisp <= 0) {
                printf("Primero ingrese el tiempo y recursos disponibles (opcion 5).\n");
            } else {
                calcular(nombres, demanda, tiempoU, recursoU, cantidad, tiempoDisp, recursosDisp);
            }
        } else if (opcion != 0) {
            printf("Opcion invalida.\n");
        }
    } while (opcion != 0);

    printf("Hasta luego.\n");
    return 0;
}

