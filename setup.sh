#!/bin/bash

# Fonction pour afficher un message d'erreur et quitter le script en cas d'échec
function exit_with_error {
    zenity --error --text="Erreur: $1"
    exit 1
}

# Récupérer le chemin du répertoire actuel
current_directory="$PWD"

# Installer les dépendances nécessaires
sudo apt-get update || exit_with_error "Échec de la mise à jour des paquets"
sudo apt-get install -y gdal-bin libgdal-dev libpq-dev libpqxx-dev libgtest-dev xvfb || exit_with_error "Échec de l'installation des dépendances"
sudo apt-get install build-essential qt5-qmake qtbase5-dev qtchooser qtbase5-dev-tools
sudo apt-get install postgresql-14-postgis-3
sudo apt-get install postgresql-15-postgis-3
sudo apt-get install postgresql-16-postgis-3
sudo apt-get install curl
sudo apt-get install apt-transport-https ca-certificates curl software-properties-common
sudo curl -fsSL https://download.docker.com/linux/ubuntu/gpg | sudo gpg --dearmor -o /usr/share/keyrings/docker-archive-keyring.gpg
sudo echo "deb [arch=amd64 signed-by=/usr/share/keyrings/docker-archive-keyring.gpg] https://download.docker.com/linux/ubuntu $(lsb_release -cs) stable" | sudo tee /etc/apt/sources.list.d/docker.list > /dev/null
sudo apt-get install docker-ce docker-ce-cli containerd.io
sudo apt-get install zenity
#sudo apt remove rabbitmq-server
sudo apt-get install libquazip5-dev

# # Se déplacer dans le répertoire du projet
# cd "$PROJET_DIR" || exit_with_error "Échec du changement de répertoire vers $PROJET_DIR"

# Ajouter l'utilisateur au groupe docker
sudo usermod -a -G docker $USER || exit_with_error "Échec de l'ajout de l'utilisateur au groupe docker"

#Build
mkdir -p build 

# Se déplacer dans le répertoire build
cd build || exit_with_error "Échec du changement de répertoire vers build"

# Exécuter qmake pour le projet TiSIG/src
qmake ../TiSIG || exit_with_error "Échec de l'exécution de qmake pour le projet TiSIG/src"

# Compiler le projet TiSIG/src
make  || exit_with_error "Échec de la compilation du projet TiSIG/src"

# Se déplacer dans le répertoire src
cd src || exit_with_error "Échec du changement de répertoire vers src"


# Créer un fichier .desktop directement dans /usr/share/applications
sudo bash -c 'cat <<EOL > /usr/share/applications/TiSIG.desktop
[Desktop Entry]
Name=TiSIG
Comment=MiniSIG
Path='$current_directory'/build/src
Exec='$current_directory'/build/src/src
Icon='$current_directory'/logotisig.png
Terminal=False
Type=Application
Categories=Application;
EOL'

# Afficher un message de réussite
zenity --info --text="Configuration et compilation réussies! Vous pouvez maintenant fermer et rouvrir votre session."

