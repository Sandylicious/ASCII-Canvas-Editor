/**
 * Ce programme permet d'initialiser et d'afficher un canevas ASCII sur un terminal. Le canevas peut etre cree par 
 * l'utilisateur ou provenant d'un fichier lu sur l'entree standard (stdin).
 * 
 * @author Fang, Xin Ran
 * @version 2021-10-10
 * */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define TAILLE 4
#define MAX_HEIGHT 40
#define MAX_WIDTH 80
#define BUFFER_SIZE (MAX_HEIGHT * MAX_WIDTH)

#define OPTION_TIRET_N "-n"
#define OPTION_TIRET_S "-s"
#define OPTION_TIRET_K "-k"
#define OPTION_TIRET_P "-p"
#define OPTION_TIRET_H "-h"
#define OPTION_TIRET_V "-v"
#define OPTION_TIRET_R "-r"
#define OPTION_TIRET_L "-l"
#define OPTION_TIRET_C "-c"

#define RESET	 "\x1B[0m"
#define NOIR  	 "\x1B[40m" 
#define ROUGE 	 "\x1B[41m" 
#define VERT  	 "\x1B[42m" 
#define JAUNE 	 "\x1B[43m" 
#define BLEU  	 "\x1B[44m" 
#define MAGENTA  "\x1B[45m"
#define CYAN  	 "\x1B[46m"
#define BLANC 	 "\x1B[47m"

#define USAGE "\
Usage: %s [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]\n\
          		   [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]\n\
          		   [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]\n\
Draws on an ASCII canvas. The canvas is provided on stdin and\n\
the result is printed on stdout. The dimensions of the canvas\n\
are limited to at most 40 rows and at most 80 columns.\n\
\n\
If no argument is provided, the program prints this help and exit.\n\
\n\
Canvas options:\n\
  -n HEIGHT,WIDTH           Creates a new empty canvas of HEIGHT rows and\n\
                            WIDTH columns. Should be used as first option,\n\
                            otherwise, the behavior is undefined.\n\
                            Ignores stdin.\n\
  -s                        Shows the canvas and exit.\n\
  -k                        Enables colored output. Replaces characters\n\
                            between 0 and 9 by their corresponding ANSI\n\
                            colors:\n\
                              0: black  1: red      2: green  3: yellow\n\
                              4: blue   5: magenta  6: cyan   7: white\n\
\n\
Drawing options:\n\
  -p CHAR                   Sets the pen to CHAR. Allowed pens are\n\
                            0, 1, 2, 3, 4, 5, 6 or 7. Default pen\n\
                            is 7.\n\
  -h ROW                    Draws an horizontal line on row ROW.\n\
  -v COL                    Draws a vertical line on column COL.\n\
  -r ROW,COL,HEIGHT,WIDTH   Draws a rectangle of dimension HEIGHTxWIDTH\n\
                            with top left corner at (ROW,COL).\n\
  -l ROW1,COL1,ROW2,COL2    Draws a discrete segment from (ROW1,COL1) to\n\
                            (ROW2,COL2) with Bresenham's algorithm.\n\
  -c ROW,COL,RADIUS         Draws a circle centered at (ROW,COL) of\n\
                            radius RADIUS with the midpoint algorithm.\n\
"

#define MSG_ERREUR_OPTION_PIXEL_INCORRECT "Error: wrong pixel value %c\n"
#define MSG_ERREUR_OPTION_TIRET_S_TOO_HIGH "Error: canvas is too high (max height: 40)\n"
#define MSG_ERREUR_OPTION_TIRET_S_TOO_WIDE "Error: canvas is too wide (max width: 80)\n"
#define MSG_ERREUR_OPTION_INVALIDE "Error: unrecognized option\n"
#define MSG_ERREUR_MANQUE_ARG "Error: missing value with option %s\n"
#define MSG_ERREUR_VALEUR_INCORRECTE "Error: incorrect value with option %s\n"
#define MSG_ERREUR_CANVAS_FORME_INCORRECTE "Error: canvas should be rectangular\n"

#define NOM_PROGRAMME "./canvascii"

typedef struct canvas {
    char pixels[MAX_HEIGHT][MAX_WIDTH]; // A matrix of pixels
    unsigned int width;                 // Its width
    unsigned int height;                // Its height
    char pen;                           // The character we are drawing with
} canvas;

