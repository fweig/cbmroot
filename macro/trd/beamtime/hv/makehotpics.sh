#!/bin/bash

# Bitte, wohin geht die Lieferung?
OUTFILE=allpics.tex

# Inventur.
ls -1 *.png > allnames.config
NROFFILES=$(awk 'END{print NR}' allnames.config)

# Wir sind ein rücksichtsloses Skript.
rm -f $OUTFILE

# HIER NOCH EIN PAAR LATEX-HEADER SCHREIBEN
echo "\documentclass[a4paper,10pt,DIV9,headsepline,oneside,abstract]{scrreprt}" >> $OUTFILE
echo "\usepackage[left=0.01cm,right=0.01cm,top=0.2cm,bottom=0.4cm]{geometry}" >> $OUTFILE
echo "\usepackage{graphicx}" >> $OUTFILE
echo "\usepackage[font=small]{caption}" >> $OUTFILE
echo "\begin{document}" >> $OUTFILE


# Jetzt loopen wir die Bilder ins TeX.
FILE=1
while [ $FILE -le $NROFFILES ]
do
    # Dateinamen bereitlegen
    THISNAME=$(sed -n ${FILE}p allnames.config)

    # Latex schreiben
    echo "\begin{figure}[!h]" >> $OUTFILE
    echo "\centering" >> $OUTFILE
    echo "\includegraphics[width=21.5cm]{$THISNAME}" >> $OUTFILE

    if [ $((FILE%2)) -eq 0 ]
    then
    THISNAME=${THISNAME/_/"\_"}
    THISNAME=${THISNAME/"_hv"/"\_hv"}
    THISNAME=${THISNAME/"-VoltageTrend.png"/""}
    echo "\caption*{$THISNAME}" >> $OUTFILE
    fi
    echo "\end{figure}" >> $OUTFILE
    if [ $((FILE%2)) -eq 0 ]
    then
	echo "\clearpage" >> $OUTFILE
    fi
    
    # Sehen Sie in unserer nächsten Folge:
    let FILE=$FILE+1
done

# Kiste harmonisch zunageln.
echo "\end{document}" >> $OUTFILE

# Einmal feucht durchwischen.
rm allnames.config

# Bisschen PR. Wenn der Plural falsch ist, wird der Nutzer *das* nun auch nicht mehr merken.
echo "$OUTFILE mit $((FILE-1)) Bildern erzeugt."
