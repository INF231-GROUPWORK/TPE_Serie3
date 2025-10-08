#include <stdio.h>
#include <string.h>
#include "ppm.h"

int main() {
    char cmd[20];
    printf("Application de traitement d’images PPM réalisée par notre Groupe.\n");

    while (1) {
        printf("ppmviewer > ");
        scanf("%s", cmd);

        if (strcmp(cmd, "quit") == 0)
            break;

        else if (strcmp(cmd, "size") == 0) {
            char nom[100];
            scanf("%s", nom);
            Image img;
            lirePPM(nom, &img);
            afficherTaille(img);
        }

        else if (strcmp(cmd, "gris") == 0) {
            char nom[100];
            scanf("%s", nom);
            Image img;
            lirePPM(nom, &img);
            passerGris(&img);
            char sortie[120];
            sprintf(sortie, "%s_gris.ppm", nom);
            ecrirePPM(sortie, img);
            printf("Opération effectuée ; %s créé\n", sortie);
        }

        else if (strcmp(cmd, "dom") == 0) {
            char col; int val; char nom[100];
            scanf(" %c %d %s", &col, &val, nom);
            Image img;
            lirePPM(nom, &img);
            foncerDominante(&img, col, val);
            char sortie[120];
            sprintf(sortie, "%s_dom.ppm", nom);
            ecrirePPM(sortie, img);
            printf("Opération effectuée ; %s créé\n", sortie);
        }

        else if (strcmp(cmd, "neg") == 0) {
            char in[100], out[100];
            scanf("%s %s", in, out);
            Image img;
            lirePPM(in, &img);
            negatif(&img);
            ecrirePPM(out, img);
            printf("Négatif effectué ; %s créé\n", out);
        }

        else if (strcmp(cmd, "cut") == 0) {
            char nom[100], sortie[100];
            int l1, l2, c1, c2;
            scanf("%s %d %d %d %d %s", nom, &l1, &l2, &c1, &c2, sortie);
            Image img;
            lirePPM(nom, &img);
            couper(img, l1, l2, c1, c2, sortie);
            printf("Opération effectuée\n");
        }

        else
            printf("Commande inconnue.\n");
    }

    printf("Fermeture du programme.\n");
    return 0;
}

