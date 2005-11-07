BEGIN {
    strDrive            =   ""
    strVariable         =   ""

    if (ARGC != 2)
        printf("Usage: echo <pathstring> | awk -f getdrive.awk <variable name>\n");
    else
    {
        strVariable = ARGV[1];
        ARGC = 1;
    }
}

{
    strDrive = substr($1, 1, 2)
}

END {
    if (length(strVariable) > 0)
        printf("set %s=%s\n", strVariable, strDrive);
}



