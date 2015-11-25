#include "cordes.h"

void enregistrer_cordes (struct Diagramme *d, int *t, int n) {
  /*
   * Enregistre un tableau 't' de 'n' cordes dans un diagramme d'adresse 'd'.
   */
  int i, j, k, m, p, x;

  d->taille = n;
  m = 2 * n;
  for (i = 0; i < m; i++)
    d->brin[i].longueur = 0;

  for (i = j = 0; i < n; i++) {
    x = t[i];
    k = x > 0 ? x : -x;
    p = j + k;
    d->brin[j].longueur = k;
    d->brin[p].longueur = - k;
    do j++; while (d->brin[j].longueur);
  }
}

void enregistrer_brins (struct Diagramme *d, int *t, int n) {
  /*
   * Enregistre un tableau 't' de 'n' brins dans un diagramme d'adresse 'd'.
   */
  int i;

  d->taille = n / 2;
  for (i = 0; i < n; i++) {
    d->brin[i].longueur = t[i];
  }
}

void afficher_brins (struct Diagramme *d) {
  /* affiche les longueurs des brins d'un diagramme */
  int j, n = 2 * d->taille;
  for (int i = 0; i < n; i++){
    j = d->brin[i].longueur;
    if (j<0) j += n;
    printf ("%3d", j);
  }
  if(dowker) {
    printf (" DT:");
    for (int i = 0; i < n; i += 2) {
      j = i + d->brin[i].longueur + 1;
      printf ("%3d", j);
    }
  }
}

void afficher_orientations (struct Diagramme *d) {
  /* affiche les orientations des brins d'un diagramme */
  int i;

  for (i = 0; i < 2 * d->taille; i++)
    printf ("%3d", d->brin[i].orientation);
}

void trace_t (int *t, int n){
  for (int i = 0; i < n; i++)
    printf("%3d", t[i]);
  printf ("\n");
}

static int completer_involution (int *t, int n, int i, int p);

/*
 * involution_suivante () transforme le tableau 't', avec 'n' brins,
 * en un tableau contenant l'involution suivante dans l'ordre
 * lexicographique; toutes les cordes sont de longueur impaire.
 *
 * Si l'option globale 'groupe' est vraie,
 * toutes les cordes sont de longueur au moins égale à la
 * première -- t[0] -- , pour augmenter la probabilité
 * d'obtenir une involution minimale dans son orbite
 * (expérimentalement cette probabilité devient alors supérieure à 1/3).
 *
 * La longueur est mesurée du premier brin vers le second;
 * calculer aussi la longueur du second vers le premier fait
 * perdre plus de temps, expérimentalement, qu'en gagner.
 *
 * Cette fonction retourne 1 en général (succès),
 * et 0 en cas d'échec, ie: 't' contient la dernière
 * involution dans l'ordre lexicographique.
 *
 * involution_suivante () suppose que l'argument 't'
 * contient les k premières cordes d'une involution;
 * le premier brin suffit pour la k-ème corde
 * (utile pour initialisation, avec seulement t[0]).
 */

int involution_suivante (int *t, int n) {
  int
    i, p, succes;

  for (i = n - 1; i >= 0; i--) {
    while ( i > 0 && t[i] <= 0 ) i--;
    p = i + t[i];
    if (i == 0 && p > n - 4)
      return 0;

    /* Allonger la corde i -> p */
    t[p++] = 0;
    p++;
    if (p < n) {
      succes = completer_involution (t, n, i, p);
      if (succes) return 1;
    }
    t[i] = 0;    /* échec: effacer la corde avant de passer à la précédente */
  }
  return 0; // pour faire plaisir au compilateur
}

/*
 * completer_involution () complète un tableau 't' de 'n' brins,
 * qui contient les k premières cordes d'une involution,
 * en la plus petite involution dans l'ordre lexicographique.
 *
 * 'i' est l'indice du premier brin libre, et 'p' l'indice minimal
 * de son jumeau; toutes les cordes sont de longueur impaire;
 * si 'groupe' est vrai, toutes les cordes sont de longueur au moins
 * égale à t[0] -- voir commentaire de involution_suivante () .
 *
 * Cette fonction retourne 1 en cas de succès, et 0 en cas d'échec,
 * ie: il n'existe pas d'involution qui complète 't'.
 */

