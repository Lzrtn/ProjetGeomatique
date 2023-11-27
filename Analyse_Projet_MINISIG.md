# Analyse Projet MINISIG

## Planning
- Pas de gant : mettre backlog du produit avec backlog des sprint

## Utilisateur
- Victor Coindet, personne voulant utiliser un minisig 2D/3D
- Pour l’école
- Pas d’enjeux, projet de classe
- Aspects financier : aucun, 3,5 semaines de cours
- Aspects sociaux : Rôles des gens et équipes, l’organisation établie au sein du groupe, les rôles ainsi que des commentaires sur le déroulé des opérations.

## Lexique/Glossaire
- SIG
- CityGML
- 3D

## Objectif
### Minisig
- Visualiseur de données 2D/3D
- Français
- **2D:**
  - Orthophotographie, vecteur + raster + flux ign : WMS, WFS, WMTS
  - Chargement de données locales (SHP et GeoTIFF au minimum)
  - Au clic, affichage type de bâti (métadonnées)
  - Interface de gestion de couches
  - Bonus:
    - Gestion du style (SLD)
    - Opérations géométriques 2D: intersections, buffer
    - Édition 2D des attributs des couches vecteurs
- **3D:**
  - Visualisation de couches images (orthophotos plaquées sur MNT) et vecteurs
  - Les bâtiments en 3D seront soit extrudés1 à partir d’un format 2,5D, soit chargés à partir d’un vrai format 3D: CityGML2
  - Pour test front : à décider (sûrement citygml) : data du grand Lyon ou BDTOPO
  - Quartier de Lyon à décider
  - Interface de gestion de couches
  - Gestion de la caméra (zoom, dézoom, pan, rotations)
  - Bonus:
    - Calcul d’indicateurs morphologiques 3D simples sur les bâtiments:
      - Hauteur max
      - Hauteur à la gouttière (point le plus bas du toit)
      - Surface de plancher (somme des surfaces des planchers d’un bâtiment en comptant les étages)
      - Ouverture de ciel (pourcentage de ciel visible depuis un point donné)
- **Interface:**
  - Bouton switch 2D/3D
  - Onglet gestion des couches : masquer/Afficher supprimer zoom sur couche, flèche index des couches

## Architecture Technique
- Client Lourd avec un front-end et un back-end + BDD + flux

## Dépôt git
- Avec readme, comment installer et utiliser l’outil

## Contraintes
- Langages : cpp et OpenGL, mode AGILE, temps 3,5 semaines, à rendre pour 8 décembre au soir

## Acteurs
- Victor et acteurs potentiels

## Analyse fonctionnelle
- Diagramme activité et classes à définir

## Étude technique
- Choix des langages : Cpp avec OpengL et formats de données à définir

## Réalisation et suivi de projet
- Risques à définir

## Planning prévisionnel
- Voir trello + méthode AGILE
