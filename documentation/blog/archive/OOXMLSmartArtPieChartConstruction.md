<div id="page">

# OOXML SmartArt Pie Chart Construction

[King Salemno](https://social.msdn.microsoft.com/profile/King%20Salemno)
12/3/2010 2:07:00
PM

-----

<div id="content">

# <span style="font-family: Cambria;"><span style="color: #365f91;">OOXML SmartArt Pie Chart Construction  
</span></span>

<span style="font-family: Calibri; font-size: small;"></span>

<span style="font-size: small;"><span style="font-family: Calibri;">In
recent years, vector graphics have become much more preferable due to
the various display systems encountered. Thus, images could be resized
without fear of quality loss. SmartArt works in a similar fashion. To
illustrate this, I am going to show the construction of a basic pie
chart in Word 2010 as an
example.</span></span>

<span style="mso-no-proof: yes;"></span>

*<span style="font-size: small;"><span style="font-family: Calibri;">![](images/0576.basicpie.png)The
graphic displayed above is an example of a basic pie chart with 3
slices. All 3 slices shown are constructed via XML.</span></span>*

<span style="font-size: small;"><span style="font-family: Calibri;">To
fully understand the construction of a smartart pie chart, an
understanding of geometry is necessary. What do we know about a pie? It
represents a geometric shape which happens to be a circle. What do we
know about circles? How do we map a point on a circle? We can find
definitive points on a circle given a unit of angular measurement. There
are 2 widely used such measurements: radians and degrees. However, for
our purposes, we are going to use the degree measurement as this is the
measurement used by the current
standard.</span></span>

<span style="mso-no-proof: yes;"></span><span style="font-size: small;"><span style="font-family: Calibri;">**<span style="mso-bidi-font-family: Calibri; mso-bidi-theme-font: minor-latin;">![](images/8551.piewithmeasurements.png)Note:
0° and 360° refer to the same
point.</span><span style="font-family: &#39;MS Shell Dlg 2&#39;,&#39;sans-serif&#39;;"></span>**</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Illustrated
above is our reference circle to explain the basics of construction.
Think of this as a race. You have a starting point and an ending point.
Given 2 points of reference, you can calculate the length between both
points. This is how the slices of the pie are constructed. We provide a
starting point and ending point by using the degrees unit of
measurement. The direction of marking along the circular path is
clockwise with 0/360 being our reference point. Therefore, to create a
slice that is half of the circle (the top half); we provide the
numerical values 180 and 360. 180 will be our starting point with 360
being the end of the angle. </span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">The
following 2 graphics illustrate this slice measurement. The graphic on
the left shows the area of interest shaded, while the second shows the
slice that would appear in Word
2010:</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;"></span></span>

<span style="mso-no-proof: yes;">![](images/5141.halfcircle.png)</span>

<span style="font-size: small;"><span style="font-family: Calibri;">These
values are contained in groups of two called ***shape adjust values***.
These values are sub-elements found underneath a serialized shape object
(XML Element: dgm:shape) and are identified by a shape adjust index
(idx), which uniquely identified the starting and ending points; with
the shape adjust index idx1 being the starting point and idx2 being the
ending
point.</span></span>

<span style="font-size: small;"><span style="font-family: Calibri;">Please
note that the information presented in this blog is not currently in the
\[MS-ODRAWXML\] Specification document but should be included in a
future release of the
documentation.</span></span>

**<span style="line-height: 115%; font-size: 12pt;"><span style="font-family: Calibri;">References</span></span>**

[<span style="font-family: Calibri; font-size: small;">\[MS-ODRAWXML\]:
Office Drawing Extensions to Office Open XML
Structure</span>](http://msdn.microsoft.com/en-us/library/dd905216.aspx)

*<span style="line-height: 140%; font-family: &#39;Segoe UI&#39;,&#39;sans-serif&#39;; color: black; font-size: 9.5pt;">Specifies
extensions to the DrawingML OOXML File Format Structure, which is used
in WordprocessingML, SpreadsheetML and PresentationML documents.</span>*

[OOXML SmartArt Pie Chart
Construction.pdf](images/OOXML%20SmartArt%20Pie%20Chart%20Construction.pdf)

</div>

</div>