enum error {
    OK                         = 0, // Everything is ok
    ERR_WRONG_PIXEL            = 1, // Wrong pixel value in canvas
    ERR_CANVAS_TOO_HIGH        = 2, // Canvas is too high
    ERR_CANVAS_TOO_WIDE        = 3, // Canvas is too wide
    ERR_CANVAS_NON_RECTANGULAR = 4, // Canvas is non rectangular
    ERR_UNRECOGNIZED_OPTION    = 5, // Unrecognized option
    ERR_MISSING_VALUE          = 6, // Option with missing value
    ERR_WITH_VALUE             = 7  // Problem with value
};

void afficherCanvas(struct canvas newCanvas);
int *validerSiValeursNumeriques(char *arg, char *optionChoisie);
void validerSiNombresPositifs(int *arg, int nbrElementsAValider, char *optionChoisie);
void validerValeurHeightEtWidth(int *arg, int *height, int *width, char *optionChoisie);
struct canvas creerCanvasVide(int *height, int *width);
struct canvas tracerUneLigneHoriz(struct canvas canvasCree, int *argValide, char *optionChoisie);
struct canvas tracerUneLigneVerti(struct canvas canvasCree, int *arg, char *optionChoisie);
struct canvas tracerUnRectangle(struct canvas canvasCree, int *arg, char *optionChoisie);
struct canvas tracerUnSegment(struct canvas canvasCree, int *arg, char *optionChoisie);
struct canvas tracerUnCercle(struct canvas canvasCree, int *arg, char *optionChoisie);
struct canvas selectionnerUnCrayon(struct canvas canvasCree, int *arg, char *optionChoisie);
void afficherCanvasColore(struct canvas canvasCree);
struct canvas lireCanvasDuFichier();
void validerCanvasDuFichier(struct canvas canvasCree);

/**
 * Cette methode affiche le canevas cree.
 * 
 * @param newCanvas 	Le nouveau canevas qui vient d'etre cree
 * */
void afficherCanvas(struct canvas newCanvas)
{
	unsigned int i, j;

	for (i = 0; i < newCanvas.height; i++)
	{
		for (j = 0; j< newCanvas.width; j++)
		{
			printf("%c", newCanvas.pixels[i][j]);
		}
		printf("\n");
	}
}

/**
 * Cette methode affiche le canevas qui se trouve dans un fichier.
 * */
void afficherCanvasDuFichier()
{
	char ligne[BUFFER_SIZE + 1];
	FILE *fp = stdin;
 
    while (fgets(ligne, BUFFER_SIZE, fp) != NULL)	
	{
		printf("%s", ligne);
    }
}

/**
 * Cette methode verifie si les valeurs sont tout numeriques.
 *
 * @param  *arg Liste d'arguments a valider
 * @return      Liste d'arguments valides
 * */
int *validerSiValeursNumeriques(char *arg, char *optionChoisie)
{
	if (arg == NULL)
	{
		printf(MSG_ERREUR_MANQUE_ARG, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_MISSING_VALUE);
	}

	char delimiteur[] = ",";
	char *charEnVerification = strtok(arg, delimiteur);
	static int valeursNumeriques[TAILLE];
	int compteur = 0;

    while (charEnVerification != NULL)
    {
        if ( (*charEnVerification == '-' || isdigit(*charEnVerification)) && compteur < TAILLE )
    	{
    	    valeursNumeriques[compteur] = strtol(charEnVerification, &charEnVerification, 10);
    		
    		compteur++;
    	}
    	
    	charEnVerification = strtok(NULL, delimiteur);
    }
    
	return valeursNumeriques;
}

/**
 * Cette methode verifie si les nombres sont tout positifs.
 *
 * @param *arg 		Liste d'arguments a valider
 * @param nbrElementsAValider  Chiffre qui indique le nombre d'elements a valider
 * @param *optionChoisie	Option choisie
 * */
void validerSiNombresPositifs(int *arg, int nbrElementsAValider, char *optionChoisie)
{
	int i;

	for (i = 0; i < nbrElementsAValider; i++)
	{
		if (arg[i] < 0)
		{
			printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
			printf(USAGE, NOM_PROGRAMME);
			exit(ERR_WITH_VALUE);
		}
	}
}

/**
 * Cette methode verifie si le longueur et la largeur du canevas cree respectent les condtions.
 *
 * @param *arg 		Liste d'arguments a valider
 * @param *height   		Hauteur du canevas
 * @param *width		Largeur du canevas
 * @param *optionChoisie	Option choisie
 * */
