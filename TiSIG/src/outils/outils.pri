INCLUDEPATH ''= $$system(pwd)/include
DEPENDSPATH''= $$system(pwd)

# Spécifiez la version minimale de qmake
QT += core

# Définissez la version du langage C++
CONFIG += c++17

# Déclarez votre exécutable et les fichiers source
SOURCES +=  outils/dbmanager.cpp\
            outils/executor.cpp\
            outils/docker.cpp
HEADERS +=  outils/dbmanager.h \
            outils/executor.h \
            outils/docker.h

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Ajoutez les bibliothèques nécessaires
LIBS += -lpqxx
