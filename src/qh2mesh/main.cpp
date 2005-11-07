#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>

void    translate(const char*   file);;

int	main(int argc, char** argv)
{
    for (int i = 1; (i < argc); i++)
        translate(argv[i]);

    return 0;
}

struct  Vertex
{
    Vertex(void)
    :
        cUsed(0)
    {
    }
    
    double  xyz[3];
    int     id;
    int     cUsed;
};

struct  Face
{
    int ids[3];
};

void    swap(double g[3][4], int i, int j)
{
    for (int k = 0; (k < 4); k++)
    {
        double  t = g[i][k];
        g[i][k] = g[j][k];
        g[j][k] = t;
    }
}

void    subtract(double g[3][4], double f, int i, int j)
{
    for (int k = 0; (k < 4); k++)
    {
        g[i][k] -= g[j][k] * f;
    }
}
bool    reduce(double   g[3][4])
{
    if (fabs(g[1][0]) > fabs(g[0][0]))
        swap(g, 0, 1);

    if (fabs(g[2][0]) > fabs(g[0][0]))
        swap(g, 0, 2);

    if (fabs(g[0][0]) < 1.0e-4)
        return false;

    //Reduce the first column
    subtract(g, g[1][0] / g[0][0], 1, 0);
    g[1][0] = 0.0;
    subtract(g, g[2][0] / g[0][0], 2, 0);
    g[2][0] = 0.0;

    if (fabs(g[2][1]) > fabs(g[1][1]))
        swap(g, 1, 2);

    if (fabs(g[1][1]) < 1.0e-4)
        return false;

    //Reduce the second column
    subtract(g, g[2][1] / g[1][1], 2, 1);
    g[2][1] = 0.0;

    //Back solve ...
    if (fabs(g[2][2]) < 1.0e-4)
        return false;

    g[2][3] = g[2][3] / g[2][2];
    g[2][2] = 1.0;

    subtract(g, g[1][2], 1, 2);
    g[1][2] = 0.0f;
    subtract(g, g[0][2], 0, 2);
    g[0][2] = 0.0f;

    assert (g[1][1] != 0.0f);
    g[1][3] = g[1][3] / g[1][1];
    g[1][1] = 1.0;
    subtract(g, g[0][1], 0, 1);

    assert (g[0][0] != 0.0f);
    g[0][3] = g[0][3] / g[0][0];
    g[0][0] = 1.0;

    return true;
}

const int   c_maxVertices = 10000;
const int   c_maxFaces = 1000;

Vertex      vertices[c_maxVertices];
Face        faces[c_maxFaces];
int         vertexIDs[c_maxVertices];

void    addAdjacent(int*    nAdjacent,
                    int     adjacent[],
                    int     id)
{
    if (vertices[id].cUsed > 2)
    {
        for (int i = 0; (i < *nAdjacent); i++)
        {
            if (adjacent[i] == id)
                return;
        }

        adjacent[(*nAdjacent)++] = id;
    }
}

