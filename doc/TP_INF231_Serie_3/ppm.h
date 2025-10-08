#ifndef PPM_H
#define PPM_H

#include <stdio.h>

#define MAX_HAUTEUR 500
#define MAX_LARGEUR 500

typedef struct {
    int r, g, b;
} Pixel;

typedef struct {
    int largeur, hauteur, max_val;
    Pixel pixels[MAX_HAUTEUR][MAX_LARGEUR];
} Image;

/* lirePPM returns 1 on success, 0 on failure */
int lirePPM(const char* nomFichier, Image* img);
void ecrirePPM(const char* nomFichier, Image img);
void afficherTaille(Image img);
void passerGris(Image* img);
void negatif(Image* img);
void foncerDominante(Image* img, char couleur, int val);
void couper(Image img, int l1, int l2, int c1, int c2, const char* nomRes);
void filtrer(Image* img);

#endif

