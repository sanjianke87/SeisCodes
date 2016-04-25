#!/bin/bash
R=0/360/-90/0
J=S0/-90/15c
B=a30f10N
name=pattern
PS=BeachBall.ps

gmtset BASEMAP_TYPE=plain
gmtset PLOT_DEGREE_FORMAT=+
xyz2grd ${name}.dat -G${name}.nc -I6m/6m -R$R -ZLBxf
grd2cpt ${name}.nc -Cpolar -E100 > ${name}.cpt

psxy -R$R -J$J -T -K -P > $PS
grdimage ${name}.nc -R$R -J$J -C${name}.cpt -B$B -K -O >> $PS
grdcontour ${name}.nc -R$R -J$J -L-0.1/0.1 -C1 -K -O -W2p >> $PS
psxy -R$R -J$J -T -O >> $PS
rm .gmt* ${name}.nc ${name}.cpt
