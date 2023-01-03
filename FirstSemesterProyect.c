#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int countstring(char *);
void altas_devendedores();
void consulta_de_vendedores_por_clave();
void listado_de_vendedores();
void altas_de_productos();
void consulta_de_productos_por_clave();
void listado_de_productos();
void registro_de_ventas();
void lista_de_ventas_por_producto();
void lista_de_ventas_por_vendedor();

struct Producto{
    char clave[6];
	char nombre[41];
	
}p;

struct Vendedor{
    char clave[6];
	char nombre[41];
	char celular[11];
	char correo[41];
}vendedor;

struct Ventas{
	char cve_vendedor[6];
	char cve_producto[6];
	int ventas;
}venta;

int guardar_vendedor(struct Vendedor *v)
{
	FILE *pFile;
		
	pFile= fopen("vendedores.txt", "a+");

	if (pFile ==NULL){
		return 1;
	}
	fwrite(v, sizeof(struct Vendedor), 1, pFile);
	fclose(pFile);
	return 0;
}


void listado_de_vendedores()
{
	FILE *pFile, *pFile2;
	pFile= fopen("vendedores.txt", "r"); 
	pFile2= fopen("repvendedores.txt", "w");      //pFile2 se abre para poder hacer un registro de lo que lleva acabo en el listado de vendedores.

	struct Vendedor v;
	
	if (pFile == NULL){
		printf("\n Archivo no Encontrado. \n");
		return;
	}
	
	
	printf("\n Clave \t Nombre \t\t\t Celular \t\t\t Correo \n\n");
	

	while(fread(&v, sizeof(struct Vendedor), 1, pFile))
	{
		fwrite(&v, sizeof(struct Vendedor), 1, pFile2);

		
		printf(" %s \t %s \t\t\t %s \t\t\t %s \n", 
		v.clave,
		v.nombre,
		v.celular,
		v.correo );
	}
	
	fclose(pFile);
	fclose(pFile2);
}

int guardar_producto(struct Producto *p)
{
	FILE *pFile;
		
	pFile= fopen("productos.txt", "a+");

	if (pFile ==NULL){
		return 1;
	}
	fwrite(p, sizeof(struct Producto), 1, pFile);
	fclose(pFile);
	return 0;
}

int guardar_venta(struct Ventas *v)
{
	FILE *pFile;
		
	pFile= fopen("ventas.txt", "a+");

	if (pFile ==NULL){
		return 1;
	}
	fwrite(v, sizeof(struct Ventas), 1, pFile);
	fclose(pFile);
	return 0;	
}

void listado_de_productos()
{
	FILE *pFile, *pFile2;
	pFile= fopen("productos.txt", "r"); 
	pFile2= fopen("repproductos.txt", "w");      //pFile2 se abre para poder hacer un registro de lo que lleva acabo en el listado de productos.

	struct Producto p;
	
	if (pFile == NULL){
		printf("\n Archivo no Encontrado. \n");
		return;
	}
	
	
	printf("\n Clave \t Nombre \n\n");
	

	while(fread(&p, sizeof(struct Producto), 1, pFile))
	{
		fwrite(&p, sizeof(struct Producto), 1, pFile2);

		
		printf(" %s \t %s \n\n", 
		p.clave,
		p.nombre );
	}
	
	fclose(pFile);
	fclose(pFile2);
}


/* Busca vendores por clave, y guarda la informacion en el parametro vendedor
*    en caso de encontrar alguna coincidencia.
 * Parametros: Puntero a clave, y puntero a vendedor
 * Retorno:
 *    - 0 En caso de exito
 *    - 1 Si no existe el archivo
 *    - 2 En caso de no encontrar vendedor
*/


int buscar_vendedor(char *clave, struct Vendedor *v)  //Funcion utilizada para buscar al vendedor necesitado dentro del archivo.
{
	FILE *pFile;
	pFile= fopen("vendedores.txt", "r");

	if (pFile == NULL){
		return 1;
	}
	while(fread(v, sizeof(struct Vendedor), 1, pFile))   //buble que leera todo el .txt
	{
		if(strcmp(v->clave, clave) == 0){                //condicionante que verifica si la clave mandada es igual a cualquier clave guardada con el struct.
			return 0;
		}
	}
	
	fclose(pFile);
	return 2;
}



