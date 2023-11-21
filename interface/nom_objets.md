# Noms des variables des objets de l'interface

Les noms sont sous format *typeObj_nameObj*.

## Barre d'outils

- Bouton Switch 2D / 3D : *btn_switchMode2D3D*
- Bouton Zoom + : *btn_zoomIn*
- Bouton Zoom - : *btn_zoomOut*
- Bouton Zoom sur l'emprise totale : *btn_zoomFull*

## Fenêtre principale

- Fenêtre switch (pour passer d'une vue à l'autre) : *stackedWidget*
- Vue 2D :  
  - Fenêtre où se dessine la carte : *graphicsView_window2D*
  - Zone de texte de l'Epsg : *lineEdit_epsg2D*
  - Zone de texte de l'Échelle : *lineEdit_scale2D*
- Vue 3D :
  - Fenêtre où se dessine la carte : *openGLWidget_window3D*
  - Zone de texte de l'Epsg : *lineEdit_epsg3D*
  - Zone de texte de l'Échelle : *lineEdit_scale3D*

## Gestion des couches

- Outils de gestion des couches:
  - Bouton Information : *btn_layerInformation*
  - Bouton Monter la couche dans la liste : *btn_moveLayerUp*
  - Bouton Descendre la couche dans la liste : *btn_moveLayerDown*
  - Bouton Zoom sur l'emprise de la couche : *btn_zoomOnLayer*
  - Bouton Supprimer la couche : *btn_deleteLayer*

- Fenêtre de gestion des couches : *listeWidget_layersList*

- Fenêtre des informations attributaires : *tableWidget_layerAttributeInformation*

## Barre d'actions (menu TiSIG)

  - Aide : *action_help*

## Barre d'actions (menu Couche)

- 2D :
  - Ajouter couche Vecteur : *action_add2DVectorLayer*
  - Ajouter couche Rasteur : *action_add2DRastorLayer*
  - Ajouter couche Flux : *action_add2DDataFlow*

- 3D :
  - Ajouter couche Vecteur : *action_add3DVectorLayer*
  - Ajouter couche Rasteur : *action_add3DRastorLayer*
  - Ajouter couche Flux : *action_add3DModel*

## Fenêtre Ajouter couche Flux

 - Liste des flux pré-enregistrés : *comboBox_dataFlowWindow*
 - Url d'un flux : *lineEdit_dataFlowWindow*
 - Bouton Valider Url : *btn_validateDataFlowUrl*
 - Bouton Valider Pré-enregistré : btn_validateDataFlowPreSave