void validerValeurHeightEtWidth(int *arg, int *height, int *width, char *optionChoisie)
{
	*height = arg[0], *width = arg[1];

	if (*height == 0 || *height > MAX_HEIGHT || *width == 0 || *width > MAX_WIDTH)
	{
		printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_WITH_VALUE);
	}
}

/**
 * Cette methode cree un nouveau canevas.
 *
 * @param *height   	Hauteur du canevas
 * @param *width	Largeur du canevas
 **/
struct canvas creerCanvasVide(int *height, int *width)
{
	const char CRAYON_PAR_DEFAUT = '7', PIXEL_VIDE = '.';

	canvas newCanvas = { {{MAX_HEIGHT + 1, MAX_WIDTH + 1}}, *width, *height, CRAYON_PAR_DEFAUT };

	unsigned int i, j;

	for (i = 0; i < newCanvas.height; i++)
	{
		for (j = 0; j < newCanvas.width; j++)
			newCanvas.pixels[i][j] = PIXEL_VIDE;
	}

	return newCanvas;
}

/**
 * Cette methode trace une ligne horizontale dans le canevas cree.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * @param  *argValide	        Arguments qui sont deja valides
 * @param  *optionChoisie	Option choisie
 * @return 			Canevas modifie
 * */
struct canvas tracerUneLigneHoriz(struct canvas canvasCree, int *argValide, char *optionChoisie)
{
	unsigned int i, valeurDeLigne = argValide[0];

	if (valeurDeLigne >= canvasCree.height)
	{
		printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_WITH_VALUE);
	}	

	for (i = 0; i < canvasCree.width; i++)
	{
		canvasCree.pixels[valeurDeLigne][i] = canvasCree.pen;
	}

	return canvasCree;
}

/**
 * Cette methode trace une ligne verticale dans le canevas cree.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * @param  *arg  		Arguments de l'option
 * @param  *optionChoisie	Option choisie
 * @return canvasCree		Canevas modifie
 * */
struct canvas tracerUneLigneVerti(struct canvas canvasCree, int *arg, char *optionChoisie)
{
	unsigned int i, valeurDeLigne = arg[0];
	
	if (valeurDeLigne >= canvasCree.width)
	{
		printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_WITH_VALUE);
	}

	for (i = 0; i < canvasCree.height; i++)
	{
		canvasCree.pixels[i][valeurDeLigne] = canvasCree.pen;
	}

	return canvasCree;
}

/**
 * Cette methode trace un rectangle dans le canevas cree.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * @param  *arg  		Arguments de l'option
 * @param  *optionChoisie	Option choisie
 * @return canvasCree		Canevas modifie
 * */
struct canvas tracerUnRectangle(struct canvas canvasCree, int *arg, char *optionChoisie)
{
	int i, j, valeurDeRangee = arg[0], valeurDeColonne = arg[1], 
		hauteurDuRectangle = arg[2], largeurDuRectangle = arg[3];

	if (hauteurDuRectangle < 0 || largeurDuRectangle < 0)
	{
		printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_WITH_VALUE);
	}

	for (i = valeurDeRangee; i < hauteurDuRectangle + valeurDeRangee; i++)
	{
		for (j = valeurDeColonne; j < largeurDuRectangle + valeurDeColonne; j++)
		{
			if (valeurDeRangee >= 0 && j > 0)
				canvasCree.pixels[valeurDeRangee][j] = canvasCree.pen;
			
			if (valeurDeRangee + hauteurDuRectangle - 1 >= 0 && j >= 0)
				canvasCree.pixels[valeurDeRangee + hauteurDuRectangle - 1][j] = canvasCree.pen;		
		}

		if (i >= 0 && valeurDeColonne >= 0)
			canvasCree.pixels[i][valeurDeColonne] = canvasCree.pen;
	
		if (i >= 0 && valeurDeColonne + largeurDuRectangle - 1 >= 0)
			canvasCree.pixels[i][valeurDeColonne + largeurDuRectangle - 1] = canvasCree.pen;	
	}
	
	return canvasCree;
}

/**
 * Cette methode trace un segment dans le canevas cree.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * @param  *arg  		Arguments de l'option
 * @param  *optionChoisie	Option choisie
 * @return canvasCree		Canevas modifie
 * */
