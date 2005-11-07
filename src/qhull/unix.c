/* unix.c -- Unix version of qhull

   see README
   
   copyright (c) 1993, 1997, The Geometry Center
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "qhull.h"
#include "mem.h"
#include "set.h"

#if __MWERKS__ && __POWERPC__
#include <SIOUX.h>
#include <Files.h>
#include <console.h>
#include <Desk.h>

#elif __cplusplus
extern "C" {
  int isatty (int);
}

#elif _MSC_VER
#include <io.h>
#define isatty _isatty

#else
int isatty (int);  /* returns 1 if stdin is a tty
		   if "Undefined symbol" this can be deleted along with call in main() */
#endif


char qh_version[]= "version 2.4 97/4/2";/*Changes Announce README man make news*/
/* also change concise prompt below */
char qh_prompta[]= "\n\
qhull- compute the convex hull of a set of n-d points.\n\
    <http://www.geom.umn.edu/locate/qhull>  %s\n\
input (stdin):\n\
    1st lines= dimension #points (or vice-versa).   rest= point coordinates\n\
    first comment (non-numeric) is listed in the summary\n\
    halfspace: use dim plus one and put offsets after coefficients.\n\
        May be preceeded by a single interior point ('H').\n\
\n\
options:\n\
    d    - Delaunay triangulation by lifting points to a paraboloid\n\
    v    - Voronoi vertices via the Delaunay triangulation\n\
    Hn,n,... - halfspace intersection about [n,n,0,...]\n\
    d Qu   - furthest-site Delaunay triangulation (upper convex hull)\n\
    v Qu   - furthest-site Voronoi vertices and regions\n\
\n\
  Qopts- Qhull control options:\n\
           Qbk:n   - scale coord k so that low bound is n\n\
             QBk:n - scale coord k so that upper bound is n (QBk is %2.2g)\n\
           QbB     - scale input to unit cube\n\
           Qbb     - scale last coordinate to [0,m] for Delaunay triangulations\n\
           Qbk:0Bk:0 - remove k-th coordinate from input\n\
           QRn     - random rotation (n=seed, n=0 time, n=-1 time/no rotate)\n\
%s%s%s%s";  /* split up qh_prompt for Visual C++ */
char qh_promptb[]= "\
           Qc      - keep coplanar points with nearest facet\n\
           Qf      - partition point to furthest outside facet\n\
           Qg      - only build good facets (needs 'QGn', 'QVn', or 'PdD')\n\
           Qm      - only process points that would increase max_outside\n\
           Qi      - keep interior points with nearest facet\n\
           Qr      - process random outside points instead of furthest ones\n\
           Qs      - search all points for the initial simplex\n\
           Qu      - for 'd', compute upper hull without point at-infinity\n\
                     returns furthest-site Delaunay triangulation\n\
           Qv      - test vertex neighbors for convexity\n\
           Qx      - exact pre-merges (skips coplanar and angle coplanar facets)\n\
           Qz      - add point-at-infinity to Delaunay triangulation\n\
           QGn     - good facet if visible from point n, -n for not visible\n\
           QVn     - good facet if it includes point n, -n if not\n\
           Q0      - turn off default premerge with 'C-0'/'Qx'\n\
           Q1	   - sort merges by type instead of angle\n\
           Q2      - merge all non-convex at once instead of independent sets\n\
           Q3      - do not merge redundant vertices\n\
           Q4      - avoid old->new merges\n\
           Q5      - do not correct outer planes at end of qhull\n\
           Q6      - do not pre-merge concave or coplanar facets\n\
           Q7      - depth-first processing instead of breadth-first\n\
           Q8      - do not process near-inside points\n\
           Q9      - process furthest of furthest points\n\
";
char qh_promptc[]= "\
  Topts- Trace options:\n\
           T4      - trace at level n, 4=all, 5=mem/gauss, -1= events\n\
           Tc      - check frequently during execution\n\
           Ts      - print statistics\n\
           Tv      - verify result: structure, convexity, and point inclusion\n\
           Tz      - send all output to stdout\n\
           TFn     - report summary when n or more facets created\n\
           TPn     - turn on tracing when point n added to hull\n\
            TMn    - turn on tracing at merge n\n\
            TWn    - trace merge facets when width > n\n\
           TVn     - stop qhull after adding point n, -n for before (see TCn)\n\
            TCn    - stop qhull after building cone for point n (see TVn)\n\
\n\
Precision options (default detects precision problems without correction):\n\
    Cn   - radius of centrum (roundoff added).  Merge facets if non-convex\n\
     An  - cosine of maximum angle.  Merge facets if cosine > n or non-convex\n\
           C-0 roundoff, A-0.99/C-0.01 pre-merge, A0.99/C0.01 post-merge\n\
    En   - max roundoff error for distance computation\n\
    Rn   - randomly perturb computations by a factor of [1-n,1+n]\n\
    Vn   - min distance above plane for a visible facet (default 3C-n or En)\n\
    Un   - max distance below plane for a new, coplanar point (default Vn)\n\
    Wn   - min facet width for outside point (before roundoff, default 2Vn)\n\
";
char qh_promptd[]= "\n\
Output formats (may be combined; if none, produces a summary to stdout):\n\
    f    - all fields of all facets\n\
    i    - vertices incident to each facet (centers for Voronoi)\n\
    m    - Mathematica output (2-d and 3-d)\n\
    o    - OFF file format (dim, points and facets; Voronoi cells)\n\
    n    - normals with offsets\n\
    p    - point coordinates (Voronoi centers for option 'v')\n\
    s    - print summary to stderr\n\
    Fopts- additional input/output formats:\n\
    	   Fa      - print area for each facet\n\
           FA      - compute total area and volume for option 's'\n\
           Fc      - print count and coplanar points for each facet\n\
           FC      - print centrum or Voronoi center for each facet\n\
           Fd      - use cdd format for input (homogeneous with offset first)\n\
           FD      - use cdd format for normals (offset first)\n\
           FF      - print facets w/o ridges\n\
           Fi      - print inner planes for each facet\n\
           FI      - print id for each facet\n\
           Fm      - print merge count for each facet (511 max)\n\
           Fn      - print count and neighbors for each facet\n\
           FN      - print count and vertex neighbors for each point\n\
           Fo      - print outer planes (or max_outside) for each facet\n\
           FO      - print all options\n\
           Fp      - print point for each halfspace intersection\n\
           FP      - print nearest vertex for each coplanar point (v,p,f,d)\n\
           FQ      - print Qhull and input command\n\
           Fs      - print summary: #int (6), dimension, #points,\n\
                       tot vertices, tot facets, #vertices in output, #facets\n\
                       #real (2), max outer plane and min vertex\n\
           FS      - print sizes: #int (0), #real(2) tot area, tot vol\n\
           Ft      - print Delaunay triangulation with added centrums\n\
           Fv      - print count and vertices for each facet\n\
           FV      - print average vertex (feasible point for 'H')\n\
";
char qh_prompte[]= "\
    Gopts- Geomview output (2-d, 3-d and 4-d; 2-d Voronoi)\n\
           Ga      - all points as dots\n\
            Gp     -  coplanar points and vertices as radii\n\
            Gv     -  vertices as spheres\n\
           Gi      - inner planes only\n\
            Gn     -  no planes\n\
            Go     -  outer planes only\n\
           Gc	   - centrums\n\
           Gh      - hyperplane intersections\n\
           Gr      - ridges\n\
           GDn     - drop dimension n in 3-d and 4-d output\n\
           Gt      - transparent outer ridges to view 3-d Delaunay\n\
    Popts- Print options:\n\
           PAn     - keep n largest facets by area\n\
           Pdk:n   - drop facet if normal[k] <= n (default 0.0)\n\
           PDk:n   - drop facet if normal[k] >= n\n\
           Pg      - print good facets (needs 'QGn' or 'QVn')\n\
           PFn     - keep facets whose area is at least n\n\
           PG      - print neighbors of good facets\n\
           PMn     - keep n facets with most merges\n\
           Po      - force output.  If error, output neighborhood of facet\n\
           Pp      - do not report precision problems\n\
\n\
    .    - list of all options\n\
    -    - one line descriptions of all options\n\
";
/* for opts, don't assign 'e' or 'E' to a flag (already used for exponent) */

