BEGIN {
    strVariable         =   ""

    if (ARGC != 2)
        printf("Usage: wc <file> | awk -f todaysdate.awk <variable name>\n");
    else
    {
        strVariable = ARGV[1];
        ARGC = 1;
    }
}

{
    strDate = $0
}

END {
    if (length(strVariable) > 0)
        printf("set %s=%s\n", strVariable, strDate);
}



