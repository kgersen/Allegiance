Name

      qhull, rbox         Version 2.4     April 2, 1997
  
Convex hull, Delaunay triangulation, Voronoi vertices, Halfspace intersection
 
      Available from:
        <ftp://geom.umn.edu/pub/software/qhull.tar.Z>
        <http://www.geom.umn.edu/locate/qhull>

      Version 1 (simplicial only):
        <ftp://geom.umn.edu/pub/software/qhull-1.0.tar.Z>
        <ftp://geom.umn.edu/pub/software/qhull.sit.hqx>
       
      News and executables:
        <http://www.geom.umn.edu/~bradb/qhull-news.html>

Purpose

  The files in this directory are part of qhull-software. The
  directory contains two programs, qhull and rbox.  Qhull is a general
  dimension convex hull program that reads a set of points from stdin,
  and outputs the smallest convex set that contains the points to
  stdout according to printout options.  It also generates Delaunay
  triangulations, Voronoi vertices, furthest-site Voronoi vertices,
  and halfspace intersections.  

  Rbox is a useful tool in generating input for Qhull; it generates 
  hypercubes, diamonds, cones, circles, spirals, and random points.
  
  Qhull produces graphical output for Geomview.  This helps with
  understanding the output.  See <http://www.geom.umn.edu/locate/geomview>
    
Environment requirements

  Qhull and rbox should run on all 32-bit and 64-bit computers.  An ANSI C 
  compiler is needed to compile the files.  The software is self-contained.
  It is written as a Unix program.
  
  Qhull is copyrighted software.  Please read COPYING.txt and REGISTER.txt
  before using or distributing Qhull.


Zip file for Win32

  The zip file from qhull-news.html includes Qhull documentation and 
  executables.  Download qhull.tar.Z for sources and an article on Qhull
  [double check that the file ends in '.tar.Z'].
  
  To install Qhull:
  - Create a directory for Qhull and unzip the file.  
  - Open a DOS window for the directory.  You may use startup.bat.  
  - Use 'pwd' to double-check that you are in the Qhull directory.  
  - Execute 'qhull' for a synopsis.
    
Compiling for Win32

  Qhull compiles as a console application in Visual C++ 5.0 at warning 
  level 3.  Make a project of all files except for rbox.c and user_eg.c.  
  Make another project for rbox.c.  In Project Settings/C++/Customize,
  mark 'Disable language extensions'.  This enables the __STDC__ flag. 

  Qhull compiles with Borland C++ 5.0 bcc32.  A Makefile is included 
  (make -f MBorland).
  
  Qhull compiles with Borland C++ 4.02 for Win32 and DOS Power Pack.  
  Use 'make -f MBorland -D_DPMI'.  Qhull 1.0 compiles with Borland 
  C++ 4.02.  For rbox 1.0, use "bcc32 -WX -w- -O2-e -erbox -lc rbox.c".  
  Use the same options for Qhull 1.0. [D. Zwick]
  
  Qhull compiles with Metrowerks C++ 1.7.

  Windows DOS box merges stderr and stdout. 
  
  If you turn on full warnings, the compiler will report a number of 
  unused variables, variables set but not used, and dead code.  These are
  intentional.  For example, variables may be initialized (unnecessarily)
  to prevent warnings about possible use of initialized variables.  
   
Compiling for the Power Macintosh

  Qhull compiles for the Power Macintosh with Metrowerk's C compiler.
  It uses the SIOUX interface to read point coordinates and return output.
  There is no graphical output.  Without options, qhull returns an abbreviated
  synopsis.  Instead of using SIOUX, Qhull may be embedded within an application.  
  The distribution includes project files for qhull and rbox.

  Version 1 is available for Macintosh computers by ftp of qhull.sit.hqx
  It reads point coordinates from a standard file and returns output
  to a standard file.  There is no graphical output.

