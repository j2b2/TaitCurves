This is C code, written first in 1994, to enumerate plane immersions of a circle.
These curves are planar maps, where each vertex has degree four, and are used, inter alia, as knot projections.
Let *n* be the number or vertices, or crossings: there is a well known coding of the curve by an involution on integers from 1 to *2n*:
choose a basepoint and an orientation, and travel along the curve while numbering the edges with 1,2,3... Each crossing corresponds to two of these numbers, and Gauss noticed that these pairs are always made of an even and an odd integer.
See e.g. HTW (Hoste, Thistlethwaite, Weeks: *The First 1,701,935 Knots*, 1998), figure 1;
see also the related *problème des ménages*.

This involution may be drawn as a *chord diagram* (or *Gauss diagram*), where *2n* points are marked on a circle, and each pair (i,j) is drawn as a line between two of these points.
I decided to refer to the curve itself as a *Tait curve*, because the meaning of *Gauss curve* is quite different, but this is a (very) personal terminology.

Here are three files (not including a trivial header file):
  - `cordes.c` contains the usual `main()` function, and auxiliary ones, to display results of the computations; this is the tedious and obsolete part of a classical *C interface*;
  - `diag.c` contains mainly the following functions:
   - `involution_suivante()` (most of names of variables and functions are french, as are comments inside the code, sorry), used to iterate through involutions;
   - `minimum_cyclic()` and `is_minimal_cyclic()` used to select a canonical representation of an *oriented* curve (we may change the basepoint before travelling along the circle); I tried to optimize these functions, which should be as fast as possible;
   - `is_minimal_diedral()` used to select a canonical representation of an *unoriented* curve (we may change the direction before travelling along the circle);
   - `is_chiral_cyclic()` and`is_chiral_diedral()`.
  - `orientation.c` contains functions to compute orientation of crossings: the algorithm is not trivial, and is the essential part of this work. Diagrams with a coherent orientation are said *realizables*, and for those that are *indecomposable*, this orientation is unique, up to the orientation of the plane (or the sphere), see e.g. HTW.

History: I published results of computations carried out using the first version of this code as OEIS sequence A007756, which enumerates (prime, aka indecomposable) immersions of an *oriented* circle.
Later article HTW was published, where such computations were a mere warmup, before much more sophisticated ones for classifying knots; in an email (1999) Jim Hoste sent me the sequence that enumerates *unoriented* immersions, up to 17 crossings, but he didn't publish it.
I added the (short) function `is_minimal_diedral()` to my code, and got the same numbers, up to 14 crossings.

Robert Coquereaux and Jean-Bernard Zuber (aka CZ) wrote recently (2015) a paper (*Maps, immersions and permutations*, http://arxiv.org/abs/1507.03163), where they systematically study and enumerate, using methods of group theory, numerous classes of immersions in arbitrary genus.
They asked whether my program could compute numbers given in their *table 7* (counting of irreducible indecomposable spherical immersions);
answer was positive -- I added short functions `is_chiral_cyclic()` and`is_chiral_diedral()` in `diag.c`, and they kindly encouraged me to publish my code.
