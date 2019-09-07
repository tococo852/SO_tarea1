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
	name= nombre de la carpeta que se quiere crear
Funcion:
	crea una carpeta en el directorio actual con el nombre name
Retorno:
	no retorna nada
*/

void CreateFolder(char *name) {

	//get path
	char cwd[PATH_MAX];
	getcwd(cwd, sizeof(cwd));

	//add / to the name
	char sname[strlen(name)+1];
	char slash= '/';
	snprintf(sname, strlen(name)+1, "%c%s", slash, name);

	//merge the strings
	char outnam[strlen(sname)+strlen(cwd)];
	snprintf(outnam, strlen(sname)+strlen(cwd), "%s%s", cwd, sname);

	//create the folder if it doesnt exist
	if (stat(outnam, &st) == -1){
		mkdir(outnam, 0700);}

}


int main() {
	char *name2;
	//nombre del folder
	name2="ayur  aaaa";
	CreateFolder(name2);
   
	return 0;
}
