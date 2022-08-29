#!/bin/bash
##########################################################################
# UQAM - Département d'informatique
# INF3105 - Structures de données et algorithmes
# TP2 - Analyse de traces de géolo
# http://ericbeaudry.ca/INF3105/tp2/
#
# Script d'évaluation du TP2
#
# Instructions:
# 1. Déposer ce script avec les fichiers tests dans un répertoire 
#    distinct (ex: tests).
# 2. Se placer dans le répertoire contenant votre programme; OU
#    dans le répertoire contenant la liste des soumissions Oto (*.tp_oto).
# 3. Lancer ce script (ex: ../tests/evaluer.sh).
##########################################################################

# Programme à évaluer
programme="tp2"

echo "UQAM | Département d'informatique"
echo "INF3105 | Structures de données et algorithmes"
echo "Évaluation du programme ${programme} ..."
echo

# Obtenir le chemin du répertoire contenant le présent script et les fichiers tests
pushd `dirname $0` > /dev/null
repertoire_tests=`pwd`
tests=`ls traces[0-9][0-9].txt`
popd  > /dev/null

if [ `pwd` -ef $repertoire_tests ];
then
    echo "Ce script doit être dans un répertoire différent de celui contenant votre ${programme}."
    echo "Ce script a été arrêté afin de ne pas écraser les fichiers traces??+.txt!"
    exit -2;
fi

# Détection si l'utilitaire time sous Linux est disponible (peut afficher 0.00)
echo "Vérification de la commande time..."
/usr/bin/time -f %U echo 2>&1 > /dev/null
souslinux=$?

echo "Limites :"
ulimit -t -v -f

# Détection du CPU
if [ -e /proc/cpuinfo ] ; then
    cpuinfo=`grep "model name" /proc/cpuinfo | sort -u | cut -d ":" -f 2`
else
    cpuinfo="?"
fi


date=`date +%Y%m%d_%H%M%S`

##### La fonction qui évalue un TP ######
function EvaluerTP
{
    ## Forcer la recompilation.
    #rm tp2 *.o
    #make clean   
    
    logfile="log-${date}.txt"
	echo "Les résultats sont déposés dans $logfile."

	echo "Machine : " `hostname`
	echo "#Machine : " `hostname`  > $logfile
	echo "CPU : $cpuinfo"
	echo "#CPU : $cpuinfo"  > $logfile
	echo "Date début : $date"
	echo "#Date début : $date"  >> $logfile
	echo "Limite de `ulimit -t` secondes par test"
	echo "#Limite de `ulimit -t` secondes par test"  >> $logfile

    # Pour statistiques : nombre de lignes de code...
    echo "Taille des fichiers source :" >> ${logfile}
    wc *.{c*,h*}  >> ${logfile}

    taille=`wc *.{c*,h*} | grep total`
    sommaire="$taille"
    
    make $programme
    
    # Vérification de l'existance du programme.
    if [ ! -e ${programme} ]
	    then
	    echo "  ERREUR : le fichier ${programme} est inexistant!"
	    return
    fi
    if [ ! -x ${programme} ]
	    then
	    echo "  ERREUR : le fichier ${programme} n'est pas exécutable!"
	    return
    fi


	echo 
	echo "#" >> $logfile

	echo -e "Fichier_Test\tPers\tTraces\tCorrect\tCPU\tMém.(k)"
	echo -e "Fichier_Test\tPers\tTraces\tCorrect\tCPU\tMém.(k)" >> $logfile

	for test in $tests;
	do
    	fichiertest="$repertoire_tests/$test"
    	nbpersonnes=`grep -E '[a-zA-Z][a-zA-Z \-]*' $fichiertest | wc -l`
    	nbtraces=`grep -o -E ';' $fichiertest | wc -l`
    	fichierresultat="${test%.txt}+.txt"
    	fichiersolution="$repertoire_tests/${test%.txt}+.txt"
    
        {
            # Fixer les limites : 60 s, 512Mo mémoire et 1Mo fichier
            #ulimit -t 60 -v 524288 -f 1024

	        if [ $souslinux -eq 0 ]; then
	            tcpu="`ulimit -t 600 -v 524288 -f 1024 && (/usr/bin/time -f "%U\t%Mk" ./$programme $fichiertest > $fichierresultat) 2>&1 | tail -n 1`"
        	else
            	tcpu=`(ulimit -t 60 -v 524288 -f 1024 && time -p ./$programme $fichiertest > $fichierresultat) 2>&1 | egrep user | cut -f 2 -d " "`
	        fi
	        
	    }
    
	    optimal="?"
    	if( [ -e $fichiersolution ] )
    	then
        	diff -tibw $fichierresultat $fichiersolution 2>&1 > /dev/null
    		if [ $? -eq 0 ];
    		then
    	    	optimal="OK"
     		else
     	    	optimal="Echec"
    		fi
    	fi

    	echo -e "$test\t$nbpersonnes\t$nbtraces\t$optimal\t$tcpu"
    	echo -e "$test\t$nbpersonnes\t$nbtraces\t$optimal\t$tcpu" >> $logfile
    	sommaire="${sommaire}\t${optimal}\t${tcpu}"
	done
}



function Correction
{
    # Lister les soumissions Oto (répertoires terminant par .tp_oto)
    tps=`ls -1 | grep .tp_oto`
    if [ ! -n "$tps" ]; then
      echo "allo"
      tps="."
    fi

    echo "#Rapport de correction" > "rapport-${date}.txt"
    echo "#Date: ${date}" >> "rapport-${date}.txt"
    echo -e -n "#\t" >> "rapport-${date}.txt"
    for test in ${tests}; do
       echo -e -n "\t${test}\t\t" >> "rapport-${date}.txt"
    done
    echo >> "rapport-${date}.txt"
    echo -e -n "#Soumission\tTaille sources" >> "rapport-${date}.txt"
    for test in ${tests}; do
       echo -e -n "\tOpt?\tCPU\tMem" >> "rapport-${date}.txt"
    done
    echo >> "rapport-${date}.txt"
    echo 
    for tp in $tps; do
        sommaire=""
        echo "## CORRECTION : $tp"
        pushd $tp
        	EvaluerTP
        popd
    #    echo -e ">> ${sommaire}"
        echo -e "${tp}\t${sommaire}" >> "rapport-${date}.txt"
    done
}

EvaluerTP
#Correction
