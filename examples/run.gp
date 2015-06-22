set terminal png size 1024, 1024 enhanced font 'Monaco, 10'
set output 'plot.png'
load "palettes/dark2.pal"

if (!exists("filename")) filename='def.dat'

set multiplot layout 2,2

splot filename with points ls 3 title filename

set hidden3d
set dgrid3d splines
splot filename with lines ls 3 title filename

set cntrparam levels 25
set contour
set view map
unset surface
unset key

replot \
	filename  with labels boxed


unset multiplot
