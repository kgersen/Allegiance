<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">

<xsl:script>
<![CDATA[

function GetPrefix(node)
{
  var attr = node.attributes.getNamedItem("Prefix");
  if (attr)
    return attr.value;
  var theParent = node.parentNode;
  return theParent ? GetPrefix(theParent) : "__ERROR_NO_PREFIX__";
}

function GetMCFormat(node)
{
  var attr = node.attributes.getNamedItem("Format");
  if (!attr)
    return "";
  var strFormat = attr.value;

  // Get the array of symbol names used in the format string
  var arrNames = GetFormatSymbols(strFormat);

  // Perform symbol replacements
  for (var i = 0; i < arrNames.length; ++i)
  {
    var j = i + 1;
    strFormat = strFormat.replace(arrNames[i], "%" + j);
  }

  // Perform special character replacements
  var reLF = /\\n/g;
  strFormat = strFormat.replace(reLF    , "%n");
  var reCR = /\\r/g;
  strFormat = strFormat.replace(reCR    , "%r");
  var reSP = / $/;
  strFormat = strFormat.replace(reSP    , "% ");
  var rePeriod = /^\./;
  strFormat = strFormat.replace(rePeriod, "%.");
  var reExclam = /!/g;
  strFormat = strFormat.replace(reExclam, "%!");
  var reTab = /\\t/g;
  strFormat = strFormat.replace(reTab   , "    ");

  // Return the modified string
  return strFormat;
}

function GetFormatSymbols(strFormat)
{
  // Find each pair of '%' characters in the format string
  var re = /%([^%]*)%/g;
  var strTemp = strFormat;
  var arrNames = new Array();
  while (re.exec(strTemp))
  {
    strTemp = strTemp.substr(RegExp.lastIndex);
    var strSymbol = RegExp.$1;
    if (strSymbol.length)
      arrNames[arrNames.length] = strSymbol;
  }

  // Convert each symbol string to a regular expression
  var strSpecialChars = "\\~$*+?.()|{}[]-";
  for (var i = 0; i < arrNames.length; ++i)
  {
    var strOld = arrNames[i];
    var strNew = "";
    for (var j = 0; j < strOld.length; ++j)
    {
      var ch = strOld.charAt(j);
      if (strSpecialChars.indexOf(ch) >= 0)
        strNew += "\\";
      strNew += ch;
    }
    arrNames[i] = new RegExp("%" + strNew + "%", "gi");
  }

  // Return the array of regular expressions
  return arrNames;
}

]]>
</xsl:script>
  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match="AGCEvents" xsl:space="preserve">
;/////////////////////////////////////////////////////////////////////////////
;// <xsl:value-of select="@outputfile"/>
;//
;// This is an ALWAYS GENERATED file. It should *not* be edited directly.
;//
;// It was generated at <xsl:value-of select="@time"/> from the following script:
;//   <xsl:value-of select="@scriptfullname"/>
;//
;// The input files were:
;//   <xsl:value-of select="@xmlfile"/>
;//   <xsl:value-of select="@xslfile"/>
;//
;// It should be (or was) compiled by the Win32 MC.exe message compiler.
;//
LanguageNames=(English=0x409:AGC0409)
SeverityNames=(
    Success=0x0
    Info=0x1
    Warning=0x2
    Error=0x3
    ) 
    <xsl:apply-templates/>
  </xsl:template>
  <xsl:template match="EventGroup" xml:space="preserve">
<xsl:for-each select="Event">
MessageId=<xsl:value-of select="@id"/>
Severity=<xsl:value-of select="@Severity"/>
SymbolicName=MSG_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/>
Language=English
<xsl:eval>GetMCFormat(this)</xsl:eval>
.
</xsl:for-each>
    <xsl:apply-templates select="EventGroup"/>
  </xsl:template>
</xsl:stylesheet>

