set terminal png size 1024, 1024 enhanced font 'Monaco, 10'
set output plotname

if (!exists("filename")) filename='def.dat'

set view map
plot filename with points ls 3 title filename

# set cntrparam levels 25
# set contour
# set view map
# unset surface
# unset key

# replot \
# 	filename  with labels boxed

# unset multiplot
