cscript /e:jscript /NoLogo ..\src\AGC\XMLXForm.js ..\src\AGC\AGCEvents.xml ..\src\AGC\AGCEventsCPP.xsl %1AGCEventsCPP.h
cscript /e:jscript /NoLogo ..\src\AGC\XMLXForm.js ..\src\AGC\AGCEvents.xml ..\src\AGC\AGCEventsRC2.xsl %1AGCEventsRC2.rc2
cscript /e:jscript /NoLogo ..\src\AGC\XMLXForm.js ..\src\AGC\AGCEvents.xml ..\src\AGC\AGCEventsRCH.xsl %1AGCEventsRCH.h
cscript /e:jscript /NoLogo ..\src\AGC\XMLXForm.js ..\src\AGC\AGCEvents.xml ..\src\AGC\AGCEventsMC.xsl %1AGCEventsMC.mc
mc.exe -r %1 -h %1 %1AGCEventsMC.mc
