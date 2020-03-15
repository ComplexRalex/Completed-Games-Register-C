#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#define N 100
#define L 150
#define E 500

typedef struct ELEMENTO{
	char nombre[N], link[L], opinion[E], spoiler[E], nota[E], platforms[N];
	int year, percentage;
}elemento;

typedef struct NODO{
	elemento juego;
	struct NODO *sig;
}nodo;

nodo *generarElemento(elemento juego){
	nodo *nuevo;
	nuevo = (nodo *)malloc(sizeof(nodo));
	nuevo->juego = juego;
	nuevo->sig = NULL;
	return nuevo;
}

int comprobarNum(char check[N]){
	int pos, numero;
	pos=0; /* "pos" sirve para conducirme entre los caracteres de la cadena. */
	while(((check[pos]=='+' && pos == 0) || (check[pos]>='0' && check[pos]<='9')) && (pos <= N && check[pos] != '\0')){
		if(check[pos]=='+' && (check[pos+1]<'0' || check[pos+1]>'9')) break;
		pos++;
	}
	if(check[pos]=='\0')
		numero = atoi(check);
	else 
		numero = -1;
	return numero;
}

bool leerString(char string[], size_t len, const char when){
	char c;
	unsigned int i = 0;
	
	do{
		c = getchar();
		if(i < len-1 && c != when){
			string[i] = c;
		}else{
			string[i] = '\0';
		}
		i++;
	}while(c != when);
	fflush(stdin);
	if(i >= len) return false;
	return true;
}

void fputs_MEX_HTML(const char string[], size_t size, FILE* file){
	int counter = 0;
	while(string[counter] != '\0' && counter < size-1){
		switch(string[counter]){
		case 'â': fputs("&Aacute;",file); break;
		case 'á': fputs("&aacute;",file); break;
		case 'É': fputs("&Eacute;",file); break;
		case 'é': fputs("&eacute;",file); break;
		case 'î': fputs("&Iacute;",file); break; 
		case 'í': fputs("&iacute;",file); break;
		case 'ô': fputs("&Oacute;",file); break;
		case 'ó': fputs("&oacute;",file); break;
		case 'û': fputs("&Uacute;",file); break;
		case 'ú': fputs("&uacute;",file); break;
		case 'Ü': fputs("&Uuml;",file); break;
		case 'ü': fputs("&uuml;",file); break;
		case '¿': fputs("&iquest;",file); break;
		case '¡': fputs("&iexcl;",file); break;
		case 'Ñ': fputs("&Ntilde;",file); break;
		case 'ñ': fputs("&ntilde;",file); break;
		default: fputc(string[counter],file);
		}
		counter++;
	}
}

int contarElementos(nodo *lista){
	int c = 0;
	nodo *temporal;
	temporal = lista;
	
	while(temporal != NULL){
		c++;
		temporal = temporal->sig;
	}
	
	return c;
}

nodo *insertarElemento(nodo *lista, elemento juego){
	nodo *nuevo, *tempActual, *tempAnterior;
	int c;
	
	nuevo = generarElemento(juego);
	if(lista == NULL){
		lista = nuevo;
	}else{
		tempActual = tempAnterior = lista;
		c = 1;
		while(tempActual != NULL){
			if(strcmp(juego.nombre,tempActual->juego.nombre) > 0){
				c++;
				tempAnterior = tempActual;
				tempActual = tempActual->sig;
				continue;
			}
			break;
		}
		nuevo->sig = tempActual;
		if(c > 1)
			tempAnterior->sig = nuevo;
		else
			lista = nuevo;
	}
	
	return lista;
}

nodo *eliminarElemento(nodo *lista, char nombre[N], bool *deleted){
	nodo *tempActual, *tempAnterior;
	int c;
	*deleted = false;
	
	if(lista != NULL){
		c = 1;
		tempActual = tempAnterior = lista;
		while(tempActual != NULL){
			if(strcmp(nombre, tempActual->juego.nombre) != 0){
				c++;
				tempAnterior = tempActual;
				tempActual = tempActual->sig;
				continue;
			}
			break;
		}
		if(tempActual != NULL){
			if(c > 1)
				tempAnterior->sig = tempAnterior->sig->sig;
			else
				lista = lista->sig;
			free(tempActual);
			*deleted = true;
		}
	}
	
	return lista;
}

nodo *buscarElemento(nodo *lista, char nombre[N]){
	nodo *game;
	
	if(lista != NULL){
		game = lista;
		while(game != NULL){
			if(strcmp(nombre, game->juego.nombre) == 0)
				return game;
			game = game->sig;
		}
	}
	
	return NULL;
}

void mostrarElementos(nodo *lista){
	nodo *temp;
	int i = 0;
	temp = lista;
	
	if (temp != NULL){
		printf(" ! Solo se mostrara la informacion importante.\n");
		while(temp != NULL){
			i++;
			printf(" #------------------------------------------#\n");
			printf(" # Datos del elemento:                      #\n");
			printf(" # < Nombre: %s\n", temp->juego.nombre);
			printf(" # < Enlace de imagen: %s\n", temp->juego.link);
			printf(" # < Año (concluido): %d\n", temp->juego.year);
			printf(" # < Porcentaje concluido: %d%%\n", temp->juego.percentage);
			printf(" # < Plataformas: %s\n", temp->juego.platforms);
			printf(" #------------------------------------------#\n");
			temp = temp->sig;
			if(i == 3 && temp != NULL){
				printf(" > Ingresa enter para ver otros 3 elementos. ");
				getchar();
				system("cls");
				printf(" ! Mostrando otros 3 elementos mas.\n");
				i = 0;
			}
		}
		printf(" ! Ya se han mostrado todos los elementos actuales.\n");
	}else printf(" ! No hay elementos a mostrar.\n");
}