int buscar_producto(char *clave, struct Producto *p)  //Funcion utilizada para buscar rl producto necesitado dentro del archivo.
{
	FILE *pFile;
	pFile= fopen("productos.txt", "r");

	if (pFile == NULL){
		return 1;
	}
	while(fread(p, sizeof(struct Producto), 1, pFile))   //buble que leera todo el .txt
	{
		if(strcmp(p->clave, clave) == 0){                //condicionante que verifica si la clave mandada es igual a cualquier clave guardada con el struct.
			return 0;
		}
	}
	
	fclose(pFile);
	return 2;
}

int buscar_ventas(struct Ventas *v)
{
	FILE *pFile;
	pFile= fopen("ventas.txt", "r");
	struct Ventas venta;

	if (pFile == NULL){
		return 1;
	}
	while(fread(&venta, sizeof(struct Ventas), 1, pFile))   //buble que leera todo el .txt
	{
		if( strcmp(venta.cve_producto, v->cve_producto) == 0 
		&& strcmp(venta.cve_vendedor, v->cve_vendedor) == 0) {                //condicionante que verifica si la clave mandada es igual a cualquier clave guardada con el struct.
			v->ventas = venta.ventas;
			return 0;
		}
	}
	
	fclose(pFile);
	return 2;
}

void lista_de_ventas_por_producto(){
	char buff[128];
	int sum = 0;
	struct Ventas venta;
	FILE *pFile;
	struct Vendedor vendedor;

	printf("Ingrese la clave del producto: ");
	scanf("%s", buff);

	pFile= fopen("ventas.txt", "r");

	if (pFile == NULL){
		return 1;
	}
	while(fread(&venta, sizeof(struct Ventas), 1, pFile))   //buble que leera todo el .txt
	{
		if( strcmp(buff, venta.cve_producto) == 0 ){
			if( buscar_vendedor(venta.cve_vendedor, &vendedor) > 0){
				//Esto NUNCA deberia de pasar.
				printf("Error, la venta registrada no tiene una clave de vendedor valida\n Reinicie la base de datos de ventas y vendedores\n");
				fflush(stdin);
				printf("Presione para continuar");
				getch();
				return;
			}
			
			printf("%s  %s  %i\n", vendedor.clave, vendedor.nombre, venta.ventas);
			sum += venta.ventas;
		}
	}
	printf("Total de ventas = %i\n", sum);
	fclose(pFile);
	return 2;
	
	
}


void lista_de_ventas_por_vendedor(){
	char buff[128];
	int sum = 0;
	struct Ventas venta;
	FILE *pFile;
	struct Vendedor vendedor;
	struct Producto producto;
	
	printf("Ingrese la clave del vendedor: ");
	scanf("%s", buff);

	pFile= fopen("ventas.txt", "r");

	if (pFile == NULL){
		return 1;
	}
	while(fread(&venta, sizeof(struct Ventas), 1, pFile))   //buble que leera todo el .txt
	{
		if( strcmp(buff, venta.cve_vendedor) == 0 ){
			if( buscar_producto( venta.cve_producto, &producto) > 0){
				//Esto NUNCA deberia de pasar.
				//printf("Error, la venta registrada no tiene una clave de vendedor valida\n Reinicie la base de datos de ventas y vendedores\n");
				fflush(stdin);
				printf("Presione para continuar");
				getch();
				return;
			}
			
			printf("%s  %s  %i\n",
				producto.clave,
				producto.nombre,
				venta.ventas
			 //vendedor.clave, vendedor.nombre, venta.ventas
			);
			sum += venta.ventas;
		}
	}
	printf("Total de ventas = %i\n", sum);
	fclose(pFile);
	return 2;
	
	
}


int countstring(char *s)                     //funcion utilizada para delimitar la entrada de digitos, ya que fgets detecta el caracter 10(ENTER) de la tabla de ascii y con esta funcion delimita para que no detecte el ENTER al introducir la clave
{
	int n = 0;
	int i=0;
	while(1){
		if(i++ > 10000) return 10000;
		if(s[n] == 10)
			return n;
		if(s[n] == 0)
			return n;
		n++;
	}
	return n;
}