struct canvas tracerUnSegment(struct canvas canvasCree, int *arg, char *optionChoisie)
{
	int valeurDepartSurOrdonnee = arg[0], valeurDepartSurAbscisse = arg[1], 
		valeurFinaleSurOrdonnee = arg[2], valeurFinaleSurAbscisse = arg[3];

    int distanceSurAbscisse =  abs(valeurFinaleSurAbscisse - valeurDepartSurAbscisse);
    int valeurTemp1 = valeurDepartSurAbscisse < valeurFinaleSurAbscisse ? 1 : -1;
    int distanceSurOrdonnee = - abs(valeurFinaleSurOrdonnee - valeurDepartSurOrdonnee);
    int valeurTemp2 = valeurDepartSurOrdonnee < valeurFinaleSurOrdonnee ? 1 : -1;
    int valeurTemp3 = distanceSurAbscisse + distanceSurOrdonnee; 
    int valeurTemp4 = 0;
	int i = 0;
	int valeurTempSurAbscisse = valeurDepartSurAbscisse, valeurTempSurOrdonnee = valeurDepartSurOrdonnee;

	for (i = 0; i < distanceSurAbscisse; i++)
	{
		if ( (valeurDepartSurAbscisse == valeurFinaleSurAbscisse) && (valeurDepartSurOrdonnee == valeurFinaleSurOrdonnee) ) 
		{
			printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
			printf(USAGE, NOM_PROGRAMME);
			exit(ERR_WITH_VALUE);
		}

		if (valeurTempSurOrdonnee >= 0 && valeurTempSurAbscisse >= 0)
			canvasCree.pixels[valeurTempSurOrdonnee][valeurTempSurAbscisse] = canvasCree.pen;

        	valeurTemp4 = 2 * valeurTemp3;

        if (valeurTemp4 >= distanceSurOrdonnee)
        { 
			valeurTemp3 += distanceSurOrdonnee;
            valeurTempSurAbscisse += valeurTemp1;
		}
        
        if (valeurTemp4 <= distanceSurAbscisse)
        {
			valeurTemp3 += distanceSurAbscisse;
            valeurTempSurOrdonnee += valeurTemp2;
		}
	}

	return canvasCree;
}

/**
 * Cette methode trace un cercle dans le canevas cree.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * @param  *arg  		Arguments de l'option
 * @param  *optionChoisie	Option choisie
 * @return canvasCree		Canevas modifie
 * */
struct canvas tracerUnCercle(struct canvas canvasCree, int *arg, char *optionChoisie)
{
	int valeurDeRangee = arg[0], valeurDeColonne = arg[1], rayon = arg[2];
	
	if (rayon < 0)
	{
		printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_WITH_VALUE);
	}
	
	int valeurTemp1 = 0, valeurTemp2 = rayon;
    int valeurTemp3 = 3 - 2 * rayon;

	if (valeurDeColonne >= 0)
	{
		canvasCree.pixels[valeurDeColonne][valeurDeRangee + valeurTemp2] = canvasCree.pen;

		if (valeurDeRangee >= valeurTemp2)
		{
			canvasCree.pixels[valeurDeColonne][valeurDeRangee - valeurTemp2] = canvasCree.pen;
		}
	}
	
    if (valeurDeRangee >= 0)
	{
		canvasCree.pixels[valeurDeColonne + valeurTemp2][valeurDeRangee] = canvasCree.pen;

		if (valeurDeColonne >= valeurTemp2)
		{
			canvasCree.pixels[valeurDeColonne - valeurTemp2][valeurDeRangee] = canvasCree.pen;
		}		
	}

	while (valeurTemp2 >= valeurTemp1)
    {
        valeurTemp1++;
 
        if (valeurTemp3 > 0)
        {
            valeurTemp2--;
            valeurTemp3 = valeurTemp3 + 4 * (valeurTemp1 - valeurTemp2) + 10;
        }
        else
        {
			valeurTemp3 = valeurTemp3 + 4 * valeurTemp1 + 6;
		}
        
		if (valeurDeColonne + valeurTemp1 >= 0 && valeurDeRangee + valeurTemp2 >= 0)
			canvasCree.pixels[valeurDeColonne + valeurTemp1][valeurDeRangee + valeurTemp2] = canvasCree.pen;

		if (valeurDeColonne >= valeurTemp1 && valeurDeRangee + valeurTemp2 >= 0)
			canvasCree.pixels[valeurDeColonne - valeurTemp1][valeurDeRangee + valeurTemp2] = canvasCree.pen;
		
		if (valeurDeColonne + valeurTemp1 >= 0 && valeurDeRangee >= valeurTemp2)
			canvasCree.pixels[valeurDeColonne + valeurTemp1][valeurDeRangee - valeurTemp2] = canvasCree.pen;
		
		if (valeurDeColonne >= valeurTemp1 && valeurDeRangee > valeurTemp2)
			canvasCree.pixels[valeurDeColonne - valeurTemp1][valeurDeRangee - valeurTemp2] = canvasCree.pen;
		
		if (valeurDeColonne + valeurTemp2 >= 0 && valeurDeRangee + valeurTemp1 >= 0)
			canvasCree.pixels[valeurDeColonne + valeurTemp2][valeurDeRangee + valeurTemp1] = canvasCree.pen;
		
		if (valeurDeColonne >= valeurTemp2 && valeurDeRangee + valeurTemp1 >= 0)
			canvasCree.pixels[valeurDeColonne - valeurTemp2][valeurDeRangee + valeurTemp1] = canvasCree.pen;

		if (valeurDeColonne + valeurTemp2 >= 0 && valeurDeRangee >= valeurTemp1)
			canvasCree.pixels[valeurDeColonne + valeurTemp2][valeurDeRangee - valeurTemp1] = canvasCree.pen;
		
		if (valeurDeColonne >= 0 && valeurDeRangee >= valeurTemp1)
			canvasCree.pixels[valeurDeColonne - valeurTemp2][valeurDeRangee - valeurTemp1] = canvasCree.pen;
    }
	
	return canvasCree;
}

