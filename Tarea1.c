#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>


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

create a folder of any name:			   done
function to create every folder:           not started
create a text file in any folder:		   in progress
function to create every "carta" in "mazo":not started
delete a text file from a folder: 		   not started
move a text file from a folder to another: not started
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



int main() {

	//obtiene el dir actual

	
	char *name2;
	//nombre del folder
	name2="mazo";
	CreateFolder(name2);

	char* direct1= GetDirection("mazo",0);
	printf("%s \n",direct1 );

   
	return 0;
}
