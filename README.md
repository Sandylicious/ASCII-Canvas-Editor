# Dessiner sur un canevas ASCII

## Description

Ce programme permet d'initialiser et d'afficher un canevas ASCII sur un terminal. Le canvevas est composé de ```h``` lignes et de ```w``` longueur, il peut etre cree par l'utilisateur ou provenant d'un fichier, de format ```.canvas```, lu sur l'entree standard ```stdin```.

**Caractères acceptés**
* ```.``` représente un pixel non occupé
* ```0``` représente un pixel de couleur *noir*
* ```1``` représente un pixel de couleur *rouge*
* ```2``` représente un pixel de couleur *vert*
* ```3``` représente un pixel de couleur *jaune*
* ```4``` représente un pixel de couleur *bleu*
* ```5``` représente un pixel de couleur *magenta*
* ```6``` représente un pixel de couleur *cyan*
* ```7``` représente un pixel de couleur *blanc*

**Options supportée**
* ```-n```: Créer un canevas 
* ```-s```: Afficher le canevas
* ```-h```: Tracer une ligne horizontale dans le canevas
* ```-v```: Tracer une ligne verticale dans le canevas
* ```-r```: Tracer un rectangle dans le canevas
* ```-l```: Tracer un segment dans le canevas
* ```-c```: Tracer un cercle dans le canevas
* ```-p```: Sélectionner un crayon
* ```-k```: Colorer le canevas

**Codes de retour dans les différents cas**
* ```0```: Le programme fonctionne correctement
* ```1```: Détection d'un caractère invalide dans le canevas
* ```2```: La hauteur du canevas dépasse la limite (40)
* ```3```: La largeur du canevas dépasse la limite (80)
* ```4```: Le canevas n'est pas rectangulaire
* ```5```: Détection d'une option invalide
* ```6```: Manque de paramètres pour l'option
* ```7```: Valeur invalide


Un canevas est représenté par un fichier texte contenant `h` lignes de même
longueur `w`. Les seuls caractères acceptés sont les suivants:


Un exemple de canevas valide avec `h = 5`, `w = 8` et utilisant les 7 couleurs
est:

```
...7....
.0..2.1.
...3....
.4...5..
...6....
```

Lorsque vous lancez le programme sans argument, un manuel d'utilisation doit
être affiché sur la sortie standard:

```text
$ ./canvascii
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
Draws on an ASCII canvas. The canvas is provided on stdin and
the result is printed on stdout. The dimensions of the canvas
are limited to at most 40 rows and at most 80 columns.

If no argument is provided, the program prints this help and exit.

Canvas options:
  -n HEIGHT,WIDTH           Creates a new empty canvas of HEIGHT rows and
                            WIDTH columns. Should be used as first option,
                            otherwise, the behavior is undefined.
                            Ignores stdin.
  -s                        Shows the canvas and exit.
  -k                        Enables colored output. Replaces characters
                            between 0 and 9 by their corresponding ANSI
                            colors:
                              0: black  1: red      2: green  3: yellow
                              4: blue   5: magenta  6: cyan   7: white

Drawing options:
  -p CHAR                   Sets the pen to CHAR. Allowed pens are
                            0, 1, 2, 3, 4, 5, 6 or 7. Default pen
                            is 7.
  -h ROW                    Draws an horizontal line on row ROW.
  -v COL                    Draws a vertical line on column COL.
  -r ROW,COL,HEIGHT,WIDTH   Draws a rectangle of dimension HEIGHTxWIDTH
                            with top left corner at (ROW,COL).
  -l ROW1,COL1,ROW2,COL2    Draws a discrete segment from (ROW1,COL1) to
                            (ROW2,COL2) with Bresenham's algorithm.
  -c ROW,COL,RADIUS         Draws a circle centered at (ROW,COL) of
                            radius RADIUS with the midpoint algorithm.
```

## Options supportées

À la fin, votre programme devra offrir les options suivantes.

### Option `-n`: création d'un canevas

Cette option permet de créer un canevas dans lequel tous les pixels sont vides
(le pixel vide est identifié par le caractère `.`):

```sh
$ ./canvascii -n 2,4
....
....
```

En particulier, lorsque l'option `-n` est utilisée en premier, la lecture sur
`stdin` est ignorée:

```sh
$ ./canvascii -n 2,4 < examples/empty5x8.canvas
....
....
```

Noter que les dimensions maximales autorisées sont `40,80`.

### Option `-s`: affichage du canevas

Lorsque l'option `-s` est utilisée, le canevas est affiché sur la sortie
standard et le programme termine son exécution:

```sh
$ ./canvascii -s < examples/empty5x8.canvas
........
........
........
........
........
```

