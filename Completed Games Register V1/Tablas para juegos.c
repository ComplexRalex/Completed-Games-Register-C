#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#define N 100
#define L 150
#define E 300

typedef struct ELEMENTO{
	char nombre[N], link[L], opinion[E], spoiler[E], nota[E], platforms[N];
	int year, percentage;
	struct ELEMENTO *sig;
}elemento;

elemento *generarElemento(char nombre[N], char link[L], char opinion[E], char spoiler[E], char nota[E], int year, int percentage, char platforms[N]){
	elemento *nuevo;
	nuevo = (elemento *)malloc(sizeof(elemento));
	strcpy(nuevo->nombre,nombre);
	strcpy(nuevo->link,link);
	strcpy(nuevo->opinion,opinion);
	strcpy(nuevo->spoiler,spoiler);
	strcpy(nuevo->nota,nota);
	nuevo->year = year;
	nuevo->percentage = percentage;
	strcpy(nuevo->platforms,platforms);
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

int contarElementos(elemento *lista){
	int c = 0;
	elemento *temporal;
	temporal = lista;
	
	while(temporal != NULL){
		c++;
		temporal = temporal->sig;
	}
	
	return c;
}

elemento *insertarElemento(elemento *lista, char nombre[N], char link[L], char opinion[E], char spoiler[E], char nota[E], int year, int percentage, char platforms[N]){
	elemento *nuevo, *tempActual, *tempAnterior;
	int c;
	
	nuevo = generarElemento(nombre, link, opinion, spoiler, nota, year, percentage, platforms);
	if(lista == NULL){
		lista = nuevo;
	}else{
		tempActual = tempAnterior = lista;
		c = 1;
		while(tempActual != NULL){
			if(strcmp(nombre,tempActual->nombre) > 0){
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

elemento *eliminarElemento(elemento *lista, char nombre[N], bool *deleted){
	elemento *tempActual, *tempAnterior;
	int c;
	*deleted = false;
	
	if(lista != NULL){
		c = 1;
		tempActual = tempAnterior = lista;
		while(tempActual != NULL){
			if(strcmp(nombre, tempActual->nombre) != 0){
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

elemento *buscarElemento(elemento *lista, char nombre[N]){
	elemento *juego;
	
	if(lista != NULL){
		juego = lista;
		while(juego != NULL){
			if(strcmp(nombre, juego->nombre) == 0)
				return juego;
			juego = juego->sig;
		}
	}
	
	return NULL;
}

void mostrarElementos(elemento *lista){
	elemento *temp;
	int i = 0;
	temp = lista;
	
	if (temp != NULL){
		printf(" ! Solo se mostrara la informacion importante.\n");
		while(temp != NULL){
			i++;
			printf(" #------------------------------------------#\n");
			printf(" # Datos del elemento:                      #\n");
			printf(" # < Nombre: %s\n", temp->nombre);
			printf(" # < Enlace de imagen: %s\n", temp->link);
			printf(" # < Year (concluido): %d\n", temp->year);
			printf(" # < Porcentaje concluido: %d%%\n", temp->percentage);
			printf(" # < Plataformas: %s\n", temp->platforms);
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

bool generarHTML(elemento *lista, char filename[N]){
	FILE *archivo;
	elemento *temporal;
	int id=0;
	bool flag = false;
	
	if((archivo = fopen(filename,"w")) != NULL){
		temporal = lista;
		fprintf(archivo,"<head>\n<style type=\"text/css\">\nh3.post-title{display:none;}\np{text-align: justify;}\n");
		fprintf(archivo,"body{background-image: url(https://i.imgur.com/vEF2xlr.png); background-repeat: repeat; background-position: top;}\n");
		fprintf(archivo,".estiloco{border-collapse:collapse;border-spacing:0;border-color:#ccc}\n");
		fprintf(archivo,".estiloco td{font-family:Arial, sans-serif;font-size:14px;padding:10px 7px;border-style:solid;");
		fprintf(archivo,"border-width:1px;overflow:hidden;word-break:normal;border-color:#A4A4A4;color:#333;background-color:#fff;}\n");
		fprintf(archivo,".estiloco th{font-family:Arial, sans-serif;font-size:14px;font-weight:normal;padding:5px 5px;border-style:solid;");
		fprintf(archivo,"border-width:1px;overflow:hidden;word-break:normal;border-color:#A4A4A4;color:#333;background-color:#f0f0f0;}\n");
		fprintf(archivo,".estiloco .title{font-weight:bold;font-size:18px;vertical-align:middle;}\n.estiloco .review_spoiler{text-align:justify;vertical-align:top;}\n");
		fprintf(archivo,".estiloco .note{font-style:italic;font-size:13px;vertical-align:top;text-align:justify;}\n.estiloco .mini{vertical-align:top;font-size:13px;}\n");
		fprintf(archivo,".estiloco .platforms{font-style:italic;vertical-align:top;text-align: center;}\nh3.post-title {display:none;}\np{text-align: justify;}\n</style>\n");
		fprintf(archivo,"<script type=\"text/javascript\">\nfunction toggle_visibility(id){var e = document.getElementById(id); ");
		fprintf(archivo,"if(e.style.display == \'none\') e.style.display = \'block\'; else e.style.display = \'none\';}\n</script>");
		fprintf(archivo,"\n</head>\n<h1 style=\"text-align: center; font-size: x-large;\">\n<b>Juegos completados</b>\n</h1>\n");
		fprintf(archivo,"<p style=\"text-align: justify;\">\n");
		fprintf(archivo,"Description text. This is generated inside the program, with no possibility of changing it unless you have the source code. ");
		fprintf(archivo,"\n</p>\n");
		while(temporal != NULL){
			id++;
			fprintf(archivo,"<table class=\"estiloco\"><tr><th colspan=\"2\" class=\"title\">%s</th></tr>\n",temporal->nombre);										/* Titulo */
			fprintf(archivo,"<tr><td colspan=\"2\"><img src=\"%s\" style=\"max-width: 100%%; height: auto;\"/></td></tr>\n",temporal->link);						/* Imagen */
			fprintf(archivo,"<tr><td colspan=\"2\"class=\"review_spoiler\">%s</td></tr>\n",temporal->opinion);														/* Opinion */
			if(strcmp(temporal->spoiler,"*") != 0){
				fprintf(archivo,"<tr><td colspan=\"2\" class=\"review_spoiler\"><center><button onclick=\"toggle_visibility(\'%d\')\">Mostrar/Ocultar Spoiler",id);	/* Spoiler 1/2 */
				fprintf(archivo,"</button></center><span id=\"%d\" style=\"display:none; margin-top:5px;\">%s</span></td></tr>\n",id,temporal->spoiler);			/* Spoiler 2/2 */
			}
			if(strcmp(temporal->nota,"*") != 0)
				fprintf(archivo,"<tr><td colspan=\"2\" class=\"note\">%s</td></tr>\n",temporal->nota);																/* Nota */
			fprintf(archivo,"<tr><td class=\"mini\">Completado en el \'year\': <b>%d</b></td>",temporal->year);														/* Year */
			fprintf(archivo,"<td class=\"mini\">Porcentaje completado: <b>%d%%</b></td></tr>\n",temporal->percentage);												/* Porcentaje */
			fprintf(archivo,"<tr><td colspan=\"2\" class=\"platforms\">Jugado en: <b>%s</b></td></tr></table></br>\n\n",temporal->platforms);						/* Plataformas */
			temporal = temporal->sig;
		}
		fclose(archivo);
		flag = true;
	}
	return flag;
}

void initialText(){
	printf(" |--------------------------------------------|\n");
	printf(" |    YOU SHOULD ALWAYS RUN THIS PROGRAM IN   |\n");
	printf(" |          ADMINISTRATOR MODE, DUDE!.        |\n");
	printf(" |--------------------------------------------|\n");
	system("pause");
}

/* Original version
void initialText(){
	printf(" |--------------------------------------------|\n");
	printf(" |  ESTE PROGRAMA SIEMPRE SE EJECUTA EN MODO  |\n");
	printf(" |        ADMINISTRADOR. ATENTO MORRO!.       |\n");
	printf(" |--------------------------------------------|\n");
	system("pause");
}
*/

int menu(){
	char num[5];
	int op;
	printf(" |--------------------------------------------|\n");
	printf(" |  Menu para generar tablas de juegos XD     |\n");
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

elemento *USERinsertarElemento(elemento *lista){
	char nombre[N], link[L], opinion[E], spoiler[E], nota[E], platforms[N], numTemp[5], opTemp[5];
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
	}while(year < 1999 || year > 2019);
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
		lista = insertarElemento(lista, nombre, link, opinion, spoiler, nota, year, percentage, platforms);
		system("cls");
		printf(" ! Se ha insertado el elemento correctamente.\n");
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
	
	return lista;
}

elemento *USEReliminarElemento(elemento *lista){
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

void USEReditarElemento(elemento *lista){
	elemento *juego;
	char nombre[N], link[L], opinion[E], spoiler[E], nota[E], platforms[N],numTemp[5], opTemp[5];
	int op ,year, percentage;
	bool flag1 = true, flag2;
	
	printf(" | Has elegido 'Editar un elemento' |\n");
	printf(" > Ingresa el nombre del elemento (juego): ");
	leerString(nombre,N,'\n');
	if((juego = buscarElemento(lista, nombre)) != NULL){
		do{
			system("cls");
			printf(" || - Nombre: %s\n\n",juego->nombre);
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
						strcpy(juego->nombre, nombre);
						system("cls");
						printf(" !! Se ha cambiado el nombre correctamente.\n");
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
						strcpy(juego->link, link);
						system("cls");
						printf(" !! Se ha cambiado el enlace correctamente.\n");
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
						strcpy(juego->opinion, opinion);
						system("cls");
						printf(" !! Se ha cambiado la opinion correctamente.\n");
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
						if(!strcmp(juego->spoiler,"*"))
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
								if(!strcmp(juego->spoiler,"*")){
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
										strcpy(juego->spoiler, spoiler);
										system("cls");
										printf(" !!! Se ha agregado el spoiler correctamente.\n");
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
								if(strcmp(juego->spoiler,"*") != 0){								
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
										strcpy(juego->spoiler, spoiler);
										system("cls");
										printf(" !!! Se ha cambiado el spoiler correctamente.\n");
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
								if(strcmp(juego->spoiler,"*") != 0){
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
										strcpy(juego->spoiler, "*");
										system("cls");
										printf(" !!! Se ha borrado el spoiler correctamente.\n");
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
					if(!strcmp(juego->nota,"*"))
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
						if(!strcmp(juego->nota,"*")){
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
								strcpy(juego->nota, nota);
								system("cls");
								printf(" !!! Se ha agregado la nota correctamente.\n");
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
						if(strcmp(juego->nota,"*") != 0){								
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
								strcpy(juego->nota, nota);
								system("cls");
								printf(" !!! Se ha cambiado la nota correctamente.\n");
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
						if(strcmp(juego->nota,"*") != 0){
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
								strcpy(juego->nota, "*");
								system("cls");
								printf(" !!! Se ha borrado la nota correctamente.\n");
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
						juego->year = year;
						system("cls");
						printf(" !! Se ha cambiado el 'year' correctamente.\n");
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
						juego->percentage = percentage;
						system("cls");
						printf(" !! Se ha cambiado el porcentaje correctamente.\n");
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
						strcpy(juego->platforms, platforms);
						system("cls");
						printf(" !! Se han cambiado las plataformas correctamente.\n");
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

void USERgenerarHTML(elemento *lista){
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

void USERguardarElementos(elemento *lista){
	FILE *binario;
	elemento *temporal;
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
		if((binario = fopen("lista de juegos.backup","wb")) != NULL){
			temporal = lista;
			cantidad = contarElementos(lista);
			fwrite(&cantidad,sizeof(int),1,binario);
			while(temporal != NULL){
				fwrite(temporal,sizeof(elemento),1,binario);
				temporal = temporal->sig;
			}
			fclose(binario);
			printf(" ! Se han guardado los datos correctamente.\n");
		}else printf(" ! Hubo un error al crear el archivo.\n");
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
}

elemento *USERcargarElementos(elemento *lista){
	FILE *binario;
	elemento *temporal;
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
		if((binario = fopen("lista de juegos.backup","rb")) != NULL){
			temporal = (elemento *)malloc(sizeof(elemento));
			while(lista != NULL){
				temporal = lista;
				lista = lista->sig;
				free(temporal);
			}
			fread(&cantidad,sizeof(int),1,binario);
			for(i = 0; i < cantidad; i++){
				fread(temporal,sizeof(elemento),1,binario);
				lista = insertarElemento(lista, temporal->nombre, temporal->link, temporal->opinion, temporal->spoiler, temporal->nota, temporal->year, temporal->percentage, temporal->platforms);
			}
			fclose(binario);
			printf(" ! Se han cargado los datos correctamente.\n");
		}else printf(" ! Hubo un error al cargar el archivo.\n");
	}else{
		system("cls");
		printf(" ! Se ha cancelado la operacion.\n");
	}
	return lista;
}

int main(){
	elemento *lista;
	bool flag = true;
	lista = NULL;
	
	system("cls");
	initialText();
	system("cls");
	do{
		switch(menu()){
			case 1:
				system("cls");
				lista = USERinsertarElemento(lista);
				getchar();
				system("cls");
			break;
			case 2:
				system("cls");
				if(lista == NULL) printf(" ! No hay elementos ingresados.\n");
				else lista = USEReliminarElemento(lista);
				getchar();
				system("cls");
			break;
			case 3:
				system("cls");
				if(lista == NULL){
					printf(" ! No hay elementos ingresados.\n");
					getchar();
				}
				else USEReditarElemento(lista);
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
				if(lista != NULL) USERguardarElementos(lista);
				else printf(" ! No hay elementos ingresados.\n");
				getchar();
				system("cls");
			break;
			case 6:
				system("cls");
				lista = USERcargarElementos(lista);
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
	
	system("pause");
	return 0;
}
