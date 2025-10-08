#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_WIDTH 500
#define MAX_HEIGHT 500

int R[MAX_HEIGHT][MAX_WIDTH];
int G[MAX_HEIGHT][MAX_WIDTH];
int B[MAX_HEIGHT][MAX_WIDTH];
int largeur, hauteur, maxval;

void lire_image(const char *nom) {
    FILE *f = fopen(nom, "r");
    if (!f) {
        printf("Erreur : fichier %s introuvable.\n", nom);
        return;
    }

    char magic[3];
    fscanf(f, "%s", magic);
    if (strcmp(magic, "P3") != 0) {
        printf("Erreur : format non supporté (seul P3 accepté).\n");
        fclose(f);
        return;
    }

    // Ignorer tous les commentaires commençant par #
char ligne[256];
while (fscanf(f, " %255s", ligne) == 1 && ligne[0] == '#') {
    fgets(ligne, sizeof(ligne), f); // ignorer toute la ligne
}
ungetc(ligne[0], f);


    fscanf(f, "%d %d", &largeur, &hauteur);
    fscanf(f, "%d", &maxval);

    if (largeur > MAX_WIDTH || hauteur > MAX_HEIGHT) {
        printf("Erreur : image trop grande (max %dx%d)\n", MAX_WIDTH, MAX_HEIGHT);
        fclose(f);
        return;
    }

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            fscanf(f, "%d %d %d", &R[i][j], &G[i][j], &B[i][j]);
        }
    }

    fclose(f);
}

void ecrire_image(const char *nom) {
    FILE *f = fopen(nom, "w");
    if (!f) {
        printf("Erreur : impossible d’écrire dans %s.\n", nom);
        return;
    }

    fprintf(f, "P3\n%d %d\n%d\n", largeur, hauteur, maxval);
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            fprintf(f, "%d %d %d ", R[i][j], G[i][j], B[i][j]);
        }
        fprintf(f, "\n");
    }

    fclose(f);
}



void gris() {
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int g = (R[i][j] + G[i][j] + B[i][j]) / 3;
            R[i][j] = G[i][j] = B[i][j] = g;
        }
    }
}

void negatif() {
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            R[i][j] = maxval - R[i][j];
            G[i][j] = maxval - G[i][j];
            B[i][j] = maxval - B[i][j];
        }
    }
}

void dominante(char c, int val) {
    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int max = R[i][j];
            char dom = 'R';
            if (G[i][j] > max) { max = G[i][j]; dom = 'G'; }
            if (B[i][j] > max) { max = B[i][j]; dom = 'B'; }

            if (dom == c) {
                R[i][j] += val;
                G[i][j] += val;
                B[i][j] += val;
                if (R[i][j] < 0) R[i][j] = 0; if (R[i][j] > maxval) R[i][j] = maxval;
                if (G[i][j] < 0) G[i][j] = 0; if (G[i][j] > maxval) G[i][j] = maxval;
                if (B[i][j] < 0) B[i][j] = 0; if (B[i][j] > maxval) B[i][j] = maxval;
            }
        }
    }
}

void couper(int l1, int l2, int c1, int c2) {
    int newH = l2 - l1 + 1;
    int newW = c2 - c1 + 1;

    int tempR[MAX_HEIGHT][MAX_WIDTH];
    int tempG[MAX_HEIGHT][MAX_WIDTH];
    int tempB[MAX_HEIGHT][MAX_WIDTH];

    for (int i = 0; i < newH; i++) {
        for (int j = 0; j < newW; j++) {
            tempR[i][j] = R[l1 - 1 + i][c1 - 1 + j];
            tempG[i][j] = G[l1 - 1 + i][c1 - 1 + j];
            tempB[i][j] = B[l1 - 1 + i][c1 - 1 + j];
        }
    }

    largeur = newW;
    hauteur = newH;

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            R[i][j] = tempR[i][j];
            G[i][j] = tempG[i][j];
            B[i][j] = tempB[i][j];
        }
    }
}

// Renvoie la médiane d’un petit tableau (max 9 éléments)
int mediane(int tab[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = i + 1; j < n; j++) {
            if (tab[i] > tab[j]) {
                int t = tab[i];
                tab[i] = tab[j];
                tab[j] = t;
            }
        }
    }
    return tab[n / 2];
}

void filtre_median() {
    int tmpR[MAX_HEIGHT][MAX_WIDTH];
    int tmpG[MAX_HEIGHT][MAX_WIDTH];
    int tmpB[MAX_HEIGHT][MAX_WIDTH];

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            int voisinsR[9], voisinsG[9], voisinsB[9];
            int count = 0;

            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    int ni = i + di, nj = j + dj;
                    if (ni >= 0 && ni < hauteur && nj >= 0 && nj < largeur) {
                        voisinsR[count] = R[ni][nj];
                        voisinsG[count] = G[ni][nj];
                        voisinsB[count] = B[ni][nj];
                        count++;
                    }
                }
            }
            tmpR[i][j] = mediane(voisinsR, count);
            tmpG[i][j] = mediane(voisinsG, count);
            tmpB[i][j] = mediane(voisinsB, count);
        }
    }

    for (int i = 0; i < hauteur; i++) {
        for (int j = 0; j < largeur; j++) {
            R[i][j] = tmpR[i][j];
            G[i][j] = tmpG[i][j];
            B[i][j] = tmpB[i][j];
        }
    }
}



int main() {
    char cmd[20], fichier1[100], fichier2[100];
    char couleur;
    int val, l1, l2, c1, c2;

    printf("Application de traitement d'images PPM réalisée par XXX.\n");

    while (1) {
        printf("ppmviewer > ");
        scanf("%s", cmd);

        if (strcmp(cmd, "quit") == 0) {
            break;
        }

        else if (strcmp(cmd, "size") == 0) {
            scanf("%s", fichier1);
            lire_image(fichier1);
            printf("%d x %d\n", largeur, hauteur);
        }

        else if (strcmp(cmd, "gris") == 0) {
            scanf("%s", fichier1);
            lire_image(fichier1);
            gris();
            char sortie[120];
            sprintf(sortie, "%s_gris.ppm", fichier1);
            ecrire_image(sortie);
            printf("opération effectuée ; %s créé\n", sortie);
        }

        else if (strcmp(cmd, "neg") == 0) {
            scanf("%s %s", fichier1, fichier2);
            lire_image(fichier1);
            negatif();
            ecrire_image(fichier2);
            printf("opération effectuée\n");
        }

        else if (strcmp(cmd, "dom") == 0) {
            scanf(" %c %d %s", &couleur, &val, fichier1);
            lire_image(fichier1);
            dominante(couleur, val);
            char sortie[120];
            sprintf(sortie, "%s_dom.ppm", fichier1);
            ecrire_image(sortie);
            printf("opération effectuée ; %s créé\n", sortie);
        }

        else if (strcmp(cmd, "cut") == 0) {
            scanf("%s %d %d %d %d %s", fichier1, &l1, &l2, &c1, &c2, fichier2);
            lire_image(fichier1);
            if (l1 < l2 && l2 <= hauteur && c1 < c2 && c2 <= largeur) {
                couper(l1, l2, c1, c2);
                ecrire_image(fichier2);
                printf("opération effectuée\n");
            } else {
                printf("Paramètres de découpe invalides.\n");
            }
        }

        else if (strcmp(cmd, "fil") == 0) {
            scanf("%s %s", fichier1, fichier2);
            lire_image(fichier1);
            filtre_median();
            ecrire_image(fichier2);
            printf("opération effectuée\n");
        }

        else {
            printf("Commande inconnue.\n");
        }
    }

    return 0;
}