static int completer_involution (int *t, int n, int i, int p) {

  int j, q, succes;

  /*
    printf ("completer_involution -> ");
    trace_t (t, n);
  */

  for (; p < n; p++) {
    if (t[p] == 0) {
      t[p] = - ( t[i] = p - i );

      /* Calculer l'indice 'j' du premier brin libre */

      for (j = i; j < n && t[j]; j++);
      if (j < n) {
        if (groupe)
          q = j + t[0];
        else
          q = j + 3;
        if (q < n) {
          succes = completer_involution (t, n, j, q);
          if (succes) return 1;
        }
        t[p] = 0;	/* échec: effacer la corde avant de l'allonger */
      }
      else
        return 1;	/* l'involution est complète */
    }
    p++;
  }
  t[i] = 0;    /* impossible de compléter l'involution à partir du brin i */
  return 0;
}

void involution_reverse (int *t, int *u, int n) {
  for (int i = 0, j = n - 1; i < n; i++, j--)
    u[i] = -t[j];
}

int minimum_cyclic (int *t, int n) {
  /*
   * Retourne l'indice i tel que t[i..n-1 0..i-1] soit minimal dans l'ordre
   * lexicographique, et place dans 'periode' la plus petite période de 't'.
   */
  int
    i = 0, p = 0,
    j = 1, q = 1,
    x, y,
    egalite = 0;

  /*
    for (int i = 0; i < n; i++)
      printf ("%3d",t[i]);
  */

  periode = n;
  while (1) {
    /*
     * Les candidats i' tels que i'<i ou i<i'<j ont été éliminés.
     * Début de la phase 1 pour comparer t[i:] et t[j:]
     */
    while (q < n) {
      /*
       * On compare les segments t[i..p] et t[j..q],
       * sachant que t[i..p-1]=t[j..q-1]
      */
      egalite = 0;
      x = t[p];
      if (x < 0)
        x = x + n;
      y = t[q];
      if (y < 0)
        y = y + n;

      if (x < y) {
        /*
         * q=++j serait naturel, mais j=++q est mieux
         * (juste pour le fun, car le gain de temps est marginal):
         * si j'<=q, on a t[i'..p]<t[j'..q] avec p-i'=q-j',
         * et le candidat j' est éliminé.
         */
        p = i;
        j = ++q;
      }
      else if (x == y) {
        p++;
        q++;
        egalite = 1;
      }
      else {
        /*
         * Le candidat i est éliminé, et remplacé par j
         */
        i = p = j;
        q = ++j;
      }
    } // while (q<n), fin de la phase 1

    if (!egalite) // dans ce cas x<y ou x>y, et j=q=n
      return i;

    /*
     * Phase 2: poursuivre la comparaison circulairement,
     * ie. comparer t[i..p] et t[j..n-1 0..q].
     */

    // printf (" [i=%d, p=%d, j=%d]", i, p, j);
    q = 0;
    while (p < n) {
      x = t[p];
      if (x < 0)
        x = x + n;
      y = t[q];
      if (y < 0)
        y = y + n;

      if (x < y)
        /*
         * Voir commentaire phase 1 : tous les candidats compris
         * circulairement entre j et q sont éliminés,
         * en particulier tous les candidats compris entre j et n-1.
         */
        return i;
      else if (x == y) {
        p++;
        q++;
      }
      else {
        /*
         * Même traitement que phase 1,
         * le candidat i est éliminé, et remplacé par j.
         */
        i = p = j;
        q = ++j;
        break;
      }
    } // while (p<n)

    /*
     * p<n: break après élimination du candidat i,
     * on retourne alors à une nouvelle phase 1
     */
    if (p < n) continue;

    /*
     * t[i..n-1] = t[j..n-1 0..q-1] donc q = j-i
     * est une période pour t[i..n-1 0..q-1].
     * Le tableau entier est périodique si i<q,
     * sinon décommenter le cas 'else' ci-dessous
     * pour obtenir des exemples intéressants à partir de n=9,
     * et se convaincre que 'return i' est correct:
     * le mot circulaire t[i:] est de la forme 'u^r v',
     * avec u=t[i:j] (notation Python], r>1 (car j<n),
     * et u<v (car v=t[i':i], et i' a été éliminé).
     */
    if (i < q)
      periode = q;
    /*
    else if (q > 1) {
      for (int i = 0; i < n; i++)
        printf ("%3d",t[i]);
      printf (" [i=%d, q=%d]\n", i, q);
    }
    */
    return i;
  } // while (1)
  return -1; // pour rassurer gcc
}

