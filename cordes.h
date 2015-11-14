#include <stdio.h>
#include <errno.h>
int getopt(int argc, char * const argv[], const char *optstring);
long int lrand48 (void);
extern char *optarg;
extern int optind, opterr, optopt;

#define CORDE_MAX 32
#define BRIN_MAX 64

/*
  Une corde correspond à un croisement dans un diagramme de noeud,
  et comporte deux brins jumeaux, un pour chaque passage au croisement;
  le second brin est repéré par une longueur négative, opposée
  de celle du premier. La distinction entre premier et second brin,
  correspond à celle entre premier et second passage à un
  croisement, et dépend évidemmment de l'origine choisie.

  L'orientation ne dépend que de la courbe générique (ombre du noeud),
  et vaut +1 (resp. -1) si la rotation du brin vers son jumeau est
  positive (resp. négative).

  Le signe est celui du croisement, mesuré depuis le brin supérieur
  vers le brin inférieur; il est donc égal (resp. opposé) à
  l'orientation pour le brin supérieur (resp. inférieur).

  Le signe du croisement ne dépend que de la corde, contrairement
  à l'orientation: deux brins jumeaux ont des orientations opposées.
*/

struct Brin {
  int longueur, orientation, signe;
};

struct Diagramme {
  int taille;
  struct Brin brin [BRIN_MAX];
};

/* variables globales */
int complexite, periode, orbite, delta;
/* options globales */
int groupe, gauss, liste, dowker, trace, zuber;

int orienter (struct Diagramme *d, int *erreur);
int nb_faces (struct Diagramme *d);
void enregistrer_cordes (struct Diagramme *d, int *t, int n, int signe);
void enregistrer_brins (struct Diagramme *d, int *t, int n);
void afficher_cordes (struct Diagramme *d, int signe);
void afficher_brins (struct Diagramme *d);
void afficher_orientations (struct Diagramme *d);
int involution_suivante (int *t, int n);
int minimum_cyclic (int *t, int n);
int is_minimal_cyclic (int *t, int n);
int is_minimal_diedral (int *t, int n);
void involution_reverse (int *t, int *u, int n);
int realisable (struct Diagramme *d);