void    translate(const char*   file)
{
    FILE*   fIn = NULL;
    FILE*   fOut1 = NULL;
    FILE*   fOut2 = NULL;

    if (strcmp(file, "-") == 0)
    {
        fIn = stdin;
        fOut1 = stdout;
    }
    else
    {
        const int   c_cbBfr = 512;
        char    bfr[c_cbBfr];
        assert (strlen(file) < c_cbBfr - 10);
        strcpy(bfr, file);
        strcat(bfr, ".qh");

        fIn = fopen(bfr, "r");

        if (fIn)
        {
            strcpy(bfr, file);
            strcat(bfr, "_m.x");

            fOut1 = fopen(bfr, "w");

            strcpy(bfr, file);
            strcat(bfr, ".cvh");

            fOut2 = fopen(bfr, "w");
        }
    }

    int         nVertices = 0;
    int         nUsedVertices = 0;
    int         nSingularFaces = 0;
    int         nFaces = 0;

    if (fIn)
    {
        const int   c_cbLine = 512;
        char        line[c_cbLine];

        fgets(line, c_cbLine, fIn); //Skip 1st line
        fgets(line, c_cbLine, fIn);
        sscanf(line, "%d %d", &nVertices, &nFaces);

        //Read in all of the vertices
        {
            for (int i = 0; (i < nVertices); i++)
            {
                fgets(line, c_cbLine, fIn);

                sscanf(line, "%lf %lf %lf",
                       &(vertices[i].xyz[0]),
                       &(vertices[i].xyz[1]),
                       &(vertices[i].xyz[2]));
            }
        }

        //Read in all of the faces
        {
            int faceID = 0; //possibly more than one face/line
            for (int i = 0; (i < nFaces); i++)
            {
                fgets(line, c_cbLine, fIn);

                int n = atoi(line);

                const int   c_maxIDs = 20;
                assert (n >= 3);
                assert (n < c_maxIDs);

                int ids[c_maxIDs];

                {
                    const char* p = line;

                    for (int j = 0; (j < n); j++)
                    {
                        p = strchr(p, ' ');
                        assert (p);

                        ids[j] = atoi(++p);
                    }
                }

                if ((n & 0x01) == 1)
                {
                    //Odd number of vertices
                    //Remove the last vertex from consideration
                    faces[faceID].ids[0] = ids[n-2];
                    faces[faceID].ids[1] = ids[n-1];
                    faces[faceID].ids[2] = ids[0];
                    faceID++;

                    n--;
                }

                assert ((n & 0x01) == 0);

                int p0 = 0;
                int p1 = 1;
                int p2 = n - 2;
                int p3 = n - 1;
                while (p2 > p1)
                {
                    faces[faceID].ids[0] = ids[p0];
                    faces[faceID].ids[1] = ids[p1];
                    faces[faceID].ids[2] = ids[p2];
                    faceID++;

                    faces[faceID].ids[0] = ids[p2];
                    faces[faceID].ids[1] = ids[p3];
                    faces[faceID].ids[2] = ids[p0];
                    faceID++;

                    p0++;
                    p1++;
                    p2--;
                    p3--;
                }
            }

            nFaces = faceID;
        }

        //Mark the vertices actually used by faces
        {
            for (int i = 0; (i < nFaces); i++)
            {
                for (int j = 0; (j < 3); j++)
                    vertices[faces[i].ids[j]].cUsed++;
            }
        }

        //Generate IDs for the vertices that are used
        {
            int vertexID = 0;
            for (int i = 0; (i < nVertices); i++)
            {
                if (vertices[i].cUsed > 2)
                {
                    vertexIDs[vertexID] = i;
                    vertices[i].id = vertexID++;
                }
                else
                    nSingularFaces += vertices[i].cUsed;
            }

            nUsedVertices = vertexID;
        }
    }

    if (fOut1)
    {
        fprintf(fOut1, "xof 0303txt 0032\nHeader { 1; 0; 1; }\nMesh\n{\n%d;\n", nUsedVertices); //}

        //Only print the vertices actually used by the convex hull
        {
            bool    firstF = true;
            for (int i = 0; (i < nVertices); i++)
            {
                if (vertices[i].cUsed > 2)
                {
                    if (firstF)
                        firstF = false;
                    else
                        fputs(",\n", fOut1);
                    fprintf(fOut1, "%14.6f;%14.6f;%14.6f;",
                            vertices[i].xyz[0],
                            vertices[i].xyz[1],
                            vertices[i].xyz[2]);
                }
            }
        }

        //Print the faces with the correct vertex IDs
        {
            fprintf(fOut1, ";\n\n%d;\n", nFaces - nSingularFaces);
            bool    first = true;
            for (int i = 0; (i < nFaces); i++)
            {
                if ((vertices[faces[i].ids[2]].cUsed > 2) &&
                    (vertices[faces[i].ids[1]].cUsed > 2) &&
                    (vertices[faces[0].ids[1]].cUsed > 2))
                {
                    if (first)
                        first = false;
                    else
                        fputs(",\n", fOut1);

                    fprintf(fOut1, "3;%d,%d,%d;",
                            vertices[faces[i].ids[2]].id,
                            vertices[faces[i].ids[1]].id,
                            vertices[faces[i].ids[0]].id);
                }
            }
            fputs(";\n", fOut1);
        }

        fprintf(fOut1, /*{*/"}\n");
    }

    if (fOut2)
    {
        double  radius = 0.0;
        double  xyzMax[3] = {0.0, 0.0, 0.0};
        {
            //Find the most distance vertex in the model
            for (int i = 0; (i < nUsedVertices); i++)
            {
                int vertexID = vertexIDs[i];
                double  r = vertices[vertexID].xyz[0] * vertices[vertexID].xyz[0] +
                            vertices[vertexID].xyz[1] * vertices[vertexID].xyz[1] +
                            vertices[vertexID].xyz[2] * vertices[vertexID].xyz[2];

                if (r > radius)
                    radius = r;

                for (int j = 0; (j < 3); j++)
                {
                    double  f = fabs(vertices[vertexID].xyz[j]);
                    if (f > xyzMax[j])
                        xyzMax[j] = f;
                }
            }
        }

        radius = sqrt(radius);

        //First attempt: find the best sphere
        double   aBest = radius;
        double   bBest = radius;
        double   cBest = radius;
        double   vBest = aBest * bBest * cBest;

        //Second attempt: find the best ellipse with the same aspect ratio as the bounding box
        {
            
            double  a = xyzMax[0];
            double  b = xyzMax[1];
            double  c = xyzMax[2];

            double  a2 = a * a;
            double  b2 = b * b;
            double  c2 = c * c;

            //Now find the fudge factor to make everything "fit"
            double  rMax = 0.0;
            for (int i = 0; (i < nUsedVertices); i++)
            {
                int vertexID = vertexIDs[i];
                double  r = (vertices[vertexID].xyz[0] * vertices[vertexID].xyz[0]) / a2 +
                            (vertices[vertexID].xyz[1] * vertices[vertexID].xyz[1]) / b2 +
                            (vertices[vertexID].xyz[2] * vertices[vertexID].xyz[2]) / c2;

                if (r > rMax)
                    rMax = r;
            }

            rMax = sqrt(rMax);
            a *= rMax;
            b *= rMax;
            c *= rMax;

            double v = a * b * c;
            if (v < vBest)
            {
                vBest = v;
                aBest = a;
                bBest = b;
                cBest = c;
            }
        }

        //Third attempt ... try to find a better ellipse from the data
        {
            //Generate the best ellipse (minimum volume) ellipse that contains all of the points.
            //Do it the real expensive way: look at all point triples and deduce the radii of the axes
            //and then pick the one with the smallest volume
            //Ellipse equation:
            //  (x/a)^2 + (y/b)^2 + (z/c)^2 = 1

            //We actually want to solve:
            //  Minimize: 1.0 / (ra * rb * rc) such that
            //  ra * x1^2 + rb * y1^2 + rc * z1^2 <= 1
            //              ...
            //  ra * xN^2 + rb * yN^2 + rc * zN^2 <= 1

            //but since it is too early in the morning to crank out a simplex algorithm, fake it:
            //Try all sets of 3 points, find the corresponding ellipse and see if it contains all the
            //other points.
            double*  xyzs[3];
            for (int    i = 0; (i < nUsedVertices - 2); i++)
            {
                xyzs[0] = vertices[vertexIDs[i]].xyz;

                for (int    j = i + 1; (j < nUsedVertices - 1); j++)
                {
                    xyzs[1] = vertices[vertexIDs[j]].xyz;

                    for (int    k = j + 1; (k < nUsedVertices); k++)
                    {
                        xyzs[2] = vertices[vertexIDs[k]].xyz;

                        //Rewrite the ellipse equation
                        //from: (x/a)^2 + (y/b)^2 + (z/c)^2 = 1
                        //to:   ra x^2 + rb y^2 + rc z^2 = 1        ra = 1.0/a^2; a = sqrt(1.0/ra);
                        //and solve for xyz1, 2 & 3.

                        //Setup the gaussian matrix (? ... boy, it has been awhile)
                        double  g[3][4];
                        {
                            for (int m = 0; (m < 3); m++)
                            {
                                for (int n = 0; (n < 3); n++)
                                    g[m][n] = xyzs[m][n] * xyzs[m][n];

                                g[m][3] = 1.0f;
                            }
                        }

                        //Reduce the gaussian
                        if (reduce(g) &&
                            (g[0][3] > 1.0e-4) &&
                            (g[1][3] > 1.0e-4) &&
                            (g[2][3] > 1.0e-4))

                        {
                            double   a = sqrt(1.0 / g[0][3]);
                            double   b = sqrt(1.0 / g[1][3]);
                            double   c = sqrt(1.0 / g[2][3]);

                            double  v = a * b * c;
                            if (v < vBest)
                            {
                                //Vertify that all of the other points in the convex hull are properly contained.
                                for (int m = 0; (m < nUsedVertices); m++)
                                {
                                    double x = vertices[vertexIDs[m]].xyz[0] / a;
                                    double y = vertices[vertexIDs[m]].xyz[1] / b;
                                    double z = vertices[vertexIDs[m]].xyz[2] / c;
                                    if (x*x + y*y + z*z > 1.01f)   //Allow a bit of fudging
                                        break;
                                }

                                if (m == nUsedVertices)
                                {
                                    vBest = v;

                                    aBest = a;
                                    bBest = b;
                                    cBest = c;
                                }
                            }
                        }
                    }
                }
            }
        }

        int nAdjacencies = 2 * (nUsedVertices + nFaces - nSingularFaces - 2);
        fprintf(fOut2, "%5d%5d%14.6f%14.6f%14.6f%14.6f%14.6f\n", nUsedVertices, nAdjacencies,
                radius,
                aBest, bBest, cBest, ((aBest > bBest)
                                      ? ((aBest > cBest) ? aBest : cBest)
                                      : ((bBest > cBest) ? bBest : cBest)) / radius);

        //Only print the vertices actually used by the convex hull
        {
            bool    firstF = true;
            for (int vertexID = 0; (vertexID < nVertices); vertexID++)
            {
                if (vertices[vertexID].cUsed > 2)
                {
                    fprintf(fOut2, "%14.6f%14.6f%14.6f", 
                            vertices[vertexID].xyz[0],
                            vertices[vertexID].xyz[1],
                            vertices[vertexID].xyz[2]);

                    //print the adjacent vertices
                    const int c_maxAdjacent = c_maxVertices;
                    int adjacent[c_maxAdjacent];

                    int nAdjacent = 0;
                    for (int faceID = 0; (faceID < nFaces); faceID++)
                    {
                        //Does this face contain this vertex?
                        if (vertexID == faces[faceID].ids[0])
                        {
                            addAdjacent(&nAdjacent, adjacent, faces[faceID].ids[1]);
                            addAdjacent(&nAdjacent, adjacent, faces[faceID].ids[2]);
                        }
                        else if (vertexID == faces[faceID].ids[1])
                        {
                            addAdjacent(&nAdjacent, adjacent, faces[faceID].ids[0]);
                            addAdjacent(&nAdjacent, adjacent, faces[faceID].ids[2]);
                        }
                        else if (vertexID == faces[faceID].ids[2])
                        {
                            addAdjacent(&nAdjacent, adjacent, faces[faceID].ids[0]);
                            addAdjacent(&nAdjacent, adjacent, faces[faceID].ids[1]);
                        }
                    }

                    fprintf(fOut2, "%5d\n", nAdjacent);
                    for (int a = 0; (a < nAdjacent); a++)
                        fprintf(fOut2, "%5d", vertices[adjacent[a]].id);
                    fputc('\n', fOut2);
                    
                    nAdjacencies -= nAdjacent;                  
                }
            }
        }
        if (nAdjacencies != 0)
        {
            printf("Error constructing %s.cvh\n", file);
            exit(1);
        }
    }

    if (fIn && (fIn != stdin))
        fclose(fIn);

    if (fOut1 && (fOut1 != stdout))
        fclose(fOut1);

    if (fOut2)
        fclose(fOut2);
}