int is_minimal_cyclic (int *t, int n) {
  /*
   * Version simplifiée de minimum_cyclic, qui indique seulement
   * si l'involution est ou non minimale dans son orbite cyclique.
   */
  int
    i = 0, p = 0,
    j = 1, q = 1,
    x, y,
    egalite = 0;

  periode = n;
  while (q < n) {
    /*
     * On compare les segments t[i..p] et t[j..q],
     * sachant que t[i..p-1]=t[j..q-1]
    */
    egalite = 0;
    x = t[p];
    if (x < 0)
      x = x + n;
    y = t[q];
    if (y < 0)
      y = y + n;

    if (x < y) {
      p = i;
      j = ++q;
    }
    else if (x == y) {
      p++;
      q++;
      egalite = 1;
    }
    else {
      i = j;
      break;
    }
  } // while (q<n), fin de la phase 1

  if (i > 0) return 0;  // tableau non minimal
  if (!egalite) return 1;

  /*
   * Poursuivre la comparaison circulairement
   */
  q = 0;
  while (p < n) {
    x = t[p];
    if (x < 0)
      x = x + n;
    y = t[q];
    if (y < 0)
      y = y + n;

    if (x < y)
      return 1;
    else if (x == y) {
      p++;
      q++;
    }
    else
      return 0;
  } // while (p<n)
  periode = q;
  return 1;
}

int is_minimal_diedral (int *t, int n) {
  int i, j, p, q, u[BRIN_MAX];

  if (! is_minimal_cyclic(t, n)) return 0;
  involution_reverse(t, u, n);
  j = minimum_cyclic(u, n);
  delta = j;
  /*
   * Comparer t (minimal dans son orbite cyclique)
   * et u' = u décalé (minimal dans l'orbite cyclique de u).
   *
   * Si t est le plus petit, il est minimal diédral;
   * l'orbite cyclique de t et celle de u sont disjointes.
   * En cas d'égalité l'orbite diédrale est égale à l'orbite cyclique.
   * Enfin si t est le plus grand, on le rejette.
   */
  for (i = 0; i < n; i++) {
    p = i + t[i];
    q = j + u[j] - delta;
    if (q < 0) q += n;
    // printf ("(%d, %d)", p, q);
    if (p < q) {
      orbite = 2 * periode;
      return 1;
    }
    else if (q < p)
      return 0;
    j++;
    if (j >= n) j = 0;
  }
  orbite = periode;
  return 1;
}

int is_chiral_cyclic (struct Diagramme *d) {
  /*
   * On suppose que la fonction `orienter()` a calculé les orientations des brins,
   * avec le brin 0 orienté positivement par convention.
   * On suppose aussi que la période a été calculée par `minimum_cyclic()`
   * ou sa soeur `is_minimal_cyclic()`.
   * Le diagramme `d` comporte n cordes, avec n = `d->taille`.
   */
  if (periode > d->taille)  // orbite cyclique maximale, de taille 2n
    return 1;
  return d->brin[periode].orientation > 0;
}

int is_chiral_diedral (struct Diagramme *d) {
  /*
   * On suppose que la période cyclique et la taille de l'orbite
   * diédrale ont été calculées par `is_minimal_diedral()`.
   */
  if (!is_chiral_cyclic(d)) return 0;
  if (periode < orbite)  // une orbite diédrale regroupe deux orbites cycliques
    return 1;
  /*
   * En lisant le diagramme de cordes à l'envers, delta désigne l'indice
   * de départ de l'involution minimale, cf `is_minimal_diedral()`.
   */
  return d->brin[periode - delta - 1].orientation > 0;
}

/* unused */
void rotation (struct Diagramme *d) {
  /*
   * Effectue une rotation sur le diagramme: la nouvelle origine
   * (brin 0) est le brin 1 du diagramme de départ.
   */
  int
    i,
    n = 2 * d->taille,
    p = d->brin[0].longueur;
  struct Brin b;

  d->brin[0].longueur = - (d->brin[p].longueur = n - p);
  b = d->brin[0];
  for (i = 1; i < n; i++)
    d->brin[i - 1] = d->brin[i];
  d->brin[n - 1] = b;
}
