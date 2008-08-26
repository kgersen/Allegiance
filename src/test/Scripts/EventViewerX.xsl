<?xml version='1.0'?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">

<xsl:template match="/">
  <xsl:apply-templates/>
</xsl:template>

<xsl:template match="root">
  <html>
  <head>
    <title>AGC Event Viewer (via XML)</title>
    <style>
      pre
      {
        font-family: Verdana;
        font-size: 9pt;
      }
      td
      {
        vertical-align: baseline;
      }
      td.SortHeader
      {
        cursor: hand;
      }
    </style>
  </head>
  <body bgcolor="#FFFFFF">
  <font face="Verdana" size="2">

    <table width="100%" CELLPADDING="0" CELLSPACING="0" BORDER="1" bgcolor="#E0E0E0" bordercolor="#FFFFFF" STYLE="font-family:Verdana;font-size:x-small">
      <thead>
      <tr bgcolor="#CCCCCC" VALIGN="baseline">
        <td width="22%;" style="cursor:hand;" onclick="OnSortByDateTime();">Date/Time</td>
        <td width="4%" align="center" class="SortHeader" onclick="OnSortByEvent();">ID</td>
        <td width="8%" ALIGN="center" class="SortHeader" onclick="OnSortBySubjectName();">Subject</td>
        <td width="66%" class="SortHeader" onclick="OnSortByDescription();">Description</td>
      </tr>
      </thead>
  
      <tbody id="OutputTable">
        <xsl:apply-templates select="Events"/>
      </tbody>
    </table>

  </font>
  </body>
  </html>
</xsl:template>


<xsl:template match="Events">
  <tr>
    <td><xsl:value-of select="@DateTime"/></td>
    <td><xsl:value-of select="@Event"/></td>
    <td><xsl:value-of select="@SubjectName"/></td>
    <td><xsl:value-of select="@description"/></td>
  </tr>
</xsl:template>


</xsl:stylesheet>