/**
 * Cette methode selectionne le crayon du canevas cree.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * @param  *arg  		Arguments de l'option
 * @param  *optionChoisie	Option choisie
 * @return canvasCree		Canevas modifie
 * */
struct canvas selectionnerUnCrayon(struct canvas canvasCree, int *arg, char *optionChoisie)
{
	int crayonChoisi = arg[0];

	if (crayonChoisi > 7)
	{
		printf(MSG_ERREUR_VALEUR_INCORRECTE, optionChoisie);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_WITH_VALUE);
	}

	canvasCree.pen = crayonChoisi + '0';
	
	return canvasCree;
}

/**
 * Cette methode affiche le canevas cree en couleur.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * */
void afficherCanvasColore(struct canvas canvasCree)
{
	unsigned int i, j;

	for (i = 0; i < canvasCree.height; i++)
	{
		for (j = 0; j < canvasCree.width; j++)
		{
			if (canvasCree.pixels[i][j] == '0')
				printf(NOIR " " RESET);
			
			if (canvasCree.pixels[i][j] == '1')
				printf(ROUGE " " RESET);
			
			if (canvasCree.pixels[i][j] == '2')
				printf(VERT " " RESET);
			
			if (canvasCree.pixels[i][j] == '3')
				printf(JAUNE " " RESET);
			
			if (canvasCree.pixels[i][j] == '4')
				printf(BLEU " " RESET);
			
			if (canvasCree.pixels[i][j] == '5')
				printf(MAGENTA " " RESET);
			
			if (canvasCree.pixels[i][j] == '6')
				printf(CYAN " " RESET);
			
			if (canvasCree.pixels[i][j] == '7')
				printf(BLANC " " RESET);
			
			if (canvasCree.pixels[i][j] == '.')
				printf(" ");
		}

		printf("\n");
	}
}

/**
 * Cette methode lit le canevas dans un fichier.
 *
 * @return canvasCree		Canevas du fichier
 * */
struct canvas lireCanvasDuFichier()
{
	struct canvas newCanvas;
	char ligne[BUFFER_SIZE + 1];
	FILE *fp = stdin;
	unsigned int i = 0, j = 0;
	int ligneLongueur[MAX_WIDTH];

	while (fgets(ligne, BUFFER_SIZE, fp) != NULL)	
	{
		while (ligne[j] != '\0')
		{
			newCanvas.pixels[i][j] = ligne[j];
			j++;
		}
		ligneLongueur[i] = j - 1;
		i++;
		newCanvas.width = j - 1;
		
		j = 0;
	}

	newCanvas.height = i;

	for (i = 0; i < newCanvas.height - 1; i++)
	{
		if (ligneLongueur[i] != ligneLongueur[i + 1])
		{
			printf(MSG_ERREUR_CANVAS_FORME_INCORRECTE);
			printf(USAGE, NOM_PROGRAMME);
			exit(ERR_CANVAS_NON_RECTANGULAR);
		}
	}
	
