#########################################################
# Fichier MAKE pour EG Library
# Par Asher256
#
# Pour contacter l'auteur: asher256@gmail.com
#########################################################

.PHONY: badeg

# check that EG has a good directory structure
ifeq ($(wildcard include/eg.h),)
badeg:
	 @echo Bad EG installation! You did not preserve the directory structure
	 @echo while unzipping it: did you remember to use the -d flag with pkunzip?
endif

# Vars
DOC_FILE = src/eg._tx

################## Les fichiers #######################
OBJS = anim2d$(O) camera$(O) cheat$(O) console$(O) credit$(O) ega$(O) egobject$(O) filef$(O) gui$(O) lanim$(O) map$(O) mapl$(O) misc$(O) mpos$(O)

EXES = tools/eganim$(EXE) examples/excredit$(EXE) examples/excheat$(EXE) docs/makedoc$(EXE)
#######################################################

### Le fichier include de la plateforme actuelle
include make.ad

C = .c

####### PAR DEFAUT ##################
default: lib exes doc
##################################### 

## Fonctions a inclure dans les fichiers *.AD
RM_EXES = $(RM) $(EXES)
RM_O    = $(RM) $(OBJ_DIR)/*$(O)
RM_DOC  = $(RM) docs/htm/eg*.htm docs/txt/eg*.txt docs/rtf/eg*.rtf
RM_LIB  = $(RM) $(LIB_FILE)

################ COMPILATION DES FICHIERS
$(OBJ_DIR)/%$(O) : $(SRC_DIR)/%$(C)
	$(CC) $(AVANT_CPP) $< $(AVANT_O)$@ $(CFLAGS)

#****>>> Fichier d'Exemples
tools/eganim$(EXE): tools/eganim$(C)
	    $(MAKE_EXE1) tools/eganim$(C) $(LIB_FILE) $(MAKE_EXE2)tools/eganim$(EXE) $(LIBRARY)
examples/exspeed$(EXE): examples/exspeed$(C)
	  $(MAKE_EXE1) examples/exspeed$(C) $(LIB_FILE) $(MAKE_EXE2)examples/exspeed$(EXE) $(LIBRARY)

examples/exlselect$(EXE): examples/exlselect$(C)
	   $(MAKE_EXE1) examples/exlselect$(C) $(LIB_FILE) $(MAKE_EXE2)examples/exlselect$(EXE) $(LIBRARY)
examples/excredit$(EXE): examples/excredit$(C)
	   $(MAKE_EXE1) examples/excredit$(C) $(LIB_FILE) $(MAKE_EXE2)examples/excredit$(EXE) $(LIBRARY)
examples/excheat$(EXE): examples/excheat$(C)
	$(MAKE_EXE1) examples/excheat$(C) $(LIB_FILE) $(MAKE_EXE2)examples/excheat$(EXE) $(LIBRARY)
examples/exfps$(EXE): examples/exfps$(C)
	 $(MAKE_EXE1) examples/exfps$(C) $(LIB_FILE) $(MAKE_EXE2)examples/exfps$(EXE) $(LIBRARY)

############# COMPILATION DES EXEMPLES #####################
exes: $(EXES)
	$(RM_OBJS)
############################################################

############# COMPILATION DES LIBS #########################
lib: $(addprefix $(OBJ_DIR)/, $(OBJS))
	 $(AR) $(AR_OPTION) $(OBJ_DIR)/*$(O) $(AR_OPTION2)
############################################################

############# COMPILATION DE LA DOC ########################
docs/makedoc$(EXE): docs/src/makedoc.c
	$(MAKE_EXE1) docs/src/makedoc.c $(MAKE_EXE2)docs/makedoc$(EXE)

doc: 
	docs/makedoc$(EXE) docs/$(DOC_FILE) -rtf docs/rtf/eg.rtf
	docs/makedoc$(EXE) docs/$(DOC_FILE) -ascii docs/txt/eg.txt
	docs/makedoc$(EXE) docs/$(DOC_FILE) -htm docs/htm/eg.htm
	$(RM_OBJS)

############################################################

############## INSTALLATION ################################
install:
	$(CP) $(LIB_FILE) $(CC_LIB_DIR)
	$(CP) $(INCLUDE_FILE) $(CC_INCLUDE_DIR)
############################################################



############## EFFACER LA LIBRAIRIE ########################
clean:
	 $(RM_EXES)
	 $(RM_DOC)
	 $(RM_LIB)
	 $(RM_O)

uninstall:
	 $(RM) $(CC_LIB_DIR)/libeg.a
	 $(RM) $(CC_INCLUDE_DIR)/eg.h
############################################################