int clean_end_line(char *s)                     //funcion utilizada para delimitar la entrada de digitos, ya que fgets detecta el caracter 10(ENTER) de la tabla de ascii y con esta funcion delimita para que no detecte el ENTER al introducir la clave
{
	int n = 0;
	int i=0;
	while(1){
		if(i++ > 10000) return 10000;
		if(s[n] == '\n'){
			s[n] = 0;
			return;
		}
		if(s[n] == 0)
			return;
			
		n++;
	}
	return n;
}


int main(void){
	struct Producto p;
	int a, result;
	
/*	result = buscar_vendedor("12346", &v);
	if(result)
	{
		printf("error en el archivo");
	}else{
		printf("Nombre : %s\n", v.nombre);
		printf("Correo : %s\n", v.correo);
		
	}
	
	getch();
	return 0;
*/

	do{
	 
	system("cls");   
	printf("\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n");
	printf("\t\t\t\t\t\t");
	printf("Programa hecho por Leonardo Ciscomani y Javier Alarcon\n");

	printf("\n");
	printf("\t\t\t\t\t\t");
	printf("=======================================================================================");
	printf("\n\n\t\t\t\t\t\tMenu de Opciones:\n\n");
	printf("\t\t\t\t\t\t");
	printf("1. Altas de Vendedores                         6. Listado de Productos\n");
	printf("\t\t\t\t\t\t");
    printf("2. Consulta de Vendedores por Clave            7. Registro de Ventas\n");
    printf("\t\t\t\t\t\t");
    printf("3. Listado de Vendedores                       8. Lista de Ventas por Producto\n");
    printf("\t\t\t\t\t\t");
    printf("4. Altas de Productos                          9. Lista de Ventas por Vendedor\n");
    printf("\t\t\t\t\t\t");
    printf("5. Consulta de Productos por Clave            10. Introduce para Poder Salir del Menu\n");
    printf("\n");
	printf("\t\t\t\t\t\t");
	printf("=======================================================================================");
	
	printf("\n");
	printf("\n\t\t\t\t\t\tIntroducir el numero de la opcion que desea:\n");
	printf("\t\t\t\t\t\tOpcion: ");
	scanf("%d", &a);
	
	fflush(stdin);

	
	switch (a){
	
		
		case 1:
		system("cls");
			printf("\n");
			printf("\t\t\t\t");
			printf("Altas de Vendedores\n");
			printf("=======================================================================================");
			printf("\n");
			altas_devendedores();
			
			break;
		
		
		case 2:
		system("cls");
		printf("\n");
			printf("\t\t\t");
			printf("Consulta de Vendedores por Clave\n");
			printf("=======================================================================================");
			printf("\n");
	        consulta_de_vendedores_por_clave();
			break;
		
		
		case 3:
		system("cls");
		printf("\n");
			printf("\t\t\t\t");
			printf("Listado de Vendedores\n");
			printf("=======================================================================================");
			printf("\n");
			listado_de_vendedores();
			break;
		
		 
		case 4:
			
		system("cls");
		printf("\n");
			printf("\t\t\t\t");
			printf("Altas de Productos\n");
			printf("=======================================================================================");
			printf("\n");
			altas_de_productos();
			
			break;
		
		
		case 5:
		system("cls");
		printf("\n");
			printf("\t\t\t");
			printf("Consulta de Productos por Clave\n");
			printf("=======================================================================================");
			printf("\n");
			consulta_de_productos_por_clave();
			
			break;
		
			
		case 6:
      	system("cls");
      	printf("\n");
			printf("\t\t\t\t");
			printf("Listado de Productos\n");
			printf("=======================================================================================");
			printf("\n");
			listado_de_productos();
			
			break;
		
	
		case 7:
		system("cls");
	printf("\n");
			printf("\t\t\t\t");
			printf(" Registro de Ventas\n");
			printf("=======================================================================================");
			printf("\n");
			registro_de_ventas();
			break;
		
		
		case 8:
		system("cls");
		printf("\n");
			printf("\t\t\t\t");
			printf("Lista de Ventas por Producto\n");
			printf("=======================================================================================");
			printf("\n");
			lista_de_ventas_por_producto();
			break;
		
	
		case 9:
		system("cls");
		printf("\n");
			printf("\t\t\t\t");
			printf("Lista de Ventas por Vendedor\n");
			printf("=======================================================================================");
			printf("\n");
			lista_de_ventas_por_vendedor();
			break;
		
		case 10:
		system("cls");
		printf("\n");
		    printf("Pulse ESC para Salir");
		    break;
	
	    default:
	    system("cls");
	    	printf("Lo que Usted Ingreso No es un Numero del Menu\n");
	    	break;
	}
	printf("\n\nPulse Cualquier Tecla Para volver al Menu \n");
	printf("\n\nPulse ESC para Salir del Programa\n\n");
     
} while( getch() !=27 );
	
	

	return 0;
}

