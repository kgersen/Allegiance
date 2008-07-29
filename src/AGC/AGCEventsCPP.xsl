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
// It should be included from AGCEventDef.cpp.
//
<![CDATA[

/////////////////////////////////////////////////////////////////////////////
// Macros

#define EVENTDEF_ENTRY_EX(id, nIndent, pszName, pszEvtDesc, pszFmtDesc,     \
  wSeverity, bLogAsNT, bLogAsDB)                                            \
  {                                                                         \
    id,                                                                     \
    nIndent,                                                                \
    pszName,                                                                \
    pszEvtDesc,                                                             \
    pszFmtDesc,                                                             \
    wSeverity,                                                              \
    bLogAsNT,                                                               \
    bLogAsDB,                                                               \
  },

#define EVENTDEF_ENTRY(prefix, name, wSeverity, bLogAsNT, bLogAsDB)         \
  EVENTDEF_ENTRY_EX(prefix##name, 0, OLESTR(#name),                         \
    MAKEINTRESOURCEW(IDS_##prefix##name),                                   \
    MAKEINTRESOURCEW(IDS_FMT_##prefix##name), wSeverity, bLogAsNT, bLogAsDB)

#define EVENTDEF_BEGIN_GROUP(prefix, name)                                  \
  EVENTDEF_ENTRY_EX(prefix##name, +1,                                       \
    MAKEINTRESOURCEW(IDS_NAME_##prefix##name),                              \
    MAKEINTRESOURCEW(IDS_##prefix##name), NULL, 0, false, false)

#define EVENTDEF_END_GROUP(prefix, name)                                    \
  EVENTDEF_ENTRY_EX(prefix##name, -1, OLESTR(#name), NULL, NULL, 0, false, false)


/////////////////////////////////////////////////////////////////////////////
// Description: Table of AGCEvent definitions.
//
// Note: The entries of this table *must* be listed in ascending order of the
// AGCEventID. This allows a quick binary search when resolving an AGCEventID
// to its event definition.
//
const CAGCEventDef::XEventDef CAGCEventDef::s_EventDefs[] =
{
  EVENTDEF_ENTRY(EventID_, Unknown, 0, 0, 0)
]]><xsl:apply-templates/><![CDATA[
};

/////////////////////////////////////////////////////////////////////////////
// Compute/declare the number of entries in the table.
//
const CAGCEventDef::XEventDef* CAGCEventDef::s_EventDefs_end =
  CAGCEventDef::s_EventDefs + sizeofArray(CAGCEventDef::s_EventDefs);

]]>
  </xsl:template>

  <xsl:template match="EventGroup" xml:space="preserve">
<xsl:eval>GetIndentedCommentBar(this)</xsl:eval>
<xsl:eval>GetIndent(this)</xsl:eval>// <xsl:value-of select="@DisplayName"/> Events
<xsl:eval>GetIndent(this)</xsl:eval>EVENTDEF_BEGIN_GROUP(<xsl:eval>GetPrefix(this)</xsl:eval>, <xsl:value-of select="@Name"/>_LowerBound)
<xsl:for-each select="Event"><xsl:eval>GetIndent(this)</xsl:eval>EVENTDEF_ENTRY(<xsl:eval>GetPrefix(this)</xsl:eval>, <xsl:value-of select="@Name"/>, <xsl:choose><xsl:when test="@Severity[.='Info']">EVENTLOG_INFORMATION_TYPE</xsl:when><xsl:when test="@Severity[.='Warning']">EVENTLOG_WARNING_TYPE</xsl:when><xsl:when test="@Severity[.='Error']">EVENTLOG_ERROR_TYPE</xsl:when><xsl:otherwise>EVENTLOG_SUCCESS</xsl:otherwise></xsl:choose>, <xsl:value-of select="@LogAsNTEvent"/>, <xsl:value-of select="@LogAsDBEvent"/>)
</xsl:for-each><xsl:apply-templates select="EventGroup"/>
<xsl:eval>GetIndent(this)</xsl:eval>EVENTDEF_END_GROUP(<xsl:eval>GetPrefix(this)</xsl:eval>, <xsl:value-of select="@Name"/>_UpperBound)
<xsl:eval>GetIndent(this)</xsl:eval>// End: <xsl:value-of select="@DisplayName" /> Events
<xsl:eval>GetIndentedCommentBar(this)</xsl:eval>
  </xsl:template>

</xsl:stylesheet>




