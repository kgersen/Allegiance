BEGIN {
    strBuild            =   "0"
    strVariable         =   ""

    if (ARGC != 2)
        printf("Usage: <cmd> | awk -f maxbld.awk <variable name>\n");
    else
    {
        strVariable = ARGV[1];
        ARGC = 1;
    }
}

{
    iBuild = $1
    strBuild2 = sprintf("%d", iBuild);
    if (strBuild2 > strBuild)
        strBuild = $1
}

END {
    if (length(strVariable) > 0)
        printf("set %s=%s\n", strVariable, strBuild);
}