char qh_prompt2[]= "\n\
qhull- compute the convex hull.  %s\n\
    input (stdin): dimension, #points, point coordinates\n\
    first comment (non-numeric) is listed in the summary\n\
    halfspace: use dim plus one and put offsets after coefficients\n\
\n\
options (qh-opt.htm):\n\
    d      - Delaunay triangulation by lifting points to a paraboloid\n\
    v      - Voronoi vertices and regions via the Delaunay triangulation\n\
    H1,1   - Halfspace intersection about [1,1,0,...] via polar duality\n\
    d Qu   - furthest-site Delaunay triangulation (upper convex hull)\n\
    v Qu   - furthest-site Voronoi vertices and regions\n\
    .      - concise list of all options\n\
    -      - one-line description of all options\n\
\n\
output options (subset):\n\
    s      - (default) summary to stderr\n\
    Ft     - triangulated output with centrums\n\
    o      - OFF file format (if Voronoi, outputs regions)\n\
    i      - vertices incident to each facet\n\
    n      - normals with offsets\n\
    p      - vertex coordinates (Voronoi vertex coordinates)\n\
    Fp     - halfspace intersection coordinates\n\
    FA     - compute total area and volume\n\
    G      - Geomview output (2-d, 3-d and 4-d)\n\
    m      - Mathematica output (2-d and 3-d)\n\
    Tv     - verify result: structure, convexity, and point inclusion\n\
    f      - print all fields of all facets\n\
\n\
example:\n\
    rbox 1000 s | qhull Tv s FA\n\
\n\
";
/* for opts, don't assign 'e' or 'E' to a flag (already used for exponent) */