void altas_devendedores(){
	char clave[6], nombre[41], celular[11], correo[41];
	char rpt;
	int i, d, leido;
	char ch, *ret;
	struct Vendedor v;
		
	do{
		fflush(stdin);
		int erri;
		int error;
		do{
			error = 0;
			printf("\n Clave: \n");
			ret = fgets(clave, 6, stdin);
			fflush(stdin);
			for( i=0; i<countstring(clave); i++){                          // for utilizado para revisar caracter por caracter de la clave.
				if(clave[i] < 48 || clave[i] > 57){                        // condicion la cual hace que cualquier caracter fuera del rango 48 al 57 de la tabla ascii osea los numeros del 0 al 9 marcara error.
					printf("Error, porfavor introduzca solo digitos..\n");  
					//printf("El digito numero %i es %c\n", i, clave[i]);
					error = 1;
				}
			}
			if(error == 0 && countstring(clave)<5){
				printf("Error, introduzca 5 digitos...\n ");
				error = 1;
			}
			if(error == 0 && buscar_vendedor(clave, &v) == 0 ){
				printf("Error, clave repetida");
				error = 1;
			}
			
        }while(error);

			fflush(stdin);
			printf("\n Nombre: \n");
			scanf("%s", nombre); 	
			fflush(stdin);
			//fgets(nombre,41,stdin); fflush(stdin);
			//countstring(nombre);
		
		
		do{
				erri=0;
			    printf("\n Celular a 10 digitos: \n"); 
		    	fgets(celular,11,stdin); 
			    fflush(stdin);
			    for( i=0; i<countstring(celular); i++){                          // for utilizado para revisar caracter por caracter del celular.
				if(celular[i] < 48 || celular[i] > 57){                        // condicion la cual hace que cualquier caracter fuera del rango 48 al 57 de la tabla ascii osea los numeros del 0 al 9 marcara error.
					printf("Error, porfavor introduzca solo digitos..\n"); 
					error = 1;
					break; 
				}
			}
				if(erri == 0 && countstring(celular)<10){
				printf("Error, introduzca 10 digitos...\n");
				erri=1;
			     }
		}while(erri);
		
		
			fflush(stdin);
			printf("\n Correo: \n"); 
			fgets(correo,41,stdin); 
			fflush(stdin);

			//Ya tenemos los datos del vendedor
			strcpy(v.clave, clave);                                          //strcpy es una funcion utilizada para copiar y pegar la informacion previamente digitada y guardarla en las string declarados dentro del struct.
			strcpy(v.nombre, nombre);
			strcpy(v.celular, celular);
			strcpy(v.correo, correo);
			guardar_vendedor(&v);
		printf("\n Si Desea Agregar Mas Vendedores Pulse: [s]  \n");
	
		scanf("%c", &rpt);
	}while(rpt=='s');
	
}



void consulta_de_vendedores_por_clave(){
	char clave[6];
	struct Vendedor v;
	printf("Introduzca 5 digitos porfavor...\n");
	printf("Dame la clave del vendedor: ");	
	fflush(stdin);
	fgets(clave,  6, stdin);
	
	int result;
	
	result = buscar_vendedor(clave, &v);
	if(result == 0 )
	{
		printf("Clave || Nombre      || Celular || Correo \n\n");
		printf(" %s | %s | %s | %s \n", 
		v.clave,
		v.nombre,
		v.celular,
		v.correo );
		
		
		
		//printf("%s  ", v.nombre);
		//Despliego la informacion
	}
	if(result == 1 )
	{
		printf("No hay ningun vendedor almacenado en el sistema aun.");
	}

	if(result == 2 )
	{
	
		printf("La clave del vendedor no existe\n");
	}
	
}