En particulier, s'il y a des options qui suivent `-s`, elles sont simplement
ignorées:

```sh
$ ./canvascii -s -h 0 < examples/empty5x8.canvas
........
........
........
........
........
```

### Option `-h`: tracé d'une ligne horizontale

L'option `-h` permet de tracer une ligne horizontale occupant la largeur de
tout le canevas:

```sh
$ ./canvascii -n 3,7 -h 1
.......
7777777
.......
```

Si le numéro de ligne est hors canevas, un message d'erreur est affiché et le
manuel d'utilisation est rappelé:

```sh
$ ./canvascii -n 3,5 -h 6
Error: incorrect value with option -h
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
7
```

### Option `-v`: tracé d'une ligne verticale

L'option `-v` permet de tracer une ligne verticale occupant la haute de tout le
canevas:

```sh
$ ./canvascii -n 3,7 -v 1 -v 3 -v 5
.7.7.7.
.7.7.7.
.7.7.7.
```

Comme pour l'option `-h`, si le numéro de colonne est hors canevas, un message
d'erreur est affiché et le manuel d'utilisation est rappelé:

```sh
$ ./canvascii -n 3,5 -v 5
Error: incorrect value with option -v
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
7
```

### Option `-r`: tracé d'un rectangle

L'option `-r` permet de tracer un rectangle sur le canevas:

```sh
$ ./canvascii -n 5,7 -r 1,1,3,5
.......
.77777.
.7...7.
.77777.
.......
```

Si certains points du rectangle se trouvent hors canevas, il n'y a pas
d'erreur, les pixels sont tout simplement rognés:

```sh
$ ./canvascii -n 5,7 -r 1,1,8,8
.......
.777777
.7.....
.7.....
.7.....
```

Les positions peuvent être négatives:

```sh
$ ./canvascii -n 5,5 -r -1,-1,3,3
.7...
77...
.....
.....
.....
```

Mais pas les dimensions:

```sh
$ ./canvascii -n 5,5 -r 1,1,-2,-2
Error: incorrect value with option -r
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
```

### Option `-l`: tracé d'un segment

Il est possible de tracer un segment discret avec l'option `-l`:

```sh
$ ./canvascii -n 6,13 -l 1,1,4,11
.............
.77..........
...777.......
......7777...
..........77.
.............
```

Si le segment sort du canevas, il est rogné:

```sh
$ ./canvascii -n 4,10 -l 1,1,4,11
..........
.77.......
...777....
......7777
```

Les positions avec des valeurs négatives sont acceptées:

```sh
$ ./canvascii -n 5,5 -l -2,6,6,-2
....7
...7.
..7..
.7...
7....
```

