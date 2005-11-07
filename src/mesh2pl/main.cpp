#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

void    translate(const char*   fileIn, const char* fileOut);

int	main(int argc, char** argv)
{
    for (int i = 1; (i < argc); i += 2)
        translate(argv[i], argv[i + 1]);

    return 0;
}

void    translate(const char*   fileIn, const char* fileOut)
{
    FILE*   fIn = NULL;
    FILE*   fOut = NULL;

    if (strcmp(fileIn, "-") == 0)
    {
        fIn = stdin;
        fOut = stdout;
    }
    else
    {
        fIn = fopen(fileIn, "r");

        if (fIn)
        {
            fOut = fopen(fileOut, "w");
        }
    }

    if (fIn && fOut)
    {
        const int   c_cbLine = 512;
        char        line[c_cbLine];

        int nVertices = -2;
        while (fgets(line, c_cbLine, fIn) != NULL)
        {
            if (nVertices == -2)
            {
                if (strcmp(line, "Mesh {\n") == 0)
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
    }

    if (fIn && (fIn != stdin))
        fclose(fIn);

    if (fOut && (fOut != stdout))
        fclose(fOut);
}

