#include <stdio.h>
#include <stdlib.h>
#include <sys/timeb.h>

#include "cordes.h"

struct timeb debut;

int
  nb_diag [BRIN_MAX + 1],
  cfiltre, ctraitement, compteur, miroir;

void chrono (char *s) {
  struct timeb fin;
  time_t sec;
  short msec;

  ftime( &fin );

  printf ("%s: ", s);

  sec = fin.time - debut.time;
  msec = fin.millitm - debut.millitm;
  if (msec < 0) {
    sec--;
    msec += 1000;
  }

  printf ("%d.%03d secondes\n", (int)sec, msec);

  ftime( &debut );
}

int filtrer (int *t, int n) {
  /*
   * La fonction 'filtrer' élimine les diagrammes non minimaux
   * dans leur orbite cyclique (groupe=1) ou diédrale (groupe=2).
   */
  int m = 2 * n;
  /*
   * La variable globale 'cfiltre' compte les involutions
   * produites par itération de 'involution_suivante'.
   */
  cfiltre++;
  if (!groupe) return 1; // pas de filtre
  if (groupe > 1)
    return is_minimal_diedral(t, m);
  return is_minimal_cyclic(t, m);
}

void traiter (struct Diagramme *d) {
  if (gauss && ! realisable(d))
    return;
  compteur++;
  if (liste) {
    afficher_brins (d);
    printf ("\n");
    if (gauss){
      afficher_orientations (d);
      printf ("\n");
    }
  }
  if (groupe == 1) {
    nb_diag [periode]++;
    if (liste)
      printf ("Période%3d", periode);
    if (gauss) {
      if (is_chiral_cyclic(d))
        miroir++;
      else if (liste)
        printf(", achiral");
    }
  }
  else if (groupe == 2) {
    nb_diag [orbite]++;
    if (liste)
      printf ("Période%3d, orbite diédrale de taille%3d", periode, orbite);
    if (gauss) {
      if (is_chiral_diedral(d))
        miroir++;
      else if (liste)
        printf(", achiral");
    }
  }
  if (liste)
    printf("\n");
}

void parcourir_diagrammes (int *t, int n, int max) {
  int f, m = 2 * n;
  struct Diagramme d;

  while ( involution_suivante (t, m) ) {
    f = filtrer (t, n);
    if (f) {
      ctraitement++;
      enregistrer_brins (&d, t, m);
      traiter (&d);
    }
    if (max && compteur >= max) break;
  }
}

int main (int argc, char **argv) {
  int
    c, n, max = 0, t[BRIN_MAX];

  // initialisation des compteurs
  compteur = cfiltre = ctraitement = miroir = 0;

  // global options
  dowker = groupe = liste = gauss = trace = zuber = 0;
  while ((c = getopt(argc, argv, "cdgklstz")) != -1)
    switch (c) {
      case 'c':
        groupe = 1; // groupe cyclique
        break;
      case 'd':
        groupe = 2; // groupe diédral
        break;
      case 'g':
        gauss = 1;
        break;
      case 'k':
        dowker = 1;
      case 'l':
        liste = 1;
        break;
      case 's':
        stat = 1;
        break;
      case 't':
        trace = 1;
        break;
      case 'z':
        zuber = 1;
        break;
    }

  ftime( &debut );

  n = atoi (argv [optind]);
  if (n > 8) liste = 0;
  t [0] = 1;
  for (int i = 1; i < 2 * n; i++) t [i] = 0;
  parcourir_diagrammes (t, n, max);

  if (!liste && n > 8)
    chrono ("Chrono");
  if (zuber) {
    if (groupe == 1)
      printf ("%8d OU-immersions, %d OO-immersions\n", compteur, compteur + miroir);
    else if (groupe == 2)
      printf ("%8d UU-immersions, %d UO-immersions\n", compteur, compteur + miroir);
  }
  else {
    printf ("%d involutions construites =>", cfiltre);
    if (groupe && ctraitement)
      printf (" %d minimale(s)", ctraitement);
    if (gauss) {
      printf (" %d réalisable(s)", compteur);
      if (groupe)
        printf (" %d chirale(s)", miroir);
    }
    printf ("\n");
  }
  if (groupe && stat){
    int involution = 0;
    for (int i = 0; i <= BRIN_MAX; i++)
      if (nb_diag [i] > 0) {
        if (groupe < 2)
          printf ("%8d diagramme(s) de période cyclique %2d\n", nb_diag [i], i);
        else
          printf ("%8d orbite(s) diédrale(s) de taille %2d\n", nb_diag [i], i);
        involution += i * nb_diag [i];
      }
    printf ("%8d involution(s)\n", involution);
  }
  return 1; /* pour respecter les standards et faire plaisir à gcc */
}
