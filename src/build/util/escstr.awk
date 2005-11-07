BEGIN {
    strVariable         =   ""

    if (ARGC != 2)
        printf("Usage: <cmd> | awk -f escstr.awk <variable name>\n");
    else
    {
        strVariable = ARGV[1];
        ARGC = 1;
    }
}

{
    strArg = $1
    strString = ""
    iIndex = index(strArg, "\\")
    while (iIndex > 0) {
        strTemp = sprintf("%s%s\\\\", strString, substr(strArg, 1, iIndex - 1))
        strString = strTemp
        strArg = substr(strArg, iIndex + 1)
        iIndex = index(strArg, "\\")
    }

    strTemp = sprintf("%s%s", strString, strArg)
    strString = strTemp
}

END {
    if (length(strVariable) > 0)
        printf("set %s=%s\n", strVariable, strString);
}



