# TPE_Serie3
Ce repository est pour le troisieme TP qui a pour objectif de creer une application d'images PPM en utilisant les fichiers et les tableaux
#  Projet PPM Viewer – Manipulation d’images au format P3 (TPE_Serie_3)

##  Membres du groupe

 Nom complet et Tâche réalisée 

**CHEMTCHOUA DORIANE AUDREY**: Tâche 1 – Foncer ou éclaircir tous les pixels ayant une dominante (R, G ou B)
**DOMGUEM NZONOU ANGE MALONE**: Tâche 2 – Conversion de l’image en niveaux de gris
**MOKAM FOTSO MELVINE ARLETTY**: Tâche 3 – Création du négatif d’une image
**AMBADIANG AYANGMA FAITH JORDAN EVAN’S**: Tâche 4 – Affichage de la taille de l’image (largeur × hauteur) + programme principal (`main.c`)
**FOUDA KPAMA LANDRY HERMANN**: Tâche 5 – Découpage et sauvegarde d’une portion d’image
**SIMO TAMBA IDE EVORA**: Tâche 6 – Application du filtre médian sur une image

##  Objectif du projet

Le but de ce projet est de développer une **application en langage C** permettant de **manipuler des images PPM (Portable PixMap)** au format **P3** (texte brut).

Le programme permet d'effectuer plusieurs traitements sur des images PPM :
1. Foncer ou éclaircir les pixels dominés par une couleur donnée (R, G ou B)
2. Convertir une image en niveaux de gris
3. Créer le négatif d’une image
4. Afficher la taille de l’image
5. Découper une zone précise de l’image
6. Appliquer un filtre médian
   
##  Organisation de l’implémentation

Le code a été **entièrement réalisé en langage C standard (ANSI C)**, sans utilisation d’allocation dynamique (`malloc`, `calloc`, etc.).  
Toutes les manipulations d’images se font à l’aide :
- de **fichiers PPM en mode texte (P3)**,
- et de **tableaux statiques** pour stocker les valeurs RGB.

### Structure principale du programme
Le programme repose sur une **interface en ligne de commande** simulant un mini-shell :

ppmviewer >

Cette interface permet à l’utilisateur de saisir des commandes pour appliquer différents traitements.

### Fichiers et fonctions principales

`ppmviewer.c` : Contient l’ensemble des fonctions et le `main()` 
(optionnel) `images/`: Dossier contenant les images `.ppm` utilisées pour les tests 

### Fonctions clés et leur Roles

`lire_image()`: Lit un fichier PPM et stocke ses pixels dans des tableaux RGB 
 `ecrire_image()`: Sauvegarde une image dans un fichier PPM au format P3 
 `gris()`: Convertit l’image en niveaux de gris 
 `negatif()`: Calcule le négatif de l’image 
 `dominante(couleur, valeur)`: Fonce/éclaircit les pixels selon leur dominante 
 `couper(l1, l2, c1, c2)`: Extrait une sous-image selon les coordonnées 
 `filtre_median()`: Applique un filtrage médian (réduction du bruit) 
 `mediane()`: Calcule la médiane d’un petit tableau de valeurs 
 `main()`: Gère la boucle principale et les commandes utilisateur 

## Compilation
gcc ppmviewer.c -o ppmviewer

Le programme repose sur une **interface en ligne de commande** simulant un mini-shell :
b
