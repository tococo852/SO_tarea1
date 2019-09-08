#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <time.h>


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
create a folder of any name:			   done
function to create every folder:           not started
create a text file in any folder:		   done
function to create every "carta" in "mazo":not started
delete a text file from a folder: 		   in progress
move a text file from a folder to another: done
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
CreateMazo
inputs:
	Foldername= nombre de la carpeta donde va

Funcion:
	crea un mazo aleatoriamente
Retorno:
	no retorna nada
*/

void CreateMazo(char *Foldername){
	
	//formato de las cartas
	char *color[5]={"azul","rojo","verde","amarillo","negro"};
	char *tipo[15]={"0","1","2","3","4","5","6","7","8","9","+2","reversa","salto","colores","+4"}; 

	//representacion del mazo mediante una matriz
	int mazo[5][15]={0};
	int i,j;
	for(i=0;i<=4;i++){
		for (j=0;j<=14;j++){
			if((j==0) && (i!=4)){
				mazo[i][j]=1;
			}
			if((j>=1) && (j<=12) && (i!=4)){
				mazo[i][j]=2;
			}
			if ((i==4) && (j>12)){
				mazo[i][j]=4;
			}				
		}
	}

	//seleccionamos aleatoriamente sobre la matriz las distintas cartas
	srand(time(NULL));
	int carta=1;
	for(carta=1;carta<=108;carta++){
		do{
			i=rand()% 5;
			j=rand()% 15;
		}while(mazo[i][j]==0);

		//crear la carta
		char str[50]="";
		strcat(str,tipo[j]);
		strcat(str," ");
		strcat(str,color[i]);
		printf("%s\n",str);
		CreateTexSomewhere(str,Foldername);

		//sacamos una carta
		mazo[i][j]-=1;
	}	
}


int main() {

	//char *color[5]={"azul","rojo","verde","amarillo","negro"};
	char *folder1;
	//char *folder2;
	//nombre del txt
	//nombre del folder

	folder1="mazo";
	//folder2="Jugador1";
	//crea la carpeta
	
	CreateFolder(folder1);
	
	//crea el txt nombre color.txt dentro del folder
	

	//moveTex(folder1,folder2, color[1]);
	CreateMazo(folder1);

	return 0;
}