void altas_de_productos(){
	char clave[6], nombre[41];
	char rpt;
	int i, d, leido;
	char ch, *ret;
	struct Producto p;
		
	do{
		fflush(stdin);
		int erri;
		int error;
		do{
			error = 0;
			printf("\n Clave: \n");
			ret = fgets(clave, 6, stdin);
			fflush(stdin);
			for( i=0; i<countstring(clave); i++){                                                             // for utilizado para revisar caracter por caracter de la clave.
				if(clave[i] < 48 || clave[i] > 57 && clave[i] < 65 || clave[i] > 90){                        // condicion la cual hace que cualquier caracter fuera del rango 48 al 57 de la tabla ascii osea los numeros del 0 al 9 marcara error.
					printf("Error, porfavor introduzca digitos o letras Mayusculas..\n");                     // en esta condicion se le agrego otro rango de opciones los cuales perimten introducir letras mayusculas a la clave.
					//printf("El digito numero %i es %c\n", i, clave[i]);                                      
					error = 1;
					break; 
				}
			}
			if(error == 0 && countstring(clave)<5){
				printf("Error, introduzca 5 digitos...\n ");
				error = 1;
			}
			if(error == 0 && buscar_producto(clave, &p) == 0 ){
				printf("Error, clave repetida");
				error = 1;
			}
			
        }while(error);

			fflush(stdin);
			printf("\n Nombre: \n");
			//scanf("%s", nombre); 	
			fflush(stdin);
			scanf("%40[^\n]s", nombre);
			//fgets(nombre,41,stdin); fflush(stdin);
			//countstring(nombre);
		

			//Ya tenemos los datos del vendedor
			strcpy(p.clave, clave);                                          //strcpy es una funcion utilizada para copiar y pegar la informacion previamente digitada y guardarla en las string declarados dentro del struct.
			strcpy(p.nombre, nombre);

			guardar_producto(&p);
		printf("\n Si Desea Agregar Mas Productos Pulse: [s]  \n");
	
		scanf("%c", &rpt);
	}while(rpt=='s');
	
}



void consulta_de_productos_por_clave(){
	char clave[6];
	struct Producto p;
	printf("Introduzca 5 digitos porfavor...\n");
	printf("Dame la clave del producto: ");	
	fflush(stdin);
	fgets(clave,  6, stdin);
	
	int result;
	
	result = buscar_producto(clave, &p);
	if(result == 0 )
	{
		printf("Clave || Nombre       \n\n");
		printf(" %s | %s \n", 
		p.clave,
		p.nombre );
		
		
		
		//printf("%s  ", v.nombre);
		//Despliego la informacion
	}
	if(result == 1 )
	{
		printf("No hay ningun Producto almacenado en el sistema aun.");
	}

	if(result == 2 )
	{
	
		printf("La clave del Producto no existe\n");
	}
	
}

//Funcion 7
void registro_de_ventas()
{
	struct Vendedor v;
	struct Producto p;
	struct Ventas venta;
	int res, error = 1;
	
	
	char buff[128];
	while(error){
		error = 0;
		printf("Clave vendedor :");
		scanf("%5s", buff);
		res = buscar_vendedor(buff, &v);
		if(res){
			printf("No se encontro el vendedor\n");
			error = 1;
			continue;
		}
		printf("Vendedor seleccionado : %s\n", v.nombre);
		
		printf("Clave producto :");
		scanf("%5s", buff);
		res = buscar_producto(buff, &p);
		
		if(res){
			printf("No se encontro el producto\n");
			error = 1;
			continue;
		}
		printf("Producto seleccionado : %s\n", p.nombre );
		
		printf("Ingrese la cantidad de ventas: ");
		scanf("%i", &venta.ventas);
		
		strcpy(venta.cve_vendedor, v.clave);
		strcpy(venta.cve_producto, p.clave);
		
		if(buscar_ventas(&venta) == 0){
			printf("Ya existe la venta registrada\n");
			error = 1;
			continue;
		}
	}
	guardar_venta(&venta);
	
	printf("Se guardo la venta, presiona para continuar");
	fflush(stdin);
	getch();
	return;
}