void generarHTML_index(nodo* lista, FILE *archivo){
	nodo *temporal;
	char c, caracter;
	bool existe, primero;
	
	temporal = lista;
	fprintf(archivo,"<div class=\"index\" style=\"text-align: justify;\">");				/* Abrir contenedor de indice */
	fprintf(archivo,"<p style=\"text-align: center; margin: 3px;\">&Iacute;ndice</p>");		/* Titulo de indice */
	caracter = temporal->juego.nombre[0];						/* Obtener caracter del primer nombre para indice */
	if(caracter >= '0' && caracter <= '9'){												/* El nombre del juego empieza con un numero */
		existe = true;
		primero = true;
		fprintf(archivo,"<b>#</b>: ");											/* Letra '#' de indice */
		while(temporal != NULL && existe){
			caracter = temporal->juego.nombre[0];
			if(caracter < '0' || caracter > '9') existe = false;
			else{
				if(primero){ fprintf(archivo,"<a href=\"#"); primero = false; }
				else fprintf(archivo," &#8212; <a href=\"#");
				fputs_MEX_HTML(temporal->juego.nombre,N,archivo);				/* Ingresa ID del juego para la navegacion */
				fprintf(archivo,"\"><i>");
				fputs_MEX_HTML(temporal->juego.nombre,N,archivo);				/* Ingresa el nombre del juego en indice */
				fprintf(archivo,"</i></a>");
				temporal = temporal->sig;
			}
		}
		fprintf(archivo,".<br>");
	}
	if((caracter >= 'A' && caracter <= 'Z') || (caracter >= 'a' && caracter <= 'z')){	/* El nombre del juego empieza con un caracter de 'A' a 'Z' */
		for(c = 'A'; c <= 'Z' && temporal != NULL; c++){
			caracter = temporal->juego.nombre[0];
			if(caracter >= 'a' && caracter <= 'z') caracter -= 'a' - 'A'; 
			if(caracter == c){
				existe = true;
				primero = true;
				fprintf(archivo,"<b>%c</b>: ",c);								/* Letras del abecedario de indice */
				while(temporal != NULL && existe){
					caracter = temporal->juego.nombre[0];
					if(caracter >= 'a' && caracter <= 'z') caracter -= 'a' - 'A'; 
					if(caracter != c) existe = false;
					else{
						if(primero){ fprintf(archivo,"<a href=\"#"); primero = false; }
						else fprintf(archivo," &#8212; <a href=\"#");
						fputs_MEX_HTML(temporal->juego.nombre,N,archivo);		/* Ingresa ID del juego para la navegacion */
						fprintf(archivo,"\"><i>");
						fputs_MEX_HTML(temporal->juego.nombre,N,archivo);		/* Ingresa el nombre del juego en indice */
						fprintf(archivo,"</i></a>");
						temporal = temporal->sig;
					}
				}
				fprintf(archivo,".<br>");
			}
		}
	}
	fseek(archivo,-4,SEEK_END);		/* Se regresa 4 caracteres del archivo */
	fprintf(archivo,"</div><br>");	/* Se cierra el contenedor */
	fputc('\n',archivo);
}

bool generarHTML(nodo *lista, char filename[N]){
	FILE *archivo, *plantilla;
	nodo *temporal;
	int id = 0;
	bool flag = false;
	
	if((archivo = fopen(filename,"w")) != NULL){
		if((plantilla = fopen("playgames_data\\first_message_template.html","r")) != NULL){
			temporal = lista;
			while(!feof(plantilla))
				fputc(fgetc(plantilla),archivo);
			fseek(archivo,-1,SEEK_END);		/* Se regresa un caracter del archivo */
			fputc('\n',archivo);			/* Borra el ultimo caracter basura */
			if(contarElementos(lista) != 0) generarHTML_index(lista, archivo);
			while(temporal != NULL){
				fprintf(archivo,"<table class=\"estiloco\" id=\""); fputs_MEX_HTML(temporal->juego.nombre,N,archivo); fprintf(archivo,"\">");			/* Abre tabla y esccribe ID para indice */
				fprintf(archivo,"<tr><th colspan=\"2\" class=\"title\">"); fputs_MEX_HTML(temporal->juego.nombre,N,archivo); fprintf(archivo,"</th></tr>\n");			/* Titulo */
				/* fprintf(archivo,"<table class=\"estiloco\"><tr><th colspan=\"2\" class=\"title\">%s</th></tr>\n",temporal->juego.nombre);													Titulo sin caracteres ilegales */
				fprintf(archivo,"<tr><td colspan=\"2\"><img src=\"%s\" style=\"max-width: 100%%; height: auto;\"/></td></tr>\n",temporal->juego.link);											/* Imagen */
				fprintf(archivo,"<tr><td colspan=\"2\"class=\"review_spoiler\">"); fputs_MEX_HTML(temporal->juego.opinion,E,archivo); fprintf(archivo,"</td></tr>\n");							/* Opinion */
				/* fprintf(archivo,"<tr><td colspan=\"2\"class=\"review_spoiler\">%s</td></tr>\n",temporal->juego.opinion);																		Opinion sin caracteres ilegales */
				if(strcmp(temporal->juego.spoiler,"*") != 0){
					id++;
					fprintf(archivo,"<tr><td colspan=\"2\" class=\"review_spoiler\"><center><button onclick=\"toggle_visibility(\'spoiler%d\')\">Mostrar/Ocultar Spoiler",id);												/* Spoiler 1/2 */
					fprintf(archivo,"</button></center><span id=\"spoiler%d\" style=\"display:none; margin-top:5px;\">",id); fputs_MEX_HTML(temporal->juego.spoiler,E,archivo); fprintf(archivo,"</span></td></tr>\n");		/* Spoiler 2/2 */
					/* fprintf(archivo,"<tr><td colspan=\"2\" class=\"review_spoiler\"><center><button onclick=\"toggle_visibility(\'spoiler%d\')\">Mostrar/Ocultar Spoiler",id);					Spoiler 1/2 sin caracteres ilegales */
					/* fprintf(archivo,"</button></center><span id=\"spoiler%d\" style=\"display:none; margin-top:5px;\">%s</span></td></tr>\n",id,temporal->juego.spoiler);						Spoiler 2/2 sin caracteres ilegales */
				}
				if(strcmp(temporal->juego.nota,"*") != 0){
					fprintf(archivo,"<tr><td colspan=\"2\" class=\"note\">"); fputs_MEX_HTML(temporal->juego.nota,E,archivo); fprintf(archivo,"</td></tr>\n");																/* Nota */
					/* fprintf(archivo,"<tr><td colspan=\"2\" class=\"note\">%s</td></tr>\n",temporal->juego.nota); 																			Opinion sin caracteres especiales */
				}
				fprintf(archivo,"<tr><td class=\"mini\">Completado en el a&ntilde;o: <b>%d</b></td>",temporal->juego.year);														/* Year */
				fprintf(archivo,"<td class=\"mini\">Porcentaje completado: <b>%d%%</b></td></tr>\n",temporal->juego.percentage);												/* Porcentaje */
				fprintf(archivo,"<tr><td colspan=\"2\" class=\"platforms\">Jugado en: <b>%s</b></td></tr>",temporal->juego.platforms);							/* Plataformas */
				fprintf(archivo,"</table></br>\n\n");																							/* Cierra tabla */
				temporal = temporal->sig;
			}
			flag = true;
			fclose(plantilla);
		}
		fclose(archivo);
	}
	return flag;
}

