#include    "pch.h"

const int   c_maxVertices = 10000;
const int   c_maxFaces = 1000;
const int   c_cbBfr = 512;

const int   c_ellipse = 0x01;
const int   c_oneHull = 0x02;
const int   c_writeHull = 0x04;

const int   c_writeMesh = 0x08;
const int   c_startMesh = 0x10;
const int   c_endMesh = 0x20;

bool    x2pl(const char*     fileIn, const char* fileOut);
bool    qh2cvh(const char*   file, FILE*    fOut, int  mode);
bool    dat2cvh(const char*  file, FILE*    fOut);

int	main(int argc, char** argv)
{
    bool    success = false;

    if (argc == 3)
    {
        char    bfr[256];
        strcpy(bfr, argv[2]);
        strcat(bfr, ".cvh");

        FILE*   fOut = fopen(bfr, "w");
        if (fOut)
        {
            enum Modes
            {   
                c_flatten,
                c_extract,
                c_bound
            }   mode;

            if (strcmp(argv[1], "-flatten") == 0)
                mode = c_flatten;
            else if (strcmp(argv[1], "-extract") == 0)
                mode = c_extract;
            else if (strcmp(argv[1], "-bound") == 0)
                mode = c_bound;

            char    bfrIn[c_cbBfr];
            strcpy(bfrIn, argv[2]);
            strcat(bfrIn, "flat.x");

            char    bfrOut[c_cbBfr];
            strcpy(bfrOut, argv[2]);
            strcat(bfrOut, ".pl");

            success = x2pl(bfrIn, bfrOut);
            if (success)
            {
                char    bfrSystem[c_cbBfr];
                strcpy(bfrSystem, "%fedroot%\\src\\bin\\qhull -o -Tv <");
                strcat(bfrSystem, argv[2]);
                strcat(bfrSystem, ".pl >");
                strcat(bfrSystem, argv[2]);
                strcat(bfrSystem, ".qh");
                system(bfrSystem);

                success = qh2cvh(argv[2], fOut, (mode == c_bound)
                                 ? (c_ellipse | c_startMesh)
                                 : (c_ellipse | c_oneHull | c_writeHull | c_writeMesh | c_startMesh | c_endMesh));
            }

            if (mode == c_bound)
            {
                int hullID = -1;
                do
                {
                    char    bfrHull[c_cbBfr];
                    sprintf(bfrHull, "%s_%d", argv[2], ++hullID);

                    char    bfrIn[c_cbBfr];
                    strcpy(bfrIn, bfrHull);
                    strcat(bfrIn, ".mesh");

                    char    bfrOut[c_cbBfr];
                    strcpy(bfrOut, bfrHull);
                    strcat(bfrOut, ".pl");

                    success = x2pl(bfrIn, bfrOut);
                }
                while (success);
                success = true;
                fprintf(fOut, "%5d\n", hullID);

                for (int i = 0; ((i < hullID) && success); i++)
                {
                    char    bfrHull[c_cbBfr];
                    sprintf(bfrHull, "%s_%d", argv[2], i);

                    char    bfrSystem[c_cbBfr];
                    strcpy(bfrSystem, "%fedroot%\\src\\bin\\qhull -o -Tv <");
                    strcat(bfrSystem, bfrHull);
                    strcat(bfrSystem, ".pl >");
                    strcat(bfrSystem, bfrHull);
                    strcat(bfrSystem, ".qh");
                    system(bfrSystem);

                    success = qh2cvh(bfrHull, fOut, (i == hullID - 1)
                                                    ? (c_writeHull | c_writeMesh | c_endMesh)
                                                    : (c_writeHull | c_writeMesh));
                }
            }

            if (mode != c_flatten)
                success = dat2cvh(argv[2], fOut);

            fclose(fOut);
        }
    }

    return success ? 0 : -1;
}

