#!/bin/bash
export GDFONTPATH=/usr/share/fonts/truetype/msttcorefonts

for img in `ls *.bmp`
	do
		convert $img $img.png
	done
# renomeia os arquivos convertidos, excluindo a extensão antiga
rename 's/.bmp//' *.png
rm *.bmp