	newCanvas.pen = '7';

	return newCanvas;
}

/**
 * Cette methode valide le canevas provenant du fichier.
 *
 * @param  canvasCree 		Canevas qui vient d'etre cree
 * */
void validerCanvasDuFichier(struct canvas canvasCree)
{
	unsigned int i, j, k, compteurChar = 0;
	const unsigned int TAILLE_LISTE_CHAR_VALIDES = 10;
	char charValides[] = {'0', '1', '2', '3', '4', '5', '6', '7', '.', ' '};
	char charInvalide = ' ';
	
	if (canvasCree.height > MAX_HEIGHT)
	{
		printf(MSG_ERREUR_OPTION_TIRET_S_TOO_HIGH);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_CANVAS_TOO_HIGH);
	}
	else if (canvasCree.width > MAX_WIDTH)
	{
		printf(MSG_ERREUR_OPTION_TIRET_S_TOO_WIDE);
		printf(USAGE, NOM_PROGRAMME);
		exit(ERR_CANVAS_TOO_WIDE);
	}

	for (i = 0; i < canvasCree.height; i++)
	{
		for (j = 0; j < canvasCree.width; j++)
		{
			for (k = 0; k < TAILLE_LISTE_CHAR_VALIDES; k++)
			{
				if (canvasCree.pixels[i][j] != charValides[k])
				{
					compteurChar++;

					if (compteurChar >= TAILLE_LISTE_CHAR_VALIDES)
					{
						charInvalide = canvasCree.pixels[i][j];
						printf(MSG_ERREUR_OPTION_PIXEL_INCORRECT, charInvalide);
						printf(USAGE, NOM_PROGRAMME);
						exit(ERR_WRONG_PIXEL);
					}
				}
			}

			k = 0;
			compteurChar = 0;
		}
	}
}

int main(int argc, char *argv[])
{
	if (argc == 1)
	{
		printf(USAGE, argv[0]);

		return 0;
	}

	int i, nbrElementsAValider = 0, height = 0, width = 0;
	struct canvas newCanvas;
	int *argValides;

	for (i = 1; i < argc; i++)
	{
		char *prochainArg = argv[i + 1];

		if ( (strcmp(argv[i], OPTION_TIRET_N)) == 0 )
		{
			argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			nbrElementsAValider = 2;

			validerSiNombresPositifs(argValides, nbrElementsAValider, argv[i]);
			validerValeurHeightEtWidth(argValides, &height, &width, argv[i]);

			newCanvas = creerCanvasVide(&height, &width);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_S)) == 0 )
		{
			newCanvas = lireCanvasDuFichier();
			validerCanvasDuFichier(newCanvas);
			break;
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_H)) == 0 )
		{	
			argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			nbrElementsAValider = 1;

			validerSiNombresPositifs(argValides, nbrElementsAValider, argv[i]);

			newCanvas = tracerUneLigneHoriz(newCanvas, argValides, argv[i]);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_V)) == 0 )
		{
			argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			nbrElementsAValider = 1;

			validerSiNombresPositifs(argValides, nbrElementsAValider, argv[i]);

			newCanvas = tracerUneLigneVerti(newCanvas, argValides, argv[i]);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_R)) == 0 )
		{
		 	argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			newCanvas = tracerUnRectangle(newCanvas, argValides, argv[i]);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_L)) == 0 )
		{
			argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			newCanvas = tracerUnSegment(newCanvas, argValides, argv[i]);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_C)) == 0 )
		{
			argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			newCanvas = tracerUnCercle(newCanvas, argValides, argv[i]);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_P)) == 0 )
		{
			argValides = validerSiValeursNumeriques(prochainArg, argv[i]);
			nbrElementsAValider = 1;

			validerSiNombresPositifs(argValides, nbrElementsAValider, argv[i]);

			newCanvas = selectionnerUnCrayon(newCanvas, argValides, argv[i]);
		}
		else if ( (strcmp(argv[i], OPTION_TIRET_K)) == 0 ) 
		{
			if (argc == 2)
			{
				newCanvas = lireCanvasDuFichier();
				validerCanvasDuFichier(newCanvas);
			}
			
			afficherCanvasColore(newCanvas);

			return 0;
		}
	}

	afficherCanvas(newCanvas);

	return 0;	
}
