#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>


	/*

	//lee los archivos de un folder "mazo"
    DIR *d;

    struct dirent *dir;

    char* path=GetDirection("mazo", 1);

    d = opendir(path);

    if (d)

    {
        while ((dir = readdir(d)) != NULL){
        	if (strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0 ){
            printf("%s\n ", dir->d_name);
        	}
        }
        closedir(d);
    }*/



//formato: Type_repetition_color
//colors= {blue, red, green, yellow}
//type={1-9, 0, +2, +4, back, jump, col}
//Special= black: can change the next color to a chosen one
//

/*typedef struct carta{
   char *type;
   char *color;
};*/


/*
Goals entregable

----------------------------------------------------------
crear una carpeta con cualquier nombre:	       hecho
----------------------------------------------------------
funcion para crear todas 
las carpetas necesarias           		       no empezado
----------------------------------------------------------
crear un texto en una carpeta deseada:		   hecho
----------------------------------------------------------
funcion para crear el texto que debe           no empezado
tener una "carta" en su interior
----------------------------------------------------------
funcion para crear todas las cartas necesarias
en la carpeta mazo:							   no empezado
----------------------------------------------------------
borrar un archivo de una carpeta: 		       hecho
----------------------------------------------------------
mover un archivo de una carpeta a otra:        hecho
----------------------------------------------------------
*/


//no se que hace esto, solo se que en internet dice que sirve para ver si esta creadoel folder de antes
struct stat st = {0};

/*
CreateFolder
inputs:
	name= nombre de la que se quiere la direccion
	addSlash= agrega una slash extra para usar la direccion directamente a la hora de crear archivos 
Funcion:
	crea una direccion a una carpeta
Retorno:
	direction= un string con la direcion

ejemplos:
	si name=mazo y addSlash=1 retorna "sourcePath/mazo/"
	si name=mazo y addSlash=0 retorna "sourcePath/mazo"

*/


char * GetDirection(char *name, int addSlash){
	//get path
	char cwd[256];
	getcwd(cwd, sizeof(cwd));

	//add / to the name
	char sname[strlen(name)];
	char *slash= "/";
	snprintf(sname, strlen(name)+ strlen(slash)+1, "%s%s", slash, name);


	//merge the strings
	char outnam[strlen(sname)+strlen(cwd)];
	snprintf(outnam, strlen(sname)+strlen(cwd)+1, "%s%s", cwd, sname);

	if(addSlash==1){

		//merge the strings
		char *direction = malloc (sizeof (char) * (strlen(sname)+strlen(cwd) +1));
		snprintf(direction, strlen(sname)+strlen(cwd)+2, "%s%s%s", cwd, sname,slash);

		return direction;

	}

	else{//addSlash==0
		//merge the strings
		char *direction = (char *) malloc (sizeof (char) * (strlen(sname)+strlen(cwd)));
		snprintf(direction, strlen(sname)+strlen(cwd)+1, "%s%s", cwd, sname);
		return direction;

	}
}

/*
inputs:
	TextName= nombre del txt que se va a crear
	Foldername= nombre de la carpeta donde va
Funcion:
	genera la direcion que deberia tener el txt Text name para existir en la carpeta Foldername
Retorno:
	direction= un string con la direcion de txt dentro de folder


*/

char *GetTxtDirection(char *TextName,char *Foldername){

	char *path= GetDirection(Foldername, 1);
	char *tex= ".txt";

	char *direction = (char *) malloc (sizeof (char) * (strlen(TextName)+strlen(path)+strlen(tex)));

	snprintf(direction, strlen(TextName)+strlen(path)+strlen(tex)+2, "%s%s%s", path, TextName,tex);

	return direction;


}

/*
void CreateTexSomewhere
inputs:
	TextName= nombre que debe tener el archivo .txt
	Foldername= nombre de la carpeta donde va
Funcion:
	crea el archivo txt de nombre TextName en la carpeta Foldername
Retorno:
	no retorna

*/

void CreateTexSomewhere(char *TextName, char *Foldername){
	char* direct1=GetTxtDirection(TextName,Foldername);
	//printf("%s \n",direct1 );
	FILE *OutFile = fopen(direct1,"w");

	//this can be adjusted to fill the tex with a desired tex in the future
	fprintf(OutFile,"This is a blue card");

	fclose(OutFile);
	free(direct1);
}

/*
CreateFolder
inputs:
	name= nombre de la carpeta que se quiere crear
Funcion:
	crea una carpeta en el directorio actual con el nombre name
Retorno:
	no retorna nada
*/


void CreateFolder(char *name) {
	char *outnam= GetDirection(name, 0);
	
	//create the folder if it doesnt exist
	if (stat(outnam, &st) == -1){
		mkdir(outnam, 0700);
	}

	free(outnam);

}

/*
CreateFolder
inputs:
	name= nombre del texto a mover
	origin= carpeta donde se encuentra dicho texto
	destiny= carpeta a donde se quiere que termine el texto
	
Funcion:
	mueve un texto name de origin a destiny 
Retorno:
	no retorna nada
*/

void moveTex(char* origin, char* destiny, char* name){
	char *oldpath= GetTxtDirection(name, origin);
	char *newpath=GetTxtDirection(name, destiny);
	printf("%s\n",oldpath );
	printf("%s\n",newpath );
	rename(oldpath, newpath);
	free(oldpath);
	free(newpath);





}

/*
void deletTex
inputs:
	name= nombre del texto a borrar
	origin= carpeta donde se encuentra dicho texto
	
Funcion:
	borra un texto name de origin
Retorno:
	no retorna nada
*/

void deletTex(char* origin, char* name){
	char *path= GetTxtDirection(name, origin);
	//printf("%s\n",oldpath );
 	remove(path);
	free(path);






}
/*
void BuildFolders
inputs:
	no recibe
	
Funcion:
	crea las 6 carpetas necesarias para el programa 
	4 jugadores
	1 mazo
	1 carpeta de "ultima carta jugada"
Retorno:
	no retorna nada
*/

void BuildFolders(){

}

int main() {


	
	char *color;
	char *folder1;
	char *folder2;
	//nombre del txt
	color="azul";
	//nombre del folder

	folder1="mazo";
	folder2="Jugador1";
	//crea la carpeta
	//CreateFolder(folder);
	//crea el txt nombre color.txt dentro del folder
	//CreateTexSomewhere(color,folder);

	deletTex(folder2, color);







   
	return 0;
}