char qh_prompt3[]= "\n\
Qhull %s.  Upper-case options [not 'F.' or 'PG'] use a number\n\
\n\
 delaunay       voronoi	       Halfspace      facet_dump     Geomview \n\
 incidences     mathematica    normals        points         off_file\n\
 summary\n\
 Farea          FArea-total    Fcoplanars     FCentrums      Fd-cdd-in\n\
 FD-cdd-out     FFacet-xridge  Finner         FIds           Fmerges\n\
 Fneighbors     FNeigh-vertex  Fouter         FOptions       Fpoint-intersect\n\
 FPoint_near    FQhull         Fsummary       FSize          Ftriangles\n\
 Fvertices      FVertex-ave\n\
 Gvertices      Gpoints        Gall_points    Gno_planes     Ginner\n\
 Gcentrums      Ghyperplanes   Gridges        Gouter         GDrop_dim\n\
 Gtransparent   PArea-keep     Pdrop d0:0D0   Pgood          PFacet_area_keep\n\
 PGood_neighbors PMerge-keep   Poutput_forced Pprecision_not\n\
 QbBound 0:0.5  QbB-scale-box  Qbb-scale-last Qcoplanar\n\
 Qfurthest      Qgood_only     QGood_point    Qinterior\n\
 Qmax_out       Qrandom        QRotate        Qsearch_1st    QupperDelaunay\n\
 QVertex_good   Qvneighbors    Qxact_merge    Qzinfinite     Q0_no_premerge\n\
 Q1_no_angle    Q2_no_independ Q3_no_redundant Q4_no_old     Q5_no_check_out\n\
 Q6_no_concave  Q7_depth_first Q8_no_near_in  Q9_pick_furthest T4_trace\n\
 Tcheck_often   Tstatistics    Tverify        Tz_stdout      TFacet_log\n\
 TPoint_trace   TMerge_trace   TWide_trace    TVertex_stop   TCone_stop\n\
 Angle_max      Centrum_size\n\
 Error_round    Random_dist    Visible_min    Ucoplanar_max  Wide_outside\n\
";

/*-------------------------------------------------
-main- processes the command line, calls qhull() to do the work, and exits
*/
int main(int argc, char *argv[]) {
  int curlong, totlong, exitcode, numpoints, dim;
  coordT *points;
  boolT ismalloc;

#if __MWERKS__ && __POWERPC__
  char inBuf[BUFSIZ], outBuf[BUFSIZ], errBuf[BUFSIZ];
  SIOUXSettings.showstatusline= false;
  SIOUXSettings.tabspaces= 1;
  SIOUXSettings.rows= 40;
  if (setvbuf (stdin, inBuf, _IOFBF, sizeof(inBuf)) < 0   /* w/o, SIOUX I/O is slow*/
  || setvbuf (stdout, outBuf, _IOFBF, sizeof(outBuf)) < 0
  || (stdout != stderr && setvbuf (stderr, errBuf, _IOFBF, sizeof(errBuf)) < 0)) 
    fprintf (stderr, "qhull internal warning (main): could not change stdio to fully buffered.\n");
  argc= ccommand(&argv);
#endif

  if ((argc == 1) && isatty( 0 /*stdin*/)) {      
    fprintf(stdout, qh_prompt2, qh_version);
    exit(qh_ERRnone);
  }
  if (argc > 1 && *argv[1] == '-' && !*(argv[1]+1)) {
    fprintf(stdout, qh_prompta, qh_version, qh_DEFAULTbox, 
		qh_promptb, qh_promptc, qh_promptd, qh_prompte);
    exit(qh_ERRnone);
  }
  if (argc >1 && *argv[1] == '.' && !*(argv[1]+1)) {
    fprintf(stdout, qh_prompt3, qh_version);
    exit(qh_ERRnone);
  }
  qh_init_A (stdin, stdout, stderr, argc, argv);  /* sets qh qhull_command */
  exitcode= setjmp (qh errexit); /* simple statement for CRAY J916 */
  if (!exitcode) {
    qh_initflags (qh qhull_command);
    points= qh_readpoints (&numpoints, &dim, &ismalloc);
    qh_init_B (points, numpoints, dim, ismalloc);
    qh_qhull();
    qh_check_output();
    qh_produce_output();
    if (qh VERIFYoutput && !qh FORCEoutput && !qh STOPpoint && !qh STOPcone)
      qh_check_points();
    exitcode= qh_ERRnone;
  }
  qh NOerrexit= True;  /* no more setjmp */
#ifdef qh_NOmem
  qh_freeqhull( True);
#else
  qh_freeqhull( False);
  qh_memfreeshort (&curlong, &totlong);
  if (curlong || totlong) 
    fprintf (stderr, "qhull internal warning (main): did not free %d bytes of long memory (%d pieces)\n",
       totlong, curlong);
#endif
  return exitcode;
} /* main */

