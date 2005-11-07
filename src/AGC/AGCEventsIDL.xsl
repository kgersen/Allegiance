<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">

<xsl:script src="AGCEventsXML.js">
<![CDATA[
var g_strMaxIndent  = "                                                                             ";
var g_strCommentBar = "/////////////////////////////////////////////////////////////////////////////";

function GetPrefix(node)
{
  var attr = node.attributes.getNamedItem("Prefix");
  if (attr)
    return attr.value;
  var theParent = node.parentNode;
  return theParent ? GetPrefix(theParent) : "__ERROR_NO_PREFIX__";
}

function GetLevel(node)
{
  // Count the levels deep that we are
  var nLevels = -1;
  var parentNode = node.parentNode;
  while (parentNode)
  {
    ++nLevels;
    parentNode = parentNode.parentNode;
  }
  return nLevels;
}

function GetIndent(node)
{
  var nLevels = GetLevel(node);
  return g_strMaxIndent.substr(0, nLevels * 2);
}

function GetIndentedCommentBar(node)
{
  var nLevels = GetLevel(node);
  var nSpaces = nLevels * 2;
  return g_strMaxIndent.substr(0, nLevels * 2) + g_strCommentBar.substr(nSpaces);
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
// It should be included from AGCIDL.idl.
//

<![CDATA[
/////////////////////////////////////////////////////////////////////////////
// AGCEventID
//
[
  uuid(014CE25C-1482-11d3-8B5F-00C04F681633),
  helpstring("IDs of the different Events that IAdminEvents handles.")
]
typedef [v1_enum] enum AGCEventID
{
  ///////////////////////////////////////////////////////////////////////////
  //
  EventID_Unknown = 0,
]]>
    <xsl:apply-templates/>
<![CDATA[
]]>
  ///////////////////////////////////////////////////////////////////////////
} AGCEventID;
  </xsl:template>

  <xsl:template match="EventGroup" xml:space="preserve">
<xsl:eval>GetIndentedCommentBar(this)</xsl:eval>
<xsl:eval>GetIndent(this)</xsl:eval>// <xsl:value-of select="@DisplayName"/> Events
<xsl:eval>GetIndent(this)</xsl:eval><xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name" />_LowerBound = <xsl:value-of select="@LowerBound" />,
<xsl:for-each select="Event"><xsl:eval>GetIndent(this);</xsl:eval><xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name"/> = <xsl:value-of select="@id"/>,
</xsl:for-each><xsl:apply-templates select="EventGroup"/>
<xsl:eval>GetIndent(this)</xsl:eval><xsl:eval>GetPrefix(this)</xsl:eval><xsl:value-of select="@Name" />_UpperBound = <xsl:value-of select="@UpperBound" />,
<xsl:eval>GetIndent(this)</xsl:eval>// End: <xsl:value-of select="@DisplayName"/> Events
<xsl:eval>GetIndentedCommentBar(this)</xsl:eval>
  </xsl:template>

</xsl:stylesheet>

