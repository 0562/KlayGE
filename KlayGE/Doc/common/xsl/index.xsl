<?xml version="1.0" encoding="gb2312" ?>

<xsl:stylesheet xmlns:xsl="http://www.w3.org/TR/WD-xsl">
	<xsl:template match="/">
		<html>
		<head>
			<title><xsl:value-of select="˵��/����"/></title>
			<meta http-equiv="Content-Type" content="text/html; charset=gb2312"/>
		</head>
		<body style="line-height: 150%">
			<xsl:apply-templates select="˵��/����"/>
			<h2>������</h2>
			<xsl:apply-templates select="˵��/����"/>
			<hr/>
			<h2>��Ա��</h2>
			<table border="1" width="100%">
				<xsl:for-each select="˵��/����">
					<tr>
						<td style="line-height: 150%">
							<a><xsl:attribute name="href"><xsl:value-of select="ê��"/></xsl:attribute><xsl:value-of select="����"/></a>
						</td>
						<td style="line-height: 150%">
							<xsl:value-of select="����"/>
						</td>
					</tr>
				</xsl:for-each>
			</table>
			<hr/>
			<h2>������</h2>
			<xsl:apply-templates select="˵��/����"/>
			<hr/>
			<div style="text-align: center">
				<a href="http://www.enginedev.com" target="_blank">ճ����Ϸ����</a><br/>
				��Ȩ����(C) �������̴�ѧ �����ѧԺ �����003�� ��������2003
			</div>
		</body>
		</html>
	</xsl:template>

	<xsl:template match="����">
		<h1 style="text-align: center"><xsl:value-of/></h1>
	</xsl:template>

	<xsl:template match="����">
		<p><xsl:value-of/></p>
	</xsl:template>

	<xsl:template match="����">
		<hr/>
		<a><xsl:attribute name="name"><xsl:value-of select="����"/></xsl:attribute>ԭ�ͣ�</a>
		<xsl:apply-templates select="ԭ��"/>
		<xsl:apply-templates select="����"/>
		<xsl:apply-templates select="����"/>
		<xsl:apply-templates select="��������"/>
		<xsl:apply-templates select="ע��"/>
	</xsl:template>

	<xsl:template match="ԭ��">
		<blockquote>
			<pre style="background-color: #C0C0C0; color: #000000; font-weight: bold; padding: 5">
				<xsl:value-of/>
			</pre>
		</blockquote>
	</xsl:template>

	<xsl:template match="����">
		<xsl:value-of/>
	</xsl:template>

	<xsl:template match="����">
		<h4>���أ�</h4>
		<xsl:value-of/>
	</xsl:template>

	<xsl:template match="��������">
		<dl>
			<h4>������</h4>
			<xsl:apply-templates select="����"/>
		</dl>
	</xsl:template>

	<xsl:template match="����">
		<dt style="font-style: italic">[<xsl:value-of select="����"/>] <xsl:value-of select="����"/></dt>
		<dd><xsl:value-of select="����"/></dd>
	</xsl:template>

	<xsl:template match="ע��">
		<h4>ע�⣺</h4>
		<xsl:value-of/>
	</xsl:template>
</xsl:stylesheet>