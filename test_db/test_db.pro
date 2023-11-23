# test_db.pro

# Spécifiez la version minimale de qmake
QT += core

# Définissez la version du langage C++
CONFIG += c++17

# Déclarez votre exécutable et les fichiers source
SOURCES += main.cpp dbmanager.cpp docker.cpp executor.cpp
HEADERS += dbmanager.h docker.h executor.h

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Ajoutez les bibliothèques nécessaires
LIBS += -lpqxx