bool    x2pl(const char*   fileIn, const char* fileOut)
{
    bool    success = false;

    FILE*   fIn = NULL;
    FILE*   fOut = NULL;

    fIn = fopen(fileIn, "r");

    if (fIn)
    {
        fOut = fopen(fileOut, "w");

        if (fOut)
        {
            success = true;

            const int   c_cbLine = 512;
            char        line[c_cbLine];

            int nVertices = -2;
            while (fgets(line, c_cbLine, fIn) != NULL)
            {
                if (nVertices == -2)
                {
                    if (strcmp(line, "Mesh {\n"/*}*/) == 0)
                        nVertices = -1;
                }
                else if (nVertices == -1)
                {
                    nVertices = atoi(line);
                    fprintf(fOut, "3 %d %s\n", nVertices, fileIn);
                }
                else if (nVertices == 0)
                    break;
                else
                {
                    //Write out the vertices, translating ';' to ' '
                    int         nsc = 0;
                    const char* p = line;
                    while (true)
                    {
                        assert (*p != '\0');

                        if (*p == ';')
                        {
                            if (++nsc == 3)
                                break;
                            fputc('\t', fOut);
                        }
                        else
                            fputc(*p, fOut);
                        p++;
                    }
                    fputc('\n', fOut);

                    nVertices--;
                }
            }
            fclose(fOut);
        }
        fclose(fIn);
    }

    return success;
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

bool    qh2cvh(const char*   file, FILE*    fOut, int mode)
{
    bool    success = false;
    assert (fOut);

    FILE*   fIn;

    {
        char    bfr[c_cbBfr];
        assert (strlen(file) < c_cbBfr - 10);
        strcpy(bfr, file);
        strcat(bfr, ".qh");

        fIn = fopen(bfr, "r");
    }

    if (fIn)
    {
        success = true;

        int         nVertices = 0;
        int         nUsedVertices = 0;
        int         nSingularFaces = 0;
        int         nFaces = 0;

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

                vertices[i].cUsed = 0;
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


        double  radius = 0.0;
        double  xyzMin[3] = {0.0, 0.0, 0.0};
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
                    if (vertices[vertexID].xyz[j] > xyzMax[j])
                        xyzMax[j] = vertices[vertexID].xyz[j];
                    if (vertices[vertexID].xyz[j] < xyzMin[j])
                        xyzMin[j] = vertices[vertexID].xyz[j];
                }
            }
        }

        radius = sqrt(radius);

        static FILE*   fOutMesh;

        if (mode & c_startMesh)
        {
            char    bfr[c_cbBfr];
            assert (strlen(file) < c_cbBfr - 10);

            strcpy(bfr, "..\\artbuild\\");
            strcat(bfr, file);
            strcat(bfr, "_m.x");

            fOutMesh = fopen(bfr, "w");

            fprintf(fOutMesh, "xof 0302txt 0064\nHeader { 1; 0; 1; }\n");
        }

        if (mode & c_writeMesh)
        {

            fprintf(fOutMesh, "Mesh\n{\n%d;\n", nUsedVertices); //}

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
                            fputs(",\n", fOutMesh);
                        fprintf(fOutMesh, "%14.6f;%14.6f;%14.6f;",
                                -vertices[i].xyz[0],
                                -vertices[i].xyz[1],
                                vertices[i].xyz[2]);
                    }
                }
            }

            //Print the faces with the correct vertex IDs
            {
                fprintf(fOutMesh, ";\n\n%d;\n", nFaces - nSingularFaces);
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
                            fputs(",\n", fOutMesh);

                        fprintf(fOutMesh, "3;%d,%d,%d;",
                                vertices[faces[i].ids[2]].id,
                                vertices[faces[i].ids[1]].id,
                                vertices[faces[i].ids[0]].id);
                    }
                }
                fputs(";\n", fOutMesh);
            }

            fprintf(fOutMesh, /*{*/"}\n");

            if (mode & c_endMesh)
                fclose(fOutMesh);
        }

        if (mode & c_ellipse)
        {
            //See if the bounding box is appropriately centered
            {
                int i;
                for (i = 0; (i < 3); i++)
                {
                    if (fabs(xyzMin[i] + xyzMax[i]) > 0.05 * (xyzMax[i] - xyzMin[i]))
                        break;
                }

                if (i != 3)
                {
                    printf("***Non-centered cvh: %s\n", file);
                    printf("\t%f %f\n", fabs(xyzMin[0] + xyzMax[0]), (xyzMax[0] - xyzMin[0]));
                    printf("\t%f %f\n", fabs(xyzMin[1] + xyzMax[1]), (xyzMax[1] - xyzMin[1]));
                    printf("\t%f %f\n", fabs(xyzMin[2] + xyzMax[2]), (xyzMax[2] - xyzMin[2]));
                }
            }

            //First attempt: find the best sphere
            double   aBest = radius;
            double   bBest = radius;
            double   cBest = radius;
            double   vBest = aBest * bBest * cBest;

            //Second attempt: find the best ellipse with the same aspect ratio as the bounding box
            {
        
                double  a = fabs(xyzMin[0]) > fabs(xyzMax[0]) ? fabs(xyzMin[0]) : fabs(xyzMax[0]);
                double  b = fabs(xyzMin[1]) > fabs(xyzMax[1]) ? fabs(xyzMin[1]) : fabs(xyzMax[1]);
                double  c = fabs(xyzMin[2]) > fabs(xyzMax[2]) ? fabs(xyzMin[2]) : fabs(xyzMax[2]);

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
                                    int m;
									for (m = 0; (m < nUsedVertices); m++)
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

            fprintf(fOut, "%16.8f%16.8f%16.8f%16.8f%16.8f\n",
                    radius,
                    aBest, bBest, cBest, ((aBest > bBest)
                                          ? ((aBest > cBest) ? aBest : cBest)
                                          : ((bBest > cBest) ? bBest : cBest)) / radius);
        }

        if (mode & c_oneHull)
            fprintf(fOut, "1\n");

        //Only print the vertices actually used by the convex hull
        if (mode & c_writeHull)
        {
            int nAdjacencies = 2 * (nUsedVertices + nFaces - nSingularFaces - 2);
            fprintf(fOut, "%5d%5d%18.8f%18.8f%18.8f\n",
                    nUsedVertices, nAdjacencies,
                    0.5 * (xyzMin[0] + xyzMax[0]),
                    0.5 * (xyzMin[1] + xyzMax[1]),
                    0.5 * (xyzMin[2] + xyzMax[2]));

            bool    firstF = true;
            for (int vertexID = 0; (vertexID < nVertices); vertexID++)
            {
                if (vertices[vertexID].cUsed > 2)
                {
                    fprintf(fOut, "%18.8f%18.8f%18.8f", 
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

                    fprintf(fOut, "%5d\n", nAdjacent);
                    for (int a = 0; (a < nAdjacent); a++)
                        fprintf(fOut, "%5d", vertices[adjacent[a]].id);
                    fputc('\n', fOut);
                
                    nAdjacencies -= nAdjacent;                  
                }
            }
            if (nAdjacencies != 0)
            {
                printf("Error constructing %s.cvh\n", file);
                success = false;
            }
        }

        fclose(fIn);
    }

    return success;
}

bool    dat2cvh(const char*  file, FILE*    fOut)
{
    bool    success = false;

    char    bfr[c_cbBfr];
    strcpy(bfr, file);
    strcat(bfr, ".dat");

    FILE*   fIn = fopen(bfr, "r");
    if (fIn)
    {
        success = true;

        char    line[c_cbBfr];
        while (fgets(line, c_cbBfr, fIn) != NULL)
        {
            fputs(line, fOut);
        }

        fclose(fIn);
    }
    return success;
}
