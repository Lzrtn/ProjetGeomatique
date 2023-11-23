# TSI23_minisig

## Changement des droits administrateurs docker

Pour avoir les droits administrateur et lancer des dockers sans problème, lancer ces deux commandes :

```
curl https://get.docker.com/ | sudo sh
sudo usermod -a -G docker $USER
```

Au besoin, fermez votre session et rouvrez-la.

## Arborescence des fichiers

L'application se trouve dans le dossier **TiSIG**.
Il faut inclure vos codes dans les différents sous-dossiers de TiSIG (2D, 3D, interface, outils pour le moment) et vérifier que les appels aux fichiers sources fonctionnent bien.
