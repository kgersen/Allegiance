<?xml version='1.0'?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">


  <xsl:template match="/">
  <xsl:apply-templates/>
</xsl:template>


<xsl:template match="AGCEvents">
  <html>
  <head>
    <title>AGC Event Definition Browser</title>
    <script language="jscript">
    <![CDATA[
      function ExpandContract()
      {
        var tags = event.srcElement.parentElement.children.tags("div");
        for (var it = new Enumerator(tags); !it.atEnd(); it.moveNext())
        {
          var tag = it.item();
          var bCollapsed = "none" == tag.style.display;
          tag.style.display = bCollapsed ? "" : "none";
          var str = event.srcElement.innerText;
          if ('+' == str.charAt(0) || '-' == str.charAt(0))
            event.srcElement.innerText = (bCollapsed ? '-' : '+') + str.substring(1);
        }
      }
    ]]>
    </script>
  </head>
  <body bgcolor="#FFFFFF">
  <font face="Verdana" size="2">

  <xsl:apply-templates/>

  </font>
  </body>
  </html>
</xsl:template>


<xsl:template match="EventGroup">
  <div style="margin-left:2em;">
  <span style="cursor:hand;" onclick="ExpandContract();"
    onmouseover="this.style.color='red';" onmouseout="this.style.color='black';">
    - <xsl:value-of select="@DisplayName"/> events
  </span>
    <xsl:apply-templates select="EventGroup"/>
    <xsl:for-each select="Event">
      <div style="margin-left:3em;">
        <span style="cursor:text;">
          <xsl:value-of select="@Name"/> - <xsl:value-of select="@Description"/>
        </span>
      </div>
    </xsl:for-each>
  </div>
</xsl:template>


</xsl:stylesheet>