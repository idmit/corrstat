# set terminal png size 1024, 512 enhanced font 'Monaco, 10'
# set output 'plot.png'
# load "palettes/dark2.pal"

set terminal epslatex size 16cm, 8cm color colortext
set output plotname

if (!exists("filename")) filename='def.dat'

set multiplot layout 1,2

set xlabel vxlabel
set ylabel vylabel
set zlabel '$\widehat{c}$'

set format x '\ft $%g$'
set format y '\ft $%g$'
set format z '\ft $%g$'

set xrange[0:1]
set yrange[0:1]

set xtics 0, 0.2, 1
set ytics 0, 0.2, 1
set ztics 0, 3, 2000

set hidden3d
set dgrid3d splines
splot filename with lines ls 3 title vtitle

set xtics autofreq
set ytics autofreq
unset ztics

set cntrparam levels 25
set contour
set view map
unset surface
unset key

replot filename with lines

unset multiplot
