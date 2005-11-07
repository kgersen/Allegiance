BEGIN {
    # Variable initialization.
    iError = 0
}

{
    if (NR % 6 == 1)
        strQuery = $0
    else if (NR % 6 == 4)
    {
        if ($1 == "0")
        {
            printf("%s returns 0 records.\n", strQuery);
            iError = 1
        }
    }
}

END {
}

