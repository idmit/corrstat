set terminal png size 1024, 512 enhanced font 'Monaco, 10'
set output 'plot.png'
load "palettes/dark2.pal"

if (!exists("filename")) filename='def.dat'

set multiplot layout 1,2

splot filename with points ls 3 title filename

set hidden3d
set dgrid3d splines
splot filename with lines ls 3 title filename

unset multiplot
