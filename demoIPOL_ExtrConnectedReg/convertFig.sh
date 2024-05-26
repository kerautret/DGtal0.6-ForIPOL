#! /bin/sh
PATH=/usr/bin/:/usr/local/bin:/opt/local/bin


fig2dev -L eps $1 resu.eps
# Alternative to convert due to gs security issue (called by convert when converting eps file).
# convert -density 50 resu.eps $2
# test: ps2pdf -dEPSCrop  resu.pdf
gs -q -dNOCACHE -dEPSCrop -dNOPAUSE -dBATCH -dSAFER -sDEVICE=png16m -r120  -sOutputFile=$2 resu.eps

