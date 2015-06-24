set terminal png size 1024, 1024 enhanced font 'Monaco, 10'
set output plotname
# load "palettes/dark2.pal"

if (!exists("filename")) filename='def.dat'

binwidth = 4
binstart = -100
load '/Users/ivandmi/Documents/dev/corrstat/corrstat/data/shift_marginals/hist.fct'
plot filename i 0 @hist ls 1