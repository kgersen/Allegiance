@echo off
if %1 == Retail (copy AGC.retail.manifest AGC.manifest /y) 
if %1 == FZRetail (copy AGC.retail.manifest AGC.manifest /y) else (copy AGC.debug.manifest AGC.manifest /y)
