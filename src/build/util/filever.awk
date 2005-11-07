BEGIN {
    strVariable         =   ""

    if (ARGC != 2)
    {
        printf("Usage: filever /B <file> | awk -f filever.awk <variable name>\n");
    } else
    {
        strVariable = ARGV[1];
        ARGC = 1;
    }
}

{
    strVersion = $5
}

END {
    if (length(strVariable) > 0)
        printf("set %s=%s\n", strVariable, strVersion);
}



