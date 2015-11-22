#include "cordes.h"

/*
 * Le tableau 'oriente' contient les indices des croisements
 * déjà orientés, sa taille (croissante) est donnée par 'libre'.
 */
int oriente[CORDE_MAX], libre;

int circuit (struct Diagramme *d, int i) {
  /*
   * Une corde (i,p) est aussi un circuit i->p,
   * analysé par cette fonction, qui suppose que
   * 1. i<p et 2. le croisement (i,p) est déjà orienté.
   *
   * Un lobe est un circuit interne j->q
   * (i<j<q<p) où j n'appartient pas déjà à un lobe
   * (les lobes ne se chevauchent pas).
   *
   * S'il n'y a pas de lobe, la suite de brins[i,i+1,...,p-1]
   * forme une courbe de Jordan;
   * sinon, pour chaque lobe j->q, il faut retirer du circuit
   * la suite de brins[j, j+1,...,q-1] pour obtenir une courbe de Jordan.
   */
  int
    n = 2 * d->taille,
    j, p, q, r,
    e, epsilon, omega,
    lobe[BRIN_MAX];    // indice du lobe enveloppant, sinon 0

  p = d->brin[i].longueur + i;
  epsilon = d->brin[i].orientation;

  /* Marquer les lobes du circuit i->p */

  for (int j = i + 1; j < p;) {
    q = d->brin[j].longueur + j;
    if (j < q && q < p) {  // lobe
      for (int k = j; k < q; k++)
        lobe[k] = j;
      j = q;
    } else lobe[j++] = 0;
  }

  /*
   *  Maintenant le brin q parcourt circulairement le circuit p->i,
   *  et chaque corde (j,q) avec i<j<p permet d'orienter
   *  le croisement (j,q) ou bien un lobe, en utilisant la position
   *  du brin q par rapport à la courbe de Jordan.
   */

  for (int t = p + 1; t < i + n; t++) {
    q = t < n ? t : t - n;
    j = d->brin[q].longueur + q;
    if (j < i || j > p) // corde non sécante
      continue;

    if (lobe[j]) {
      int lobe_jordan = 1,
        a = lobe[j],
        b = d->brin[a].longueur + a;
      /*
       * Orienter le lobe, ie. le croisement (a,b),
       * en commençant par prendre en compte
       * les croisements entre le lobe et la courbe de Jordan.
       */
      for (int k = a + 1; k < j; k++) {
        r = d->brin[k].longueur + k;
        if (i < r && r < p && ! lobe[r])
          lobe_jordan = -lobe_jordan;
      }
      omega = -epsilon * lobe_jordan;
      j = a;
      q = b;
    }
    else {
      /*
       * Croisement (j,q), où j est un brin de la courbe de Jordan:
       * les brins q-1 et q sont de part et d'autre de cette courbe
       */
      epsilon = -epsilon;
      omega = epsilon;
    }

    /*
     * Maintenant on peut orienter le croisement (j,q);
     * s'il est déjà orienté on teste une éventuelle contradiction.
     */

    e = d->brin[j].orientation;
    if (e) {
      if (e != omega) {
        if (trace) {
          afficher_brins(d);
          printf (" (contradiction i=%d j=%d)\n", i, j);
        }
        return 0;
      }
    }
    else {
      d->brin[j].orientation = omega;
      d->brin[q].orientation = -omega;
      oriente[libre++] = j < q ? j : q;
    }
  } // fin du parcours circulaire p->i
  return 1;
}

int orienter (struct Diagramme *d, int *ok) {
  /*
   * Calcule les orientations des croisements de la courbe de Gauss
   * associée à un diagramme de cordes, si elle existe.
   *
   * S'arrête dès que:
   *    1) tous les croisements (= cordes) sont orienté(e)s,
   * ou 2) une contradiction apparaît,
   * ou 3) l'algorithme ne progresse plus.
   *
   * Retourne le nombre de cordes orientées, ce qui permet de distinguer
   * 1) et 3) ; le cas 3) signifie que le diagramme n'est pas connexe.
   * Le cas 2) est signalé par la valeur de 'ok'.
   *
   * Le cas 1) ne signifie ni qu'aucune contradiction ne serait apparue
   * en poursuivant l'exécution de l'algorithme, ni que le diagramme
   * est réalisable: il faut encore calculer le nombre de faces.
   */
  int p, n = 2 * d->taille;

  for (int i = 1; i < n; i++)
    d->brin[i].orientation = 0;

  d->brin[0].orientation = 1;
  p = d->brin[0].longueur;
  d->brin[p].orientation = -1;
  oriente[0] = 0;
  libre = 1;

  for (int i = 0; i < libre; i++) {
    *ok = circuit (d, oriente[i]);
    if (! *ok || libre >= d->taille)
      return libre;
  }
  if (trace) {
    afficher_brins(d);
    printf (" (non primitif)\n");
  }
  return libre;
}

int realisable (struct Diagramme *d) {
  int c, ok, n = d->taille;
  c = orienter (d, &ok);
  if (c < n || ! ok)
    return 0;
  else {
    c = nb_faces (d);
    if (c != n + 2) {
      if (trace) {
        afficher_brins(d);
        printf (" (%d faces)\n", c);
      }
      return 0;
    }
  }
  return 1;
}

int nb_faces (struct Diagramme *d) {
  int
    f = 0,		/* compte les faces */
    i, j,
    n = 2 * d->taille,
    depart = 1,
    sens,
    passage[BRIN_MAX];

  /*
    Pour chaque brin i, passage[i] vaut:
      0  si le brin n'a pas été parcouru
      1  si le brin a été parcouru dans le sens positif
     -1  si le brin a été parcouru dans le sens négatif
      2  si le brin a été parcouru dans les deux sens
  */

  for (i = 0; i < n; i++)
    passage[i] = 0;

  for (i = 0; i < n;) {
    if (passage[i] < 2) {
      f++;
      depart = passage[i] <= 0 ? 1 : -1;
      j = i;
      sens = depart;
      do {
        passage[j] = passage[j] ? 2 : sens;
        /*
          printf ("%d (%d) -> ", j, passage[j]);
        */
        j = j + sens;

        /* attention: parcours circulaire */
        if (j == n)
          j = 0;
        else if (j < 0)
          j = n - 1;
        /*
          printf ("%d (%d)\n", j, passage[j]);
        */

        if (d->brin[j].orientation < 0)
          sens = -sens;
        j = d->brin[j].longueur + j;	/* autre extrémité de la corde */
      } while (j != i || sens != depart);
    }
    if (passage[i] == 2)
      i++;
  }
  return f;

}
