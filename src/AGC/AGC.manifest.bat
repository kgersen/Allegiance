@echo off
if %1 == Retail (copy AGC.retail.manifest AGC.manifest /y) else (copy AGC.debug.manifest AGC.manifest /y)
