#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <dirent.h>
#include <time.h>



//formato: Type_repetition_color
//colors= {blue, red, green, yellow}
//type={1-9, 0, +2, +4, back, jump, col}
//Special= black: can change the next color to a chosen one
//

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

struct playOut { 
    char* s; //skill
    char* c;//color
    char* name;//
}; 
  
typedef struct playOut playOut; 

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
	char* direction;
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
		direction = malloc (sizeof (char) * (strlen(sname)+strlen(cwd) +1));
		snprintf(direction, strlen(sname)+strlen(cwd)+2, "%s%s%s", cwd, sname,slash);


	}

	else{//addSlash==0
		//merge the strings
		direction = (char *) malloc (sizeof (char) * (strlen(sname)+strlen(cwd)));
		snprintf(direction, strlen(sname)+strlen(cwd)+1, "%s%s", cwd, sname);

	}
	//printf("just before passing%s, %li\n",direction,strlen(direction) );
	return direction;


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

char *GetTxtDirection(char *TextName,char *Foldername,int flag){
	//printf("texttanme is %s and it length is %li\n",TextName, strlen(TextName) );
	char* direction;
	char *path= GetDirection(Foldername, 1);
	char *tex= ".txt";

	//printf("dentro de gettxt %s, %s, %s\n",TextName, Foldername, path );

	if (flag==1){	
		direction = (char *) malloc (sizeof (char) * (strlen(TextName)+strlen(path)+strlen(tex)));
		snprintf(direction, strlen(TextName)+strlen(path)+strlen(tex)+2, "%s%s%s", path, TextName,tex);


	}
	else{

		direction = (char *) malloc (sizeof (char) * (strlen(TextName)+strlen(path)));
		snprintf(direction, strlen(TextName)+strlen(path)+1, "%s%s", path, TextName);
	}
	//printf("en get txt direccion %s y also %li\n",direction, strlen(direction) );
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
	char* direct1=GetTxtDirection(TextName,Foldername,1);
	//printf("%s \n",direct1 );
	FILE *OutFile = fopen(direct1,"w");

	//this can be adjusted to fill the tex with a desired tex in the future
	fprintf(OutFile,"This is a blue card");

	fclose(OutFile);
	//free(direct1);  //explota la creacion del mazo
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

void DeleteFolder(char *name) {
	char *outnam= GetDirection(name, 0);
	
	rmdir(outnam);

	free(outnam);
}




/*
MoveText
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
	//printf("inputs %s, %s, %s\n",origin,destiny,name );

	char *oldpath= GetTxtDirection(name, origin,0);
	char *newpath=GetTxtDirection(name, destiny,0);

	//printf("old path%s incio %c en posicion %li\n ",oldpath,oldpath[56], strlen(oldpath) );
	//printf("new path%s incio %c en posicion %li\n ",newpath,newpath[56], strlen(newpath) );

	//printf("new path%s\n", newpath);
	//printf("%s\n",oldpath );
	//printf("%s\n",newpath );
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
		char str[100]="";
		strcat(str,tipo[j]);
		strcat(str,"_");
		strcat(str,color[i]);
		strcat(str,"_");

		char num[6];
		sprintf(num,"%i",carta);
		strcat(str,num);
		if (strlen(str)==(15))
		{
			strcat(str,"0");

		}


		CreateTexSomewhere(str,Foldername);

		//sacamos una carta
		mazo[i][j]-=1;
	}
}

/*
RevisarCarpeta
inputs:
	Foldername= nombre de la carpeta donde leer

Funcion:
	lee todas las cartas de una carpeta
Retorno:
	no retorna nada
*/

void RevisarCarpeta(char *Foldername){
	DIR *d;
    struct dirent *dir;
    char* path=GetDirection(Foldername, 1);
    d = opendir(path);
    int i=0;
    if (d){
        while ((dir = readdir(d)) != NULL){
        	if (strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0){
            	//printf("%s\n",dir->d_name );
        	
            	i+=1;
            	//printf("el largo del str %s es %li \n",dir->d_name,strlen(dir->d_name) );
            	if (strlen(dir->d_name)==19)

            	{
            		printf("F\n");
            	}

            	//char *token = strtok(dir->d_name,"_");
            	//printf("%s\n",token);//Token guarda el numero de la carta
            	//token = strtok(NULL,"_");
            	//printf("%s\n",token);//Token guarda el color de la carta
        	}
        }
        closedir(d);
    }
    printf("%d\n", i);
}

/*
robar carta
inputs:
	jugador= string del jugador

Funcion:
	saca una carta de mazo y la pone en jugador
Retorno:
	no retorna nada
*/

char* robarCarta(char *jugador){
	DIR *d;
    struct dirent *dir;
    char* path=GetDirection("mazo", 1);
    char* robada;
    //printf("robar carta dir mazo ==%s\n",path );
    d = opendir(path);
    if (d){
    	while ((dir = readdir(d)) != NULL){
    		if (strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0){
    		robada=dir->d_name;
			moveTex("mazo", jugador,dir->d_name);

        	break;
        }
    }

        closedir(d);

    }
    return robada;
}



char* concatenar(char* a, char* b){

	char* new = (char *) malloc (sizeof (char) * (strlen(a)+strlen(b)));
	snprintf(new, strlen(a)+strlen(b)+1, "%s%s", a, b);
	return new;
}


void manoInicial(){
	char*  jugador="jugador";
	int i=1;
	int j=0;
	char* current;
	char str[10];
	while(i<5){
		sprintf(str, "%d", i);
		current=concatenar(jugador, str);
		i++;
		printf("%s\n",current );
		while(j<7){
			robarCarta(current);
			j++;
		}
		j=0;


	}

}

void verMano(char *Foldername){
	DIR *d;
    struct dirent *dir;
    char* path=GetDirection(Foldername, 1);
    d = opendir(path);
    int i=0;
    if (d){
        while ((dir = readdir(d)) != NULL){
        	if (strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0){
            	//printf("%s\n",dir->d_name );
        	
            	i+=1;
            	//printf("el largo del str %s es %li \n",dir->d_name,strlen(dir->d_name) );
            

            	char *token = strtok(dir->d_name,"_");//numero o habilidad
            	char* token1 = strtok(NULL,"_");//color

            	printf("%i.- la carta %s %s\n",i,token, token1);//Token guarda el numero de la carta
        	}
        }
        closedir(d);
    }
    //printf("%d\n", i);
}







	//Se crean los nombres que representaran al juego en su totalidad
	
//char *mazo = "mazo";
	/*char *mazo = "mazo";
	char *mesa = "mesa";
	char *jugador1 = "jugador1";
	char *jugador2 = "jugador2";
	char *jugador3 = "jugador3";
	char *jugador4 = "jugador4";

	//Se crean las carpetas asociadas a dichos nombres
	CreateFolder(mazo);
	CreateFolder(mesa);
	CreateFolder(jugador1);
	CreateFolder(jugador2);
	CreateFolder(jugador3);
	CreateFolder(jugador4);

	//Se crea el mazo de forma aleatoria

	CreateMazo(mazo);
	//RevisarCarpeta(mazo);

	//int exit=1;
	//int cartas=0;

	RevisarCarpeta(mazo);*/

	///char* archivo="+2_amarillo_65";
	//char* carpeta_original="mazo";
	//char* carpeta_nueva="jugador1";

    //moveTex(carpeta_original,carpeta_nueva,archivo);

	//RevisarCarpeta(mazo);
	//printf("%li\n",strlen("reversa_rojo_92.txt") );	
	//printf("%li\n",strlen("reversa_rojo_92") );


	//robarCarta("jugador3");
	//CreateMazo("mazo");
	//RevisarCarpeta("mazo");


	/*
	do{
		printf("%s\n", "hola");

	}while(exit!=1 || cartas!=108);
	*/
	//manoInicial();
	//verMano("jugador3");


	//int i=1;

	//crear carpetas, llenar mazo, robar iniciales

/*
pipes
int main(){
	//todas las pipes necesarias para comunicarse en ambas direcciones entre los 4 procesos
	int P1[2],P11[2],P2[2],P22[2],P3[2],P33[2];
    pipe(P1);pipe(P11);pipe(P2);pipe(P22);pipe(P3);pipe(P33); 
    int pid;
    //id de jugador 
    int j=1;
    int game=1;
    char readbuff[256];
    char* here;
    char* from;



    pid=fork();

    if (pid==0)
    {
    	//operaciones del proceso 2
    	j=2;
    	here="en 2";
    	from="desde 2";
    	printf("TWO\n");
    	close(P1[1]);
    	close(P11[0]);
    	close(P2[1]);
    	close(P22[0]);
    	close(P3[1]);
    	close(P33[0]);
    	write(P11[1],from, strlen(from)+1);

    	

    }
    else{
    	pid=fork();
    	if (pid==0)
    	{

    		//operaciones del proceso 3
    		j=3;
    		here="en 3";
    		from="desde 3";
    		printf("TRHEE\n");
    		close(P1[0]);
    		close(P11[1]);
    		close(P2[1]);
    		close(P22[0]);
    		close(P3[0]);
    		close(P33[1]);
    		write(P1[1],from, strlen(from)+1);


    	}
    	else{
    		pid=fork();
    		if (pid==0)
    		{
    			//operaciones del proceso 4
    			j=4;
    			here="en 4";
    			from="desde 4";
    			printf("FOUR\n");
    			close(P1[1]);
    			close(P11[0]);
    			close(P2[0]);
    			close(P22[1]);
    			close(P3[1]);
    			close(P33[0]);
    			read(P1[0],readbuff, 20);

    			printf("%s %s\n",here, readbuff );

    		}
    		else{
    			//proceso padre (1)
    			printf("ONE\n");
    			here="en 1";
    			from="desde 1";
    			close(P1[0]);
    			close(P11[1]);
    			close(P2[0]);
    			close(P22[1]);
    			close(P3[0]);
    			close(P33[1]);
    			read(P11[0],readbuff,20 );
    			printf("%s %s\n",here, readbuff );





    		}
    	}
    }
*/
//juega la carta numero int carta de la mano de jugar y la pone en la mesa, elimiando la carta que habia en mesa
//retorna 0 si no se jugo la carta, retorna 1 si se jugo la carta



playOut getCarta(char* jugador, int carta){
	playOut P;
	DIR *d;
    struct dirent *dir;
    char* path=GetDirection(jugador, 1);
    d = opendir(path);
    int i=0;
    char* nuevaJugada;

    char* skill="";
    char* color="";    

    if (d){
        while ((dir = readdir(d)) != NULL){
        	if (strcmp(dir->d_name, ".")!=0 && strcmp(dir->d_name, "..")!=0){
            	//printf("%s\n",dir->d_name );
            	if((i+1)==carta || carta==-1){

            		nuevaJugada=(char *) malloc (sizeof (char) * (strlen(dir->d_name)+1));
            		strcpy(nuevaJugada,dir->d_name);

            		char* skil = strtok(dir->d_name,"_");//numero o habilidad
            		skill=(char *) malloc (sizeof (char) * (strlen(skil)+1));
            		strcpy(skill,skil);

            		char* colo = strtok(NULL,"_");//color
            		color=(char *) malloc (sizeof (char) * (strlen(colo)+1));
            		strcpy(color,colo);
            		break;

				}

        	
            	i+=1;
            	//printf("el largo del str %s es %li \n",dir->d_name,strlen(dir->d_name) );


            	//printf("%i.- la carta %s %s\n",i,token, token1);//Token guarda el numero de la carta
        	}
        }
        closedir(d);
       
    }




    P.s=skill;
    P.c=color;
    P.name=nuevaJugada;
    return P;
}

playOut makeOut(char* txt){
	char* nam=(char *) malloc (sizeof (char) * (strlen(txt)+1));
    strcpy(nam,txt);
	playOut P;
	char* skill = strtok(txt,"_");//numero o habilidad
	char*ski=(char *) malloc (sizeof (char) * (strlen(skill)+1));
	strcpy(ski,skill);

    char* color = strtok(NULL,"_");//color
    char*col=(char *) malloc (sizeof (char) * (strlen(color)+1));
   	strcpy(col,color);


    P.c=col;
    P.s=ski;
    P.name=nam;
    return P;



}


int validarJugada(playOut res){
	printf("color %s\n",res.c );
	playOut mesa=getCarta("mesa",-1);

	//1 si es valida 0 si no
	printf("color en la mesa %s color de la carta %s\n",mesa.c,res.c );
	printf("tipo en la mesa %s tipo de la carta %s\n",mesa.s,res.s );
	return (strcmp(mesa.c, res.c)==0 || strcmp(mesa.s, res.s)==0);

}
void borrar(char* carpeta, char* tex){
	char* path=GetTxtDirection(tex, carpeta, 0);
	//printf("%s\n",path );
	remove(path);
}

void remplazar(playOut res){
	playOut mesa= getCarta("mesa",-1);
	moveTex("jugador1","mesa",res.name);
	borrar("mesa", mesa.name);





}

#define BUF_SIZE 256


int main(){
	char *mazo = "mazo";
	char *mesa = "mesa";
	char *jugador1 = "jugador1";
	char *jugador2 = "jugador2";
	char *jugador3 = "jugador3";
	char *jugador4 = "jugador4";

	//Se crean las carpetas asociadas a dichos nombres
	CreateFolder(mazo);
	CreateFolder(mesa);
	CreateFolder(jugador1);
	CreateFolder(jugador2);
	CreateFolder(jugador3);
	CreateFolder(jugador4);
	CreateMazo(mazo);
	manoInicial();

	playOut resultado;

	int input;
	
	int game=1;
	verMano("jugador1");
	printf("no hay ninguna carta en la mesa\n");
	printf("que carta vas a jugar\n");
	scanf(" %i", &input);
	resultado=getCarta("jugador1", input);

	moveTex(jugador1,mesa,resultado.name);


	printf("%s %s %s\n",resultado.s, resultado.c, resultado.name );
	int turno=1;
	//int menu=1;
	char* robo;
	playOut rob;


	
	while(game){
		turno=1;

		while(turno){

		printf("-----------------------------------\n");
		printf("es tu turno jugador 1, que vas a hacer\n" );
		printf("1.-ver la mesa\n");
		printf("2.-ver mi mano\n");
		printf("3.-robar una carta\n");
		printf("4.-jugar una carta\n");
		printf("5.-acabar el juego\n");
		printf("-----------------------------------\n");
		scanf(" %i", &input);


		if (input==1)
		{
			verMano("mesa");
		}
		else if(input==2){
			verMano("jugador1");

		}
		else if(input==3){
			robo=robarCarta("jugador1");
			rob=makeOut(robo);
			printf("robaste la carta %s %s\n",rob.s, rob.c );
			if(strcmp(rob.c,"negro")==0)
			{
				printf("no esta implementado f\n");
			}
			else
			{
				if (validarJugada(rob))

				{
					printf("se puede jugar la carta que robaste\n" );
					printf("quieres jugarla?\n");
					printf("1.-si\n");
					printf("2.-no\n");
					scanf(" %i", &input);
					if(input==1){
						remplazar(rob);
					}
				}

				printf("no se puede jugar la carta robada, fin del turno\n");
				turno=0;
			}


		}
		else if(input==4){
			printf("que carta vas a jugar\n");
			verMano("jugador1");
			scanf(" %i", &input);
			resultado=getCarta("jugador1", input);
			if (validarJugada(resultado))
			{
				remplazar(resultado);
			}
			else
			{
				printf("esa carta no es valida\n");
			}






		}
		else if(input==5){
			printf("game over\n");
			game=0;
			turno=0;
		}
	}


	}
	
    }



