# test_db.pro

# Spécifiez la version minimale de qmake
QT += core

# Définissez la version du langage C++
CONFIG += c++17

# Déclarez votre exécutable et les fichiers source
SOURCES += main.cpp dbmanager.cpp executor.cpp docker.cpp
HEADERS += dbmanager.h executor.h docker.h

# Ajoutez les flags de compilation nécessaires pour C++
QMAKE_CXXFLAGS += -std=c++17

# Ajoutez les bibliothèques nécessaires
LIBS += -lpqxx
