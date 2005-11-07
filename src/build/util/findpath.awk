BEGIN {
    strVariable         =   ""
    strPath             =   ""

    if (ARGC != 2)
    {
        printf("Usage: dir /s <file> | awk -f findpath.awk <variable name>\n");
    } else
    {
        strVariable = ARGV[1];
        ARGC = 1;
    }
}

/Directory/ {
    fSpace = 0;
    strTemp = $3;
    for(dwIndex = 4; dwIndex <= NF; dwIndex++)
    {
        strTemp = strTemp " " $(dwIndex)
        fSpace = 1;
    }

    if (1 == fSpace)
    {
        strPath = "\"" strTemp "\"";
    } else
        strPath = strTemp;
}

END {
    if ((length(strVariable) > 0) && (length(strPath) > 0))
        printf("set %s=%s\n", strVariable, strPath);
}