L'algorithme utilisé pour tracer un segment discret est appelé [Algorithme de
Bresenham](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm). Le
pseudocode traitant tous les octants simultanément se trouve à la fin de la
section [*All
cases*](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm?section=7#All_cases)
de l'article Wikipedia en anglais qui traite de ce sujet.

### Option `-c`: tracé d'un cercle

On peut également tracer un cercle discret à l'aide de l'option `-c`:

```sh
$ ./canvascii -n 11,11 -c 5,5,5
...77777...
..7.....7..
.7.......7.
7.........7
7.........7
7.........7
7.........7
7.........7
.7.......7.
..7.....7..
...77777...
```

Comme pour les options `-r` et `-l`, si le cercle sort du canevas, il est tout
simplement rogné:

```sh
$ ./canvascii -n 5,5 -c 4,4,3
.....
...77
..7..
.7...
.7...
```

Le centre du cercle peut avoir des coordonnées négatives:

```sh
$ ./canvascii -n 5,5 -c -1,-1,5
....7
....7
...7.
..7..
77...
```

Mais pas son rayon:

```sh
$ ./canvascii -n 5,5 -c 1,1,-2
Error: incorrect value with option -c
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
```

L'algorithme utilisé pour tracer un cercle discret est appelé [Algorithme de
tracé d'arc de cercle par point
milieu](https://en.wikipedia.org/wiki/Midpoint_circle_algorithm). Une
implémentation de cet algorithme en C est disponible sur le site [Rosetta
Code](http://rosettacode.org/wiki/Bitmap/Midpoint_circle_algorithm#C).
N'hésitez pas à la récupérer, à l'adapter à votre programme et ensuite à citer
sa provenance.

### Option `-p`: sélection du crayon

En tout temps, on peut changer le crayon utilisé pour dessiner:

```sh
$ ./canvascii -n 5,5 -p 4 -h 2 -p 2 -v 2
..2..
..2..
44244
..2..
..2..
```

Par défaut, le crayon utilisé est `7`.

### Option `-k`: colorisation du canevas

Finalement, il est possible d'afficher une sortie en couleur en transformant
les caractères `0` à `7` par leur [code de couleur
ANSI](https://en.wikipedia.org/wiki/ANSI_escape_code#3/4_bit) correspondant:

```sh
$ ./canvascii -k < examples/all-pens5x8.canvas
```

![](misc/all-colors.png)

```sh
$ ./canvascii -n 19,19 -p 1 -c 9,9,9 -l 3,3,15,15 -l 3,15,15,3 -k
```

![](misc/crossed-red-circle.png)

## Gestion des erreurs

Votre programme doit détecter les erreurs d'utilisation. Pour cela, vous devez
utiliser les codes d'erreur suivants (identifiés à l'aide d'un type énumératif
en C):

```c
enum error {
    OK                         = 0, // Everything is ok
    ERR_WRONG_PIXEL            = 1, // Wrong pixel value in canvas
    ERR_CANVAS_TOO_HIGH        = 2, // Canvas is too high
    ERR_CANVAS_TOO_WIDE        = 3, // Canvas is too wide
    ERR_CANVAS_NON_RECTANGULAR = 4, // Canvas is non rectangular
    ERR_UNRECOGNIZED_OPTION    = 5, // Unrecognized option
    ERR_MISSING_VALUE          = 6, // Option with missing value
    ERR_WITH_VALUE             = 7  // Problem with value
}
```

Lorsqu'une erreur survient, un message en conséquence, suivi du manuel
d'utilisation est affiché sur le canal d'erreur (`stderr`):

1. Si un caractère apparaissant dans le canevas est interdit:

```sh
$ ./canvascii -s < examples/wrong-char.canvas
Error: wrong pixel value #
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
1
```

2. Si le canevas lu sur `stdin` est trop haut:

```sh
$ ./canvascii -s < examples/too-high.canvas
Error: canvas is too high (max height: 40)
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
2
```

3. Si le canevas lu sur `stdin` est trop large:

```sh
$ ./canvascii -s < examples/too-wide.canvas
Error: canvas is too wide (max width: 80)
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
3
```

4. Si les lignes du canevas lu sur `stdin` ne sont pas toutes de même longueur:

```sh
$ ./canvascii -s < examples/non-uniform-width.canvas
Error: canvas should be rectangular
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
4
```

5. Si on tente d'utiliser une option non reconnue:

```sh
$ ./canvascii -n 5,5 -a
Error: unrecognized option -a
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
5
```

6. Si on oublie de fournir une valeur à une option qui en demande une:

```sh
$ ./canvascii -n
Error: missing value with option -n
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
6
```

7. Finalement, si on fournit une valeur mal formatée ou invalide:

```sh
$ ./canvascii -n 5,5 -p a
Error: incorrect value with option -p
Usage: ./canvascii [-n HEIGHT,WIDTH] [-s] [-k] [-p CHAR]
          [-h ROW] [-v COL] [-r ROW,COL,HEIGHT,WIDTH]
          [-l ROW1,COL1,ROW2,COL2] [-c ROW,COL,RADIUS]
[...]
$ echo $?
7
```


## Auteur

Fang, Xin Ran

## Fonctionnement

**Guide d'utilisation**
Avant de pouvoir compiler un programme en ```C``` à l'aide de la ligne de commande, il faut s'assurer d'avoir déjà installé gcc ```GNU C Compiler``` sur le système.

**Compilation d'un programme en C**
Pour compiler un programme en C, il faut utiliser la commande ```gcc -option```.

```
$ gcc fichier.c 
```

Pour afficher les messages d'avertissement lorsqu'il y a d'erreurs, il faut ajouter l'option ```-Wall```. 

```
$ gcc -Wall fichier.c
```

Pour donner un nom au programme compilé, il suffit d'ajouter l'option ```-o```.

```
$ gcc -Wall fichier.c -o nom_du_programme
```

**Exécution d'un programme en C**
Pour exécuter un programme, il faut placer ```./``` devant le nom du programme.

```
$ ./nom_du_programme
```

**Extraction d'une archive**
Pour extraire une archive, il suffit d'utiliser la commande ```tar``` avec l'option ```-xf```.

```
$ tar -xf nom_de_archive
```

**Exécution du programme canvascii**
```
$ ./canvascii -option arguments
```


## Dépendances

* [GCC](https://gcc.gnu.org/) - The GNU C Compiler 
* [Vim](https://www.vimorg/) - The text editor
* [Bats](https://github.com/bats-core/bats-core/) - Bash Automated Testing System

## Références

* [Algorithme de Bresenham](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm/)
