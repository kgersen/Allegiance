

SUB LogInit(strFileName as STRING)
    OPEN strFileName FOR OUTPUT AS 5
END SUB


SUB LogTerm()
    CLOSE 5
END SUB


SUB LogMessage(strMessage as STRING)
    PRINT #5, strMessage
END SUB

