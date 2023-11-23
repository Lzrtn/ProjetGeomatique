TEMPLATE = subdirs

SUBDIRS += 2D \
           3D \
           interface \
           outils \



## Spécifiez le chemin relatif vers le dossier icons
#DOSSIER_SOURCE = $$PWD/icons

## Dossier de destination dans le répertoire de build
#MKDIR_CMD = mkdir -p  # Utilisez -p pour créer le dossier parent s'il n'existe pas
#MKDIR_DESTINATION = $$OUT_PWD
#MKDIR = $$MKDIR_CMD $$MKDIR_DESTINATION
#QMAKE_PRE_LINK += $$MKDIR

## Copie des fichiers du dossier source vers le dossier de destination
#COPY_CMD = cp -r  # "xcopy" sur Windows
#COPY_FILES = $$COPY_CMD $$DOSSIER_SOURCE/* $$MKDIR_DESTINATION
#QMAKE_PRE_LINK += $$COPY_FILES

#message("Chemin absolu du dossier source: $$PWD")
#message("Chemin absolu du dossier de destination: $$OUT_PWD")

#exists($$MKDIR_DESTINATION) {
#    message("Le dossier de destination existe: $$MKDIR_DESTINATION")
#} else {
#    error("Le dossier de destination n'existe pas: $$MKDIR_DESTINATION")
#}

copydata.commands = $(COPY_DIR) $$PWD/icons $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