void initialText(){
	printf(" |--------------------------------------------|\n");
	printf(" |  ESTE PROGRAMA SIEMPRE SE EJECUTA EN MODO  |\n");
	printf(" |        ADMINISTRADOR. ATENTO MORRO!.       |\n");
	printf(" |--------------------------------------------|\n");
	system("pause");
}

int menu(){
	char num[5];
	int op;
	printf(" |--------------------------------------------|\n");
	printf(" |  Menú para generar tablas de juegos XD     |\n");
	printf(" |                                            |\n");
	printf(" | 1 - Insertar un elemento                   |\n");
	printf(" | 2 - Eliminar un elemento                   |\n");
	printf(" | 3 - Editar un elemento                     |\n");
	printf(" |                                            |\n");
	printf(" | 4 - Generar documento HTML                 |\n");
	printf(" |                                            |\n");
	printf(" | 5 - Guardar datos                          |\n");
	printf(" | 6 - Recuperar datos                        |\n");
	printf(" |                                            |\n");
	printf(" | 7 - Mostrar elementos actuales             |\n");
	printf(" |                                            |\n");
	printf(" | 0 - Salir                                  |\n");
	printf(" |--------------------------------------------|\n");
	printf(" > Elige una opcion: ");
	leerString(num,5,'\n');
	if(num[1] == '\0' && (num[0] >= '0' && num[0] <= '9'))
		op = atoi(num);
	else
		op = -1;
	return op;
}

