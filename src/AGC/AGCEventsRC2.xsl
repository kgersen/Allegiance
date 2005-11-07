<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">

<xsl:script src="AGCEventsXML.js">
<![CDATA[

function GetPrefix(node)
{
  var attr = node.attributes.getNamedItem("Prefix");
  if (attr)
    return attr.value;
  var theParent = node.parentNode;
  return theParent ? GetPrefix(theParent) : "__ERROR_NO_PREFIX__";
}

]]>
</xsl:script>

  <xsl:template match="/">
    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="AGCEvents" xsl:space="preserve">
/////////////////////////////////////////////////////////////////////////////
// <xsl:value-of select="@outputfile"/>
//
// This is an ALWAYS GENERATED file. It should *not* be edited directly.
//
// It was generated at <xsl:value-of select="@time"/> from the following script:
//   <xsl:value-of select="@scriptfullname"/>
//
// The input files were:
//   <xsl:value-of select="@xmlfile"/>
//   <xsl:value-of select="@xslfile"/>
//
// It should be included from AGC.rc2, after including resource.h.
//
STRINGTABLE DISCARDABLE 
BEGIN
    <xsl:apply-templates/>
END
  </xsl:template>

  <xsl:template match="EventGroup" xml:space="preserve">
  IDS_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/>_LowerBound "<xsl:value-of select="@Description"/>"
<xsl:for-each select="Event">  IDS_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/> "<xsl:value-of select="@Description"/>"
  IDS_FMT_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/> "<xsl:value-of select="@Format"/>"
</xsl:for-each>
    <xsl:apply-templates select="EventGroup"/>
  IDS_NAME_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/>_LowerBound "<xsl:value-of select="@DisplayName"/>"
  </xsl:template>
</xsl:stylesheet>

