<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">

<xsl:script src="AGCEventsXML.js">
<![CDATA[
var g_idResDescriptionBase = 0;
var g_idResFormatBase      = 0;

function GetPrefix(node)
{
  var attr = node.attributes.getNamedItem("Prefix");
  if (attr)
    return attr.value;
  var theParent = node.parentNode;
  return theParent ? GetPrefix(theParent) : "__ERROR_NO_PREFIX__";
}

function SaveDescriptionBase(node)
{
  g_idResDescriptionBase = new Number(node.attributes.getNamedItem("ResIDDescription").value);
  return g_idResDescriptionBase.toString();
}

function SaveFormatBase(node)
{
  g_idResFormatBase = new Number(node.attributes.getNamedItem("ResIDFormat").value);
  return g_idResFormatBase.toString();
}

function GetDescriptionID(node, strAttr)
{
  var id = new Number(node.attributes.getNamedItem(strAttr).value);
  return (g_idResDescriptionBase + id).toString();
}

function GetFormatID(node)
{
  var id = new Number(node.attributes.getNamedItem("id").value);
  return (g_idResFormatBase + id).toString();
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
// It should be included from resource.h.
//
#define IDS_EventID_Unknown     <xsl:eval>SaveDescriptionBase(this)</xsl:eval>
#define IDS_FMT_EventID_Unknown <xsl:eval>SaveFormatBase(this)</xsl:eval>

    <xsl:apply-templates/>
  </xsl:template>

  <xsl:template match="EventGroup" xml:space="preserve">
#define IDS_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/>_LowerBound <xsl:eval>GetDescriptionID(this, "LowerBound")</xsl:eval>
<xsl:for-each select="Event">
#define IDS_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/> <xsl:eval>GetDescriptionID(this, "id")</xsl:eval>
#define IDS_FMT_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/> <xsl:eval>GetFormatID(this)</xsl:eval>
</xsl:for-each>
    <xsl:apply-templates select="EventGroup"/>
#define IDS_NAME_<xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/>_LowerBound <xsl:eval>GetDescriptionID(this, "UpperBound")</xsl:eval>
  </xsl:template>

</xsl:stylesheet>