nodo *USERinsertarElemento(nodo *lista, bool *unsaved){
	char nombre[N], link[L], opinion[E], spoiler[E], nota[E], platforms[N], numTemp[5], opTemp[5];
	elemento juego_temporal;
	int op, year, percentage;
	bool flag = false;
	
	printf(" | Has elegido 'Insertar un elemento' |\n");
	do{
		printf(" > Ingresa el nombre del elemento (juego): ");									/* Nombre del juego */
		leerString(nombre,N,'\n');
		if(buscarElemento(lista,nombre) != NULL)
			printf(" ! Ese elemento ya ha sido registrado.\n");
		else flag = true;
	}while(!flag);
	printf(" > Ingresa el enlace de una imagen del juego: ");									/* Link a Imagen del juego */
	leerString(link,L,'\n');
	do{
		printf(" > Ingresa tu opinion/review sobre juego: ");									/* Opinion/Review del juego */
		if(!leerString(opinion,E,'\n')){
			do{
				printf(" ! La opinion supera el limite de caracteres disponibles.\n");
				printf(" > Quieres continuar? (1 - Si | 2 - No): ");
				leerString(opTemp,5,'\n');
				if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
					op = atoi(opTemp);
				else
					op = -1;
			}while(op != 1 && op != 2);
		}else op = 1;
	}while(op != 1);
	do{
		printf(" > Quieres agregar algun spoiler? (1 - Si | 2 - No): ");						/* Spoiler del juego */
		leerString(opTemp,5,'\n');
		if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
			op = atoi(opTemp);
		else
			op = -1;
	}while(op != 1 && op != 2);
	if(op == 1){
		do{
			printf(" > Ingresa un spoiler sobre el juego: ");
			if(!leerString(spoiler,E,'\n')){
				do{
					printf(" ! El spoiler supera el limite de caracteres disponibles.\n");
					printf(" > Quieres continuar? (1 - Si | 2 - No): ");
					leerString(opTemp,5,'\n');
					if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
						op = atoi(opTemp);
					else
						op = -1;
				}while(op != 1 && op != 2);
			}else op = 1;
		}while(op != 1);
	}else strcpy(spoiler, "*");
	do{
		printf(" > Quieres agregar alguna nota? (1 - Si | 2 - No): ");							/* Nota sobre el juego */
		leerString(opTemp,5,'\n');
		if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
			op = atoi(opTemp);
		else
			op = -1;
	}while(op != 1 && op != 2);
	if(op == 1){
		do{
			printf(" > Ingresa una nota sobre el juego: ");
			if(!leerString(nota,E,'\n')){
				do{
					printf(" ! La nota supera el limite de caracteres disponibles.\n");
					printf(" > Quieres continuar? (1 - Si | 2 - No): ");
					leerString(opTemp,5,'\n');
					if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
						op = atoi(opTemp);
					else
						op = -1;
				}while(op != 1 && op != 2);
			}else op = 1;
		}while(op != 1);
	}else strcpy(nota, "*");
	do{
		printf(" > Ingresa el 'year' en el que pasaste el juego: ");							/* Year en el que se completo el juego */
		leerString(numTemp,5,'\n');
		year = comprobarNum(numTemp);
	}while(year < 1999 || year > 2020);
	do{
		printf(" > Ingresa el porcentaje (sin '%%') completado del juego: ");						/* Porcentaje completado del juego */
		leerString(numTemp,5,'\n');
		percentage = comprobarNum(numTemp);
	}while(percentage < 0 || percentage > 100);
	printf(" > Ingresa las plataformas en donde tienes el juego (separadas por ','): ");		/* Plataformas en donde se tiene el juego */
	leerString(platforms,N,'\n');
	do{
		printf(" > Estas seguro de que es correcta la informacion? (1 - Si | 2 - No): ");
		leerString(opTemp,5,'\n');
		if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
			op = atoi(opTemp);
		else
			op = -1;
	}while(op != 1 && op != 2);
	if(op == 1){
		strcpy(juego_temporal.nombre,nombre);
		strcpy(juego_temporal.link,link);
		strcpy(juego_temporal.opinion,opinion);
		strcpy(juego_temporal.spoiler,spoiler);
		strcpy(juego_temporal.nota,nota);
		strcpy(juego_temporal.platforms,platforms);
		juego_temporal.year = year;
		juego_temporal.percentage = percentage;		
		lista = insertarElemento(lista, juego_temporal);
		system("cls");
		printf(" ! Se ha insertado el elemento correctamente.\n");
		*unsaved = true;
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
	
	return lista;
}

nodo *USEReliminarElemento(nodo *lista, bool *unsaved){
	int op;
	bool flag;
	char nombre[N], opTemp[5];
	
	printf(" | Has elegido 'Eliminar un elemento' |\n");
	printf(" > Ingresa el nombre del elemento (juego) a eliminar: ");
	leerString(nombre,N,'\n');
	if(buscarElemento(lista, nombre) != NULL){
		do{
			printf(" > Estas seguro de que quiere borrar el elemento? (1 - Si | 2 - No): ");
			leerString(opTemp,5,'\n');
			if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
				op = atoi(opTemp);
			else
				op = -1;
		}while(op != 1 && op != 2);
		if(op == 1){
			lista = eliminarElemento(lista, nombre, &flag);
			system("cls");
			flag ? printf(" ! Se ha borrado el elemento correctamente.\n"): printf(" ! Hubo un error al borrar el elemento. \n");
			*unsaved = true;
		}else{
			system("cls");
			printf(" ! Se ha cancelado la operacion.\n");
		}
	}else{
		system("cls");
		printf(" ! No se encontro el elemento a borrar.\n");
	}
	
	return lista;
}

void USEReditarElemento(nodo *lista, bool *unsaved){
	nodo *game;
	char nombre[N], link[L], opinion[E], spoiler[E], nota[E], platforms[N],numTemp[5], opTemp[5];
	int op ,year, percentage;
	bool flag1 = true, flag2;
	
	printf(" | Has elegido 'Editar un elemento' |\n");
	printf(" > Ingresa el nombre del elemento (juego): ");
	leerString(nombre,N,'\n');
	if((game = buscarElemento(lista, nombre)) != NULL){
		do{
			system("cls");
			printf(" || - Nombre: %s\n\n",game->juego.nombre);
			printf(" ||----------------------------||\n");
			printf(" ||  Que le quieres editar?    ||\n");
			printf(" ||                            ||\n");
			printf(" || 1 - Editar nombre          ||\n");
			printf(" || 2 - Editar enlace          ||\n");
			printf(" || 3 - Editar opinion         ||\n");
			printf(" || 4 - Modificar spoiler      ||\n");
			printf(" || 5 - Modificar nota         ||\n");
			printf(" || 6 - Editar 'year'          ||\n");
			printf(" || 7 - Editar porcentaje      ||\n");
			printf(" || 8 - Editar plataformas     ||\n");
			printf(" ||                            ||\n");
			printf(" || 0 - Regresar al menu       ||\n");
			printf(" ||----------------------------||\n");
			printf(" >> Elige una opcion: ");
			leerString(opTemp,5,'\n');
			if(opTemp[1] == '\0' && (opTemp[0] >= '0' && opTemp[0] <= '9'))
				op = atoi(opTemp);
			else
				op = -1;
			switch(op){
				case 1:
					system("cls");
					printf(" || Has elegido 'Editar nombre' ||\n");
					printf(" >> Ingresa el nuevo nombre: ");
					leerString(nombre,N,'\n');
					do{
						printf(" >> Estas seguro de que quieres cambiar el nombre? (1 - Si | 2 - No): ");
						leerString(opTemp,5,'\n');
						if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
							op = atoi(opTemp);
						else
							op = -1;
					}while(op != 1 && op != 2);
					if(op == 1){
						strcpy(game->juego.nombre, nombre);
						system("cls");
						printf(" !! Se ha cambiado el nombre correctamente.\n");
						*unsaved = true;
					}else{
						system("cls");
						printf(" !! Se ha cancelado la operacion.\n");
					}
					getchar();
					system("cls");
				break;
				case 2:
					system("cls");
					printf(" || Has elegido 'Editar enlace' ||\n");
					printf(" >> Ingresa el nuevo enlace: ");
					leerString(link,L,'\n');
					do{
						printf(" >> Estas seguro de que quieres cambiar el enlace? (1 - Si | 2 - No): ");
						leerString(opTemp,5,'\n');
						if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
							op = atoi(opTemp);
						else
							op = -1;
					}while(op != 1 && op != 2);
					if(op == 1){
						strcpy(game->juego.link, link);
						system("cls");
						printf(" !! Se ha cambiado el enlace correctamente.\n");
						*unsaved = true;
					}else{
						system("cls");
						printf(" !! Se ha cancelado la operacion.\n");
					}
					getchar();
					system("cls");
				break;
				case 3:
					system("cls");
					printf(" || Has elegido 'Editar opinion' ||\n");
					do{
						printf(" >> Ingresa tu nueva opinion/review: ");
						if(!leerString(opinion,E,'\n')){
							do{
								printf(" !! La opinion supera el limite de caracteres disponibles.\n");
								printf(" >> Quieres continuar? (1 - Si | 2 - No): ");
								leerString(opTemp,5,'\n');
								if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
									op = atoi(opTemp);
								else
									op = -1;
							}while(op != 1 && op != 2);
						}else op = 1;
					}while(op != 1);
					do{
						printf(" >> Estas seguro de que quieres cambiar la opinion? (1 - Si | 2 - No): ");
						leerString(opTemp,5,'\n');
						if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
							op = atoi(opTemp);
						else
							op = -1;
					}while(op != 1 && op != 2);
					if(op == 1){
						strcpy(game->juego.opinion, opinion);
						system("cls");
						printf(" !! Se ha cambiado la opinion correctamente.\n");
						*unsaved = true;
					}else{
						system("cls");
						printf(" !! Se ha cancelado la operacion.\n");
					}
					getchar();
					system("cls");
				break;
				case 4:
					flag2 = true;
					do{
						system("cls");
						printf(" ||| Has elegido 'Modificar spoiler' |||\n\n");
						printf(" |||---------------------------------|||\n");
						printf(" |||  Que le quieres hacer?          |||\n");
						printf(" |||                                 |||\n");
						if(!strcmp(game->juego.spoiler,"*"))
							printf(" ||| 1 - Agregar spoiler             |||\n");
						else{
							printf(" ||| 2 - Editar spoiler              |||\n");
							printf(" ||| 3 - Borrar spoiler              |||\n");
						}
						printf(" |||                                 |||\n");
						printf(" ||| 0 - Regresar al menu anterior   |||\n");
						printf(" |||---------------------------------|||\n");
						printf(" >>> Elige una opcion: ");
						leerString(opTemp,5,'\n');
						if(opTemp[1] == '\0' && (opTemp[0] >= '0' && opTemp[0] <= '9'))
							op = atoi(opTemp);
						else
							op = -1;
						switch(op){
							case 1:
								if(!strcmp(game->juego.spoiler,"*")){
									system("cls");
									printf(" ||| Has elegido 'Agregar spoiler' |||\n");
									do{
										printf(" >>> Ingresa un spoiler sobre el juego: ");
										if(!leerString(spoiler,E,'\n')){
											do{
												printf(" !!! El spoiler supera el limite de caracteres disponibles.\n");
												printf(" >>> Quieres continuar? (1 - Si | 2 - No): ");
												leerString(opTemp,5,'\n');
												if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
													op = atoi(opTemp);
												else
													op = -1;
											}while(op != 1 && op != 2);
										}else op = 1;
									}while(op != 1);
									do{
										printf(" >>> Estas seguro de que quieres agregar el spoiler? (1 - Si | 2 - No): ");
										leerString(opTemp,5,'\n');
										if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
											op = atoi(opTemp);
										else
											op = -1;
									}while(op != 1 && op != 2);
									if(op == 1){
										strcpy(game->juego.spoiler, spoiler);
										system("cls");
										printf(" !!! Se ha agregado el spoiler correctamente.\n");
										*unsaved = true;
									}else{
										system("cls");
										printf(" !!! Se ha cancelado la operacion.\n");
									}
								}else{
									system("cls");
									printf(" !!! No puedes acceder a esta opcion.\n");
								}
								getchar();
								system("cls");
							break;
							case 2:
								if(strcmp(game->juego.spoiler,"*") != 0){								
									system("cls");
									printf(" ||| Has elegido 'Editar spoiler' |||\n");
									do{
										printf(" >>> Ingresa un nuevo spoiler: ");
										if(!leerString(spoiler,E,'\n')){
											do{
												printf(" !!! El spoiler supera el limite de caracteres disponibles.\n");
												printf(" >>> Quieres continuar? (1 - Si | 2 - No): ");
												leerString(opTemp,5,'\n');
												if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
													op = atoi(opTemp);
												else
													op = -1;
											}while(op != 1 && op != 2);
										}else op = 1;
									}while(op != 1);
									do{
										printf(" >>> Estas seguro de que quieres cambiar el spoiler? (1 - Si | 2 - No): ");
										leerString(opTemp,5,'\n');
										if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
											op = atoi(opTemp);
										else
											op = -1;
									}while(op != 1 && op != 2);
									if(op == 1){
										strcpy(game->juego.spoiler, spoiler);
										system("cls");
										printf(" !!! Se ha cambiado el spoiler correctamente.\n");
										*unsaved = true;
									}else{
										system("cls");
										printf(" !!! Se ha cancelado la operacion.\n");
									}
								}else{
									system("cls");
									printf(" !!! No puedes acceder a esta opcion.\n");
								}
								getchar();
								system("cls");
							break;
							case 3:
								if(strcmp(game->juego.spoiler,"*") != 0){
									system("cls");
									printf(" ||| Has elegido 'Borrar spoiler' |||\n");
									do{
										printf(" >>> Estas seguro de que quieres borrar el spoiler? (1 - Si | 2 - No): ");
										leerString(opTemp,5,'\n');
										if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
											op = atoi(opTemp);
										else
											op = -1;
									}while(op != 1 && op != 2);
									if(op == 1){
										strcpy(game->juego.spoiler, "*");
										system("cls");
										printf(" !!! Se ha borrado el spoiler correctamente.\n");
										*unsaved = true;
									}else{
										system("cls");
										printf(" !!! Se ha cancelado la operacion.\n");
									}
								}else{
									system("cls");
									printf(" !!! No puedes acceder aesta opcion.\n");
								}
								getchar();
								system("cls");
							break;
							case 0:
								flag2 = false;
							break;
							default:
								system("cls");
								printf(" !!! Opcion invalida.\n");
								getchar();
								system("cls");
						}
					}while(flag2);
				break;
			case 5:
				flag2 = true;
				do{
					system("cls");
					printf(" ||| Has elegido 'Modificar nota'    |||\n\n");
					printf(" |||---------------------------------|||\n");
					printf(" |||  Que le quieres hacer?          |||\n");
					printf(" |||                                 |||\n");
					if(!strcmp(game->juego.nota,"*"))
						printf(" ||| 1 - Agregar nota                |||\n");
					else{
						printf(" ||| 2 - Editar nota                 |||\n");
						printf(" ||| 3 - Borrar nota                 |||\n");
					}
					printf(" |||                                 |||\n");
					printf(" ||| 0 - Regresar al menu anterior   |||\n");
					printf(" |||---------------------------------|||\n");
					printf(" >>> Elige una opcion: ");
					leerString(opTemp,5,'\n');
					if(opTemp[1] == '\0' && (opTemp[0] >= '0' && opTemp[0] <= '9'))
						op = atoi(opTemp);
					else
						op = -1;
					switch(op){
					case 1:
						if(!strcmp(game->juego.nota,"*")){
							system("cls");
							printf(" ||| Has elegido 'Agregar nota' |||\n");
							do{
								printf(" >>> Ingresa una nota sobre el juego: ");
								if(!leerString(nota,E,'\n')){
									do{
										printf(" !!! La nota supera el limite de caracteres disponibles.\n");
										printf(" >>> Quieres continuar? (1 - Si | 2 - No): ");
										leerString(opTemp,5,'\n');
										if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
											op = atoi(opTemp);
										else
											op = -1;
									}while(op != 1 && op != 2);
								}else op = 1;
							}while(op != 1);
							do{
								printf(" >>> Estas seguro de que quieres agregar la nota? (1 - Si | 2 - No): ");
								leerString(opTemp,5,'\n');
								if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
									op = atoi(opTemp);
								else
									op = -1;
							}while(op != 1 && op != 2);
							if(op == 1){
								strcpy(game->juego.nota, nota);
								system("cls");
								printf(" !!! Se ha agregado la nota correctamente.\n");
								*unsaved = true;
							}else{
								system("cls");
								printf(" !!! Se ha cancelado la operacion.\n");
							}
						}else{
							system("cls");
							printf(" !!! No puedes acceder a esta opcion.\n");
						}
						getchar();
						system("cls");
						break;
					case 2:
						if(strcmp(game->juego.nota,"*") != 0){								
							system("cls");
							printf(" ||| Has elegido 'Editar nota' |||\n");
							do{
								printf(" >>> Ingresa una nueva nota: ");
								if(!leerString(nota,E,'\n')){
									do{
										printf(" !!! La nota supera el limite de caracteres disponibles.\n");
										printf(" >>> Quieres continuar? (1 - Si | 2 - No): ");
										leerString(opTemp,5,'\n');
										if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
											op = atoi(opTemp);
										else
											op = -1;
									}while(op != 1 && op != 2);
								}else op = 1;
							}while(op != 1);
							do{
								printf(" >>> Estas seguro de que quieres cambiar la nota? (1 - Si | 2 - No): ");
								leerString(opTemp,5,'\n');
								if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
									op = atoi(opTemp);
								else
									op = -1;
							}while(op != 1 && op != 2);
							if(op == 1){
								strcpy(game->juego.nota, nota);
								system("cls");
								printf(" !!! Se ha cambiado la nota correctamente.\n");
								*unsaved = true;
							}else{
								system("cls");
								printf(" !!! Se ha cancelado la operacion.\n");
							}
						}else{
							system("cls");
							printf(" !!! No puedes acceder a esta opcion.\n");
						}
						getchar();
						system("cls");
						break;
					case 3:
						if(strcmp(game->juego.nota,"*") != 0){
							system("cls");
							printf(" ||| Has elegido 'Borrar nota' |||\n");
							do{
								printf(" >>> Estas seguro de que quieres borrar la nota? (1 - Si | 2 - No): ");
								leerString(opTemp,5,'\n');
								if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
									op = atoi(opTemp);
								else
									op = -1;
							}while(op != 1 && op != 2);
							if(op == 1){
								strcpy(game->juego.nota, "*");
								system("cls");
								printf(" !!! Se ha borrado la nota correctamente.\n");
								*unsaved = true;
							}else{
								system("cls");
								printf(" !!! Se ha cancelado la operacion.\n");
							}
						}else{
							system("cls");
							printf(" !!! No puedes acceder aesta opcion.\n");
						}
						getchar();
						system("cls");
						break;
					case 0:
						flag2 = false;
						break;
					default:
						system("cls");
						printf(" !!! Opcion invalida.\n");
						getchar();
						system("cls");
					}
				}while(flag2);
				break;
				case 6:
					system("cls");
					printf(" || Has elegido 'Editar 'year'' ||\n");
					do{
						printf(" >> Ingresa el nuevo 'year': ");
						leerString(numTemp,5,'\n');
						year = comprobarNum(numTemp);
					}while(year < 1999 || year > 2019);
					do{
						printf(" >> Estas seguro de que quieres cambiar el 'year'? (1 - Si | 2 - No): ");
						leerString(opTemp,5,'\n');
						if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
							op = atoi(opTemp);
						else
							op = -1;
					}while(op != 1 && op != 2);
					if(op == 1){
						game->juego.year = year;
						system("cls");
						printf(" !! Se ha cambiado el 'year' correctamente.\n");
						*unsaved = true;
					}else{
						system("cls");
						printf(" !! Se ha cancelado la operacion.\n");
					}
					getchar();
					system("cls");
				break;
				case 7:
					system("cls");
					printf(" || Has elegido 'Editar porcentaje' ||\n");
					do{
						printf(" >> Ingresa el nuevo porcentaje: ");
						leerString(numTemp,5,'\n');
						percentage = comprobarNum(numTemp);
					}while(percentage < 0 || percentage > 100);
					do{
						printf(" >> Estas seguro de que quieres cambiar el porcentaje? (1 - Si | 2 - No): ");
						leerString(opTemp,5,'\n');
						fflush(stdin);
						if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
							op = atoi(opTemp);
						else
							op = -1;
					}while(op != 1 && op != 2);
					if(op == 1){
						game->juego.percentage = percentage;
						system("cls");
						printf(" !! Se ha cambiado el porcentaje correctamente.\n");
						*unsaved = true;
					}else{
						system("cls");
						printf(" !! Se ha cancelado la operacion.\n");
					}
					getchar();
					system("cls");
				break;
				case 8:
					system("cls");
					printf(" || Has elegido 'Editar plataformas' ||\n");
					printf(" >> Ingresa las plataformas: ");
					leerString(platforms,N,'\n');
					do{
						printf(" >> Estas seguro de que quieres cambiar las plataformas? (1 - Si | 2 - No): ");
						leerString(opTemp,5,'\n');
						if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
							op = atoi(opTemp);
						else
							op = -1;
					}while(op != 1 && op != 2);
					if(op == 1){
						strcpy(game->juego.platforms, platforms);
						system("cls");
						printf(" !! Se han cambiado las plataformas correctamente.\n");
						*unsaved = true;
					}else{
						system("cls");
						printf(" !! Se ha cancelado la operacion.\n");
					}
					getchar();
					system("cls");
				break;
				case 0:
					flag1 = false;
				break;
				default:
					system("cls");
					printf(" !! Opcion invalida.\n");
					getchar();
					system("cls");
			}
		}while(flag1);
	}else{
		system("cls");
		printf(" ! No se encontro el elemento ingresado.\n");
		getchar();
	}
}

void USERgenerarHTML(nodo *lista){
	char opTemp[5], name[N];
	int op;
	
	printf(" | Has elegido 'Generar documento HTML' |\n");
	printf(" > Ingresa el nombre del archivo (sin extension): ");
	leerString(name,N,'\n');
	do{
		printf(" > Estas seguro de que quieres generar el HTML? (1 - Si | 2 - No): ");
		leerString(opTemp,5,'\n');
		if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
			op = atoi(opTemp);
		else
			op = -1;
	}while(op != 1 && op != 2);
	if(op == 1){
		strcat(name,".html");
		system("cls");
		generarHTML(lista, name) ? printf(" ! Se ha generado el documento correctamente.\n") : printf(" ! Hubo un error al generar el documento.\n");
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
}

void generateBackup(nodo *lista, int *backup_count){
	char filename[60], numBackup[10];
	nodo *temporal;
	int cantidad;
	FILE *backup, *parameter;
	
	if(*backup_count > -1){
		(*backup_count)++;
		if((parameter = fopen("playgames_data\\backup_count.txt","w")) != NULL){	/* Incrementa la cantidad de backups en el archivo */
			fprintf(parameter,"%d",*backup_count);
			fclose(parameter);
		}
		strcpy(filename,"playgames_data\\playgames_backups\\pg_backup_"); sprintf(numBackup,"%d",*backup_count); /* Crea el nombre del archivo backup */
		strcat(numBackup,".dat"); strcat(filename,numBackup);
		if((backup = fopen(filename,"wb")) != NULL){
			temporal = lista;
			cantidad = contarElementos(lista);
			fwrite(&cantidad,sizeof(int),1,backup);
			while(temporal != NULL){
				fwrite(temporal,sizeof(nodo),1,backup);
				temporal = temporal->sig;
			}
			fclose(backup);
		}
	}
}

void USERguardarElementos(nodo *lista, int *backup_count, bool *unsaved){
	FILE *binario;
	nodo *temporal;
	char opTemp[5];
	int op, cantidad;
	
	printf(" | Has elegido 'Guardar datos' |\n");
	do{
		printf(" > Estas seguro de que quieres guardar los datos actuales? (1 - Si | 2 - No): ");
		leerString(opTemp,5,'\n');
		if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
			op = atoi(opTemp);
		else
			op = -1;
	}while(op != 1 && op != 2);
	if(op == 1){
		system("cls");
		generateBackup(lista, backup_count);
		if((binario = fopen("playgames_data\\playgames_latest.dat","wb")) != NULL){
			temporal = lista;
			cantidad = contarElementos(lista);
			fwrite(&cantidad,sizeof(int),1,binario);
			while(temporal != NULL){
				fwrite(temporal,sizeof(nodo),1,binario);
				temporal = temporal->sig;
			}
			fclose(binario);
			printf(" ! Se han guardado los datos correctamente.\n");
			*unsaved = false;
		}else printf(" ! Hubo un error al crear el archivo.\n");
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
}

nodo *USERcargarElementos(nodo *lista, bool *unsaved){
	FILE *binario;
	nodo *temporal;
	char opTemp[5];
	int op, cantidad, i;
	
	printf(" | Has elegido 'Recuperar datos' |\n");
	do{
		if(lista != NULL) printf(" ! La lista actual tiene elementos. Si continuas, se sobreescribiran datos.\n");
		printf(" > Estas seguro de que quieres cargar datos guardados? (1 - Si | 2 - No): ");
		leerString(opTemp,5,'\n');
		if(opTemp[1] == '\0' && (opTemp[0] == '1' || opTemp[0] <= '2'))
			op = atoi(opTemp);
		else
			op = -1;
	}while(op != 1 && op != 2);
	if(op == 1){
		system("cls");
		if((binario = fopen("playgames_data\\playgames_latest.dat","rb")) != NULL){
			temporal = (nodo *)malloc(sizeof(nodo));
			while(lista != NULL){
				temporal = lista;
				lista = lista->sig;
				free(temporal);
			}
			fread(&cantidad,sizeof(int),1,binario);
			for(i = 0; i < cantidad; i++){
				fread(temporal,sizeof(nodo),1,binario);
				lista = insertarElemento(lista, temporal->juego);
			}
			fclose(binario);
			printf(" ! Se han cargado los datos correctamente.\n");
			*unsaved = false;
		}else printf(" ! Hubo un error al cargar el archivo.\n");
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
	return lista;
}

void parameters(nodo **lista, int *cantidad, bool *load_on_start, bool *save_on_close){ /* No se ha comprobado su funcionamiento y falta añadir casos (si/no abrio el archivo) */
	FILE *parameter;
	
	if((parameter = fopen("playgames_data\\backup_count.txt","r")) != NULL){
		fscanf(parameter,"%d",cantidad);
		fclose(parameter);
	}
	
	if((parameter = fopen("playgames_data\\load_on_start.txt","r")) != NULL){
		if(fgetc(parameter) == '1') *load_on_start = true;
		else *load_on_start = false;
		fclose(parameter);
	}
	
	if((parameter = fopen("playgames_data\\save_on_close.txt","r")) != NULL){
		if(fgetc(parameter) == '1') *save_on_close = true;
		else *save_on_close = false;
		fclose(parameter);
	}
}

nodo *loadFromStart(nodo *lista){
	FILE *binario;
	nodo *temporal;
	int cantidad, i;
	
	if((binario = fopen("playgames_data\\playgames_latest.dat","rb")) != NULL){
		temporal = (nodo *)malloc(sizeof(nodo));
		fread(&cantidad,sizeof(int),1,binario);
		for(i = 0; i < cantidad; i++){
			fread(temporal,sizeof(nodo),1,binario);
			lista = insertarElemento(lista, temporal->juego);
		}
		fclose(binario);
	}
	return lista;	
}

void saveBeforeClose(nodo *lista, int *backup_count){
	FILE *binario;
	nodo *temporal;
	int cantidad;
	
	generateBackup(lista, backup_count);
	if((binario = fopen("playgames_data\\playgames_latest.dat","wb")) != NULL){
		temporal = lista;
		cantidad = contarElementos(lista);
		fwrite(&cantidad,sizeof(int),1,binario);
		while(temporal != NULL){
			fwrite(temporal,sizeof(nodo),1,binario);
			temporal = temporal->sig;
		}
		fclose(binario);
	}
}

int main(){
	nodo *lista;
	bool flag = true, cargar_en_inicio = false, guardar_al_final = false, unsaved = false;
	int cantidad_de_backups = -1;
	lista = NULL;
	
	parameters(&lista, &cantidad_de_backups, &cargar_en_inicio, &guardar_al_final);
	if(cargar_en_inicio) lista = loadFromStart(lista);
	
	/*system("cls");
	initialText();
	system("cls");*/
	do{
		switch(menu()){
			case 1:
				system("cls");
				lista = USERinsertarElemento(lista, &unsaved);
				getchar();
				system("cls");
			break;
			case 2:
				system("cls");
				if(lista == NULL) printf(" ! No hay elementos ingresados.\n");
				else lista = USEReliminarElemento(lista, &unsaved);
				getchar();
				system("cls");
			break;
			case 3:
				system("cls");
				if(lista == NULL){
					printf(" ! No hay elementos ingresados.\n");
					getchar();
				}
				else USEReditarElemento(lista, &unsaved);
				system("cls");
			break;
			case 4:
				system("cls");
				USERgenerarHTML(lista);
				getchar();
				system("cls");
			break;
			case 5:
				system("cls");
				if(lista != NULL) USERguardarElementos(lista, &cantidad_de_backups, &unsaved);
				else printf(" ! No hay elementos ingresados.\n");
				getchar();
				system("cls");
			break;
			case 6:
				system("cls");
				lista = USERcargarElementos(lista, &unsaved);
				getchar();
				system("cls");
			break;
			case 7:
				system("cls");
				mostrarElementos(lista);
				getchar();
				system("cls");
			break;
			case 8:
				system("cls");
				printf(" ! Cantidad de elementos en lista: %d.\n",contarElementos(lista));
				getchar();
				system("cls");
			break;
			case 9:
				system("cls");
				printf(" ! Modo debug en construccion...\n");									/* Hace modo debug que contendra muchas cosas, entre ellas mostrara status de parametros */
				getchar();
				system("cls");
			break;
			case 0:
				system("cls");
				printf(" | Gracias por ejecutar el programa :D |\n\n");
				flag = false;
			break;
			default:
				system("cls");
				printf(" ! Opcion invalida.\n");
				getchar();
				system("cls");
		}
	}while(flag);
	
	if(guardar_al_final && unsaved) saveBeforeClose(lista, &cantidad_de_backups);
	
	system("pause");
	return 0;
}