Compiling for Unix

  - in Makefile, check the CC, CCOPTS1, PRINTMAN, and PRINTC defines
  - in user.h, decide whether you want double-precision or single-precision
      reals (REALfloat).
  - in user.h, check the definitions of qh_SECticks and qh_CPUclock.  
      Your system may use different functions.
  - type: make doc
      This prints the documentation.  qh-man.htm is the html manual
  - For Unix-style documentation, type 'make qhull.1; make rbox.1'
  - type: make
      This builds: qhull rbox libqhull.a
  - if your compiler reports many errors, it is probably not a ANSI C compiler
      - you will need to set the -ansi switch or find another compiler
  - if your compiler warns about missing prototypes for fprintf() etc.
      - this is ok, your compiler should have given these in stdio.h
  - if your compiler warns about missing prototypes for memset() etc.
      - include memory.h in qhull_a.h
  - if your compiler reports "global.c: storage size of 'qh_qh' isn't known"
      - delete the initializer "={0}" in global.c, stat.c and mem.c
  - if your compiler warns about "stat.c: improper initializer"
      - this is ok, the initializer is not used
  - if you have trouble building libqhull.a with 'ar'
      - try 'make qhullx' 
  - if the code compiles, the qhull test case will automatically execute
      - if an error occurs, there's an incompatibility between machines
      - if you can, try a different compiler 
      - you can turn off the Qhull memory manager with qh_NOmem in mem.h
      - you can turn off compiler optimization (-O2 in Makefile)
      - if you find the source of the problem, please let us know
  - if you have Geomview, 
       - try  'rbox 100 | qhull G >a' and load 'a' into Geomview
       - run 'q_eg' for Geomview examples of Qhull output (see qh-eg.htm)
  - to install qhull, rbox, and their man pages:
      - define MANDIR and BINDIR
      - type 'make install'

Compiling for other machines
 
  If the compiler reports that memset() is undefined, include memory.h
  in qhull_a.h

  Some users have reported problems with compiling Qhull under Irix 5.1.  It
  compiles under other versions of Irix. 
  
  If you have troubles with the memory manager, you can turn it off by
  defining qh_NOmem in mem.h.

  You may compile Qhull with a C++ compiler.  

Authors:

  C. Bradford Barber                    Hannu Huhdanpaa
  bradb@geom.umn.edu                    hannu@geom.umn.edu
  
                    c/o The Geometry Center
                    University of Minnesota
                    1300 South Second Street, Suite 500
                    Minneapolis, MN 55454
  
  This software was developed under NSF grants NSF/DMS-8920161 and
  NSF-CCR-91-15793 750-7504 at the Geometry Center and Harvard 
  University.  If you find Qhull useful, please let us know.

Product consists of:
  README.txt         /* instructions for installing Qhull */
  REGISTER.txt       /* Qhull registration */
  COPYING.txt        /* copyright notice */
  Announce.txt       /* announcement */
  Changes.txt        /* change history for Qhull and rbox */
  Makefile           /* Makefile for Unix 'make' */
  qhull-PPC.sit.bin  /* projects for Metrowerk's C/C++ */
  MBorland           /* Makefile for Borland C++ for Win32 */
  rbox consists of:
     qh-rbox.htm     /* html manual */
     rbox.man        /* Unix man page */
     rbox.txt
     rbox.c          /* source program */
  qhull consists of:
     qh-man.htm      /* html manual */
     qh-in.htm
     qh-impre.htm
     qh-opt*.htm
     qh-eg.htm
     qh--4d.gif,etc. /* images for manual */
     qhull.man       /* Unix man page */
     qhull.txt
     q_eg            /* shell script to generate Geomview examples */
     q_egtest        /* shell script to generate Geomview test examples */
     q_test          /* shell script to test qhull */
     toms96-12.ps    /* article about qhull */
     user.h          /* header file of user definable constants */
     qhull.h         /* header file for qhull, defines data structures */
     unix.c          /* Unix front end to qhull */
     qhull.c         /* Quickhull algorithm with partitioning */
     user.c          /* user re-definable functions */
     user_eg.c       /* example of incorporating qhull into a user program */

     qhull_a.h       /* include file for *.c */
     geom.c          /* geometric routines */
     geom2.c
     geom.h	
     global.c        /* global variables */
     io.c            /* input/output routines */
     io.h
     mem.c           /* memory routines, this is stand-alone code */
     mem.h
     merge.c         /* merging of non-convex facets */
     merge.h
     poly.c          /* polyhedron routines */
     poly2.c
     poly.h 
     set.c           /* set routines, this only depends on mem.c */
     set.h
     stat.c          /* statistics */
     stat.h

