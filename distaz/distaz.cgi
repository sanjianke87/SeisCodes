#!/usr/bin/env python

import cgi
import cgitb; cgitb.enable()  # for troubleshooting
import distaz

print "Content-type: text/html"
print
 
print """
<html>

<head><title>Dist Az</title></head>

<body>

  <h3> Simple Distance, Azimuth, Back Azimuth Calculator</h3>
"""

form = cgi.FieldStorage()
evlat = float(form.getvalue("evlat", "0"))
evlon = float(form.getvalue("evlon", "0"))
stlat = float(form.getvalue("stlat", "0"))
stlon = float(form.getvalue("stlon", "0"))

if evlat > 90 or evlat < -90:
   print """
  <b><p>Event Lat must be -90  lat  90. </p></b>
"""
elif evlon > 180 or evlon < -180:
   print """
  <b><p>Event Lon must be -180  lon  180. </p></b>
"""
elif stlat > 90 or stlat < -90:
   print """
  <b><p>Station Lat must be -90  lat  90. </p></b>
"""
elif stlon > 180 or stlon < -180:
   print """
  <b><p>Station Lon must be -180  lon  180. </p></b>
"""
else:

   distaz = distaz.DistAz(evlat, evlon, stlat, stlon)
   print """
  <p>From (%3.2f, %3.2f) to station (%3.2f, %3.2f):
  <p>Dist: %3.2f</p> 
  <p>Az: %3.2f</p>
  <p>Baz: %3.2f</p>


"""%(evlat, evlon, stlat, stlon, distaz.getDelta(), distaz.getAz(), distaz.getBaz())

print """
  <p>

  <form method="get" action="distaz.cgi">
    <p>Event Lat: <input type="text" name="evlat" value="%3.2f"/></p>
    <p>Event Lon: <input type="text" name="evlon" value="%3.2f"/></p>
    <p>Station Lat: <input type="text" name="stlat" value="%3.2f"/></p>
    <p>Station Lon: <input type="text" name="stlon" value="%3.2f"/></p>
    <p><input type="submit" value="Submit" /></p>
  </form>

</body>

</html>
""" % (evlat, evlon, stlat, stlon)

