Oriented curves with 6 crossings:

    [Release]./TaitCurves -cg 6
    22 involutions construites => 10 minimale(s) 3 réalisable(s) 3 chirale(s)

Option 'c' (cyclic) means that diagrams are grouped according to the action of the cyclic group, and option 'g' (gauss) means that we select *realizable* diagrams, i.e. that describe plane curves.
Output shows that only 22 diagrams (i.e. involutions) were built (the other ones had no chance to be canonical representatives of their cyclic class), corresponding to 10 cyclic classes; 3 of them represent plane curves, all chirals.
Option 'z' (zuber) display results with notations of the article CZ (Coquereaux, Zuber) *Maps, immersions and permutations*, http://arxiv.org/abs/1507.03163:

    [Release]./TaitCurves -cgz 6
           3 OU-immersions, 6 OO-immersions

(the first 'O' is for Oriented curves, the second symbol U/O is for an Unoriented/Oriented sphere); chiral curves are not equivalent to their mirror.

Option 'l' (list) display each diagram, as a sequence of chord *lengths*, followed by orientations of the corresponding crossings:

    [Release]./TaitCurves -cgl 6
      3  5  5  9  5  5  7  7  3  7  7  9
      1 -1  1 -1 -1  1  1 -1 -1  1 -1  1
    Période 12
      3  7  5  9  5  5  5  7  5  7  7  7
      1  1 -1 -1  1 -1  1  1 -1 -1  1 -1
    Période 12
      3  7  5  9  7  5  3  7  5  9  7  5
      1  1 -1 -1  1 -1  1  1 -1 -1  1 -1
    Période  6

The last sequence `3 7 5 9 ...` corresponds to the consecutive chords (1,4), (2,9), (3,8), (4,1) etc. In other words, for each chord (i,j) of length k we have j=i+k, with addition mod 12 when j<i.

For enumeration of unoriented curves, use option 'd' (diedral) instead of 'c' (cyclic):

    [Release]./TaitCurves -dgz 6
           3 UU-immersions, 4 UO-immersions
    [Release]./TaitCurves -cgz 7
         11 OU-immersions, 17 OO-immersions
    [Release]./TaitCurves -dgz 7
         10 UU-immersions, 12 UO-immersions

Option 'k' (dowKer) adds *Dowker-Thistlethwaite (DT) notation* (without signs, we study curves, not knots) to lists:

    [Release]./TaitCurves -dgk 6
      3  5  5  9  5  5  7  7  3  7  7  9 DT:  4  8 10  2 12  6
      1 -1  1 -1 -1  1  1 -1 -1  1 -1  1
    Période 12, orbite diédrale de taille 12
      3  7  5  9  5  5  5  7  5  7  7  7 DT:  4  8 10 12  2  6
      1  1 -1 -1  1 -1  1  1 -1 -1  1 -1
    Période 12, orbite diédrale de taille 12, achiral
      3  7  5  9  7  5  3  7  5  9  7  5 DT:  4  8 12 10  2  6
      1  1 -1 -1  1 -1  1  1 -1 -1  1 -1
    Période  6, orbite diédrale de taille  6, achiral
    22 involutions construites => 9 minimale(s) 3 réalisable(s) 1 chirale(s)

First result above is *Figure 3* of CZ; this is the only chiral curve with 6 crossings, but when number of crossings is large, most of curves are chiral; therefore only achiral curves are reported as such in lists similar to the above one.
The 27 indecomposable irreducible immersions of an unoriented circle in an unoriented sphere with n=8 double points (*Figure 15* in CZ) may be listed by `./TaitCurves -dgk 8` (with a different ordering).

Option 's' (statistics) displays the repartition of diagrams according to orbits (cyclic or diedral):

    [Release]./TaitCurves -cgs 8
    876 involutions construites => 308 minimale(s) 38 réalisable(s) 35 chirale(s)
           1 diagramme(s) de période cyclique  2
           2 diagramme(s) de période cyclique  4
           4 diagramme(s) de période cyclique  8
          31 diagramme(s) de période cyclique 16
         538 involution(s)
    [Release]./TaitCurves -dgs 8
    876 involutions construites => 198 minimale(s) 27 réalisable(s) 14 chirale(s)
           1 orbite(s) diédrale(s) de taille  2
           2 orbite(s) diédrale(s) de taille  4
           2 orbite(s) diédrale(s) de taille  8
          12 orbite(s) diédrale(s) de taille 16
          10 orbite(s) diédrale(s) de taille 32
         538 involution(s)
