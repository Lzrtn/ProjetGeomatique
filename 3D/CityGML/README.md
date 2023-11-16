# EXPLICATION

FAIT PAR HANNICK ET MATHEO

## Exemple d'un bâtiment

```xml
<cityObjectMember>
  ..
    Suite de déinitions d'attributs
    //Exemple d'un composant :
    <gen:stringAttribute name="ADRESSE">
      <gen:value>38b Rue Chazay</gen:value>
    </gen:stringAttribute>
  .. 
  ..
  //Exemple d'un composant :
  <app:appearance>
    <app:target uri="#UUID_aa7a750e-f592-49b2-a5f1-8d98c5396079">
      <app:TexCoordList>
        <app:textureCoordinates ring="#UUID_5f88ae4c-c3a0-48c7-8e58-36c5464ed46d">0.332493 0.603582 0.003390 0.474469 0.097736 0.361596 0.277570 0.429379 0.268268 0.442754 0.414614 0.497575 0.332493 0.603582 </app:textureCoordinates>
      </app:TexCoordList>
    </app:target>
  </app:appearance>
  ..
  ..
  <bldg:lod2Solid>
    <gml:Solid gml:id="UUID_f8067a7a-020f-4538-9a25-90064971af76">
      <gml:exterior>
        <gml:CompositeSurface gml:id="UUID_b031d917-55ea-4fc0-82b4-c4063758be0b">
          <gml:surfaceMember xlink:href="#UUID_aa7a750e-f592-49b2-a5f1-8d98c5396079"/>
          <gml:surfaceMember xlink:href="#UUID_9efebde2-22ec-4bda-bf51-86f7f845af96"/>
          <gml:surfaceMember xlink:href="#UUID_c7cabff5-76d0-4759-a06d-a1023e21800b"/>
          <gml:surfaceMember xlink:href="#UUID_82286e58-68cf-4786-b8c4-2020c0941e98"/>
          <gml:surfaceMember xlink:href="#UUID_e675e737-c1d5-40df-a9d6-eaef4d6df4c6"/>
        </gml:CompositeSurface>
      </gml:exterior>
    </gml:Solid>
  </bldg:lod2Solid>
  ..
  ..
    Suite de définitions de RoofSurface ou WallSurface
    //Exemple d'un composant :
    <bldg:boundedBy>
      <bldg:RoofSurface gml:id="UUID_81e955fc-3f10-4509-a824-4e87fa94f87d">
        <bldg:lod2MultiSurface>
          <gml:MultiSurface gml:id="UUID_e471228d-da1f-4b4a-9ebf-be1235b56200" srsDimension="3">

            <gml:surfaceMember>
              <gml:Polygon gml:id="UUID_aa7a750e-f592-49b2-a5f1-8d98c5396079">
                <gml:exterior>
                  <gml:LinearRing gml:id="UUID_5f88ae4c-c3a0-48c7-8e58-36c5464ed46d">
                    <gml:posList>1838829.24900000 5173628.51500000 278.90700000 1838825.07800000 5173636.39000000 278.90700000 1838821.39700000 5173634.14700000 278.90700000 1838823.58596742 5173629.84406430 278.90700000 1838824.02200000 5173630.06500000 278.90700000 1838825.79239022 5173626.56319728 278.90700000 1838829.24900000 5173628.51500000 278.90700000 </gml:posList>
                  </gml:LinearRing>
                </gml:exterior>
              </gml:Polygon>
            </gml:surfaceMember>

          </gml:MultiSurface>
        </bldg:lod2MultiSurface>
      </bldg:RoofSurface>
    </bldg:boundedBy>
  ..
</cityObjectMember>
```

`<attributs>` définit par exemple : 

- adresse
- enveloppe (contient Upper et Lower Corner, et représente alors l'emprise 2D de l'objet)
- hauteur min, max et moy
- buildingid
- dates
- beaucoup d'informations en somme et représente en quelques sortes des métadonnées

à noter que ces attributs se suivent et n'ont pas de réel parentCLass 'attributs'

`<appearance>` définit : 

- la texture utilisée pour le bâtiment en cherchant l'image : "LYON_5EME_BATI_2015_Appearance/{id_bat}.jpg
  avec id_bat = BUILDINGID.value = cityobject.gmlid
- chaque target a un uri qui réfère a un surfaceMember
- 4 paires de UVcoordinates qui sont définies par un id appelé "ring" (4x2)

`<lod2Solid>` définit `<CompositeSurface>` qui possède toutes les surfaces définies plus bas, ces deux composants ont des ids crées sur le tas, mais chaque surfaceMember définit ici a un xlink:href qui mène vers sa définition complète.

En sachant que :
- un surfaceMember est une surface de toit ou de mur
- a un enfant Polygon qui représente sa géométrie
- qui a lui même un enfant linearRing qui renvoie vers la texture correspondante (même id)
- qui possède une liste de positions (4x3) qui permet à chaque position UV d'être associé à une position réel physique.

## Bilan

  - id du polygon d'une surface (toit ou mur)
  - xlink:href
  - uri d'une target

sont les mêmes, tout comme le ring des coordonnées texture et le LinearRing d'un Polygon
    





















