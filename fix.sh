#!/bin/sh
###############################################################################
# Fichier pour fixer la plate forme pour Unix/Linux et BeOS
#
# Par Asher256
###############################################################################

### Fixation pour unix ###
# $1: fichier/$1.ad
# $2: Djgpp
fix ()
{
    echo "Fixation pour $2..."
    cp "misc/$1.ad" "make.ad"
    echo "Termine..."
    echo ""
}

### Aide pour le ligne de commande ###
error()
{
    echo "          linux              pour linux/gcc ou autres *NIX" 
    echo "          djgpp              pour Djgpp/dos" 
    echo "          mingw32            pour Mingw32/Windows" 
    echo "          msvc               pour Visual C++"
    echo 
    echo "Exemple: \"./fix.sh unix\""
    echo 
}


### >>> Debut du programme <<< ###
echo "Programme de fixation de plateforme..."
echo "Choisissez votre PlateForme"
echo ""

case "$1" in
    "linux" ) fix "linux"    "Linux";;
    "djgpp" ) fix "djgpp"    "Djgpp";;
    "mingw" ) fix "mingw32"  "Mingw32";;
    "msvc"  ) fix "msvc"     "Visual C++";;
    * ) error;;
esac

### >>> Fin du programme  <<< ###
