INCLUDEPATH ''= $$system(pwd)/include
DEPENDSPATH''= $$system(pwd)

# Spécifiez la version minimale de qmake
QT += core

# Définissez la version du langage C++
CONFIG += c++17

# Déclarez votre exécutable et les fichiers source
SOURCES +=  ../src/outils/dbmanager.cpp\
            ../src/outils/executor.cpp\
            ../src/outils/docker.cpp
HEADERS +=  ../src/outils/dbmanager.h \
            ../src/outils/executor.h \
            ../src/outils/docker.h

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Ajoutez les bibliothèques nécessaires
LIBS += -lpqxx
