#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "ppm.h"

/* Helper: skip whitespace and full-line comments starting with '#' */
static void skip_comments_and_spaces(FILE *f) {
    int c = fgetc(f);
    while (c != EOF) {
        if (isspace(c)) { c = fgetc(f); continue; }
        if (c == '#') {
            while ((c = fgetc(f)) != EOF && c != '\n');
            c = fgetc(f);
            continue;
        }
        ungetc(c, f);
        break;
    }
}

/* lirePPM : lit uniquement le format P3 (ASCII). Retourne 1 si succès, 0 sinon.
   NOTE: silent on parse errors (no diagnostic prints) as requested. */
int lirePPM(const char* nomFichier, Image* img) {
    img->largeur = img->hauteur = img->max_val = 0;

    FILE *f = fopen(nomFichier, "r");
    if (!f) {
        return 0;
    }

    char format[3];
    if (fscanf(f, "%2s", format) != 1) { fclose(f); return 0; }
    if (strcmp(format, "P3") != 0) { fclose(f); return 0; }

    skip_comments_and_spaces(f);

    if (fscanf(f, "%d %d", &img->largeur, &img->hauteur) != 2) { fclose(f); img->largeur = img->hauteur = 0; return 0; }

    skip_comments_and_spaces(f);

    if (fscanf(f, "%d", &img->max_val) != 1) { fclose(f); img->largeur = img->hauteur = 0; return 0; }

    if (img->largeur <= 0 || img->hauteur <= 0 ||
        img->largeur > MAX_LARGEUR || img->hauteur > MAX_HAUTEUR) {
        fclose(f);
        return 0;
    }

    skip_comments_and_spaces(f);

    for (int i = 0; i < img->hauteur; i++) {
        for (int j = 0; j < img->largeur; j++) {
            int r, g, b;
            if (fscanf(f, "%d %d %d", &r, &g, &b) != 3) { fclose(f); return 0; }
            if (r < 0) r = 0; if (g < 0) g = 0; if (b < 0) b = 0;
            if (img->max_val > 0) {
                if (r > img->max_val) r = img->max_val;
                if (g > img->max_val) g = img->max_val;
                if (b > img->max_val) b = img->max_val;
            }
            img->pixels[i][j].r = r;
            img->pixels[i][j].g = g;
            img->pixels[i][j].b = b;
        }
    }

    fclose(f);
    return 1;
}

/* ecrirePPM : write P3 ASCII file */
void ecrirePPM(const char* nomFichier, Image img) {
    FILE *f = fopen(nomFichier, "w");
    if (f == NULL) {
        return;
    }

    if (img.max_val <= 0) img.max_val = 255;

    fprintf(f, "P3\n%d %d\n%d\n", img.largeur, img.hauteur, img.max_val);
    for (int i = 0; i < img.hauteur; i++) {
        for (int j = 0; j < img.largeur; j++) {
            fprintf(f, "%d %d %d ", img.pixels[i][j].r, img.pixels[i][j].g, img.pixels[i][j].b);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}

/* afficherTaille */
void afficherTaille(Image img) {
    printf("%d x %d\n", img.largeur, img.hauteur);
}

/* passerGris */
void passerGris(Image* img) {
    for (int i = 0; i < img->hauteur; i++)
        for (int j = 0; j < img->largeur; j++) {
            int moy = (img->pixels[i][j].r + img->pixels[i][j].g + img->pixels[i][j].b) / 3;
            img->pixels[i][j].r = img->pixels[i][j].g = img->pixels[i][j].b = moy;
        }
}

/* negatif */
void negatif(Image* img) {
    int M = (img->max_val > 0) ? img->max_val : 255;
    for (int i = 0; i < img->hauteur; i++)
        for (int j = 0; j < img->largeur; j++) {
            img->pixels[i][j].r = M - img->pixels[i][j].r;
            img->pixels[i][j].g = M - img->pixels[i][j].g;
            img->pixels[i][j].b = M - img->pixels[i][j].b;
        }
}

/* foncerDominante */
void foncerDominante(Image* img, char couleur, int val) {
    for (int i = 0; i < img->hauteur; i++) {
        for (int j = 0; j < img->largeur; j++) {
            Pixel *p = &img->pixels[i][j];
            int maxc = (p->r > p->g) ? ((p->r > p->b) ? p->r : p->b)
                                    : ((p->g > p->b) ? p->g : p->b);
            int domOK = (couleur == 'R' && p->r == maxc) ||
                        (couleur == 'G' && p->g == maxc) ||
                        (couleur == 'B' && p->b == maxc);
            if (domOK) {
                int nr = p->r + val, ng = p->g + val, nb = p->b + val;
                if (nr < 0) nr = 0; if (nr > 255) nr = 255;
                if (ng < 0) ng = 0; if (ng > 255) ng = 255;
                if (nb < 0) nb = 0; if (nb > 255) nb = 255;
                p->r = nr; p->g = ng; p->b = nb;
            }
        }
    }
}

/* couper */
void couper(Image img, int l1, int l2, int c1, int c2, const char* nomRes) {
    if (l1 >= l2 || c1 >= c2 || l2 > img.hauteur || c2 > img.largeur || l1 < 0 || c1 < 0) {
        return;
    }
    Image res;
    res.hauteur = l2 - l1;
    res.largeur = c2 - c1;
    res.max_val = img.max_val > 0 ? img.max_val : 255;

    for (int i = 0; i < res.hauteur; i++)
        for (int j = 0; j < res.largeur; j++)
            res.pixels[i][j] = img.pixels[i + l1][j + c1];

    ecrirePPM(nomRes, res);
}

/* filtrer - 3x3 average */
void filtrer(Image* img) {
    Image copie = *img;
    if (img->hauteur < 3 || img->largeur < 3) return;
    for (int i = 1; i < img->hauteur - 1; i++) {
        for (int j = 1; j < img->largeur - 1; j++) {
            int sumR = 0, sumG = 0, sumB = 0;
            for (int di = -1; di <= 1; di++)
                for (int dj = -1; dj <= 1; dj++) {
                    sumR += copie.pixels[i + di][j + dj].r;
                    sumG += copie.pixels[i + di][j + dj].g;
                    sumB += copie.pixels[i + di][j + dj].b;
                }
            img->pixels[i][j].r = sumR / 9;
            img->pixels[i][j].g = sumG / 9;
            img->pixels[i][j].b = sumB / 9;
        }
    }
}

