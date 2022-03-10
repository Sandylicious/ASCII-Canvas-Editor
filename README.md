# TP1: Dessiner sur un canevas ASCII"

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

## Auteur

Xin Ran Fang

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

## Dépendances

* [GCC](https://gcc.gnu.org/) - The GNU C Compiler 
* [Vim](https://www.vimorg/) - The text editor
* [Bats](https://github.com/bats-core/bats-core/) - Bash Automated Testing System

## Références

* [Algorithme de Bresenham](https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm/)
