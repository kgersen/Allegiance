BEGIN {
}

/Computer/ {
    strName = substr($3, 3, length($3) - 2)
    printf("set COMPUTERNAME=%s\n", strName);
}

END {
}

