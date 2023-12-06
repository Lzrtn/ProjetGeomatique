#include "mnt3dstorage.h"
#include "building3d.h"
#include "mnt3d.h"
#include <iostream>

MNT3dstorage::MNT3dstorage(const std::string baseName, const double xtranslate, const double ytranslate, const std::string pathTexture)
{
    std::string path0 = "../src/data/DONNEES_BDORTHO/"+baseName+"_0.obj";
    std::cout << path0 << std::endl;
    MNT3DFactory *mnt0 = new MNT3DFactory(path0, pathTexture);

    std::string path1 = "../src/data/DONNEES_BDORTHO/"+baseName+"_1.obj";
    MNT3DFactory *mnt1 = new MNT3DFactory(path1, pathTexture);

    std::string path2 = "../src/data/DONNEES_BDORTHO/"+baseName+"_2.obj";
    MNT3DFactory *mnt2 = new MNT3DFactory(path2, pathTexture);

    std::string path3 = "../src/data/DONNEES_BDORTHO/"+baseName+"_3.obj";
    MNT3DFactory *mnt3 = new MNT3DFactory(path3, pathTexture);

    std::string path4 = "../src/data/DONNEES_BDORTHO/"+baseName+"_4.obj";
    MNT3DFactory *mnt4 = new MNT3DFactory(path4, pathTexture);

    std::string path5 = "../src/data/DONNEES_BDORTHO/"+baseName+"_5.obj";
    MNT3DFactory *mnt5 = new MNT3DFactory(path5, pathTexture);

    this->mnt_tiles = {
        {0, mnt0},
        {1, mnt1},
        {2, mnt2},
        {3, mnt3},
        {4, mnt4},
        {5, mnt5},
    };


    this->first = true;

    float x = static_cast<float>(xtranslate);
    float y = static_cast<float>(ytranslate);

    this->setTranslation({x, y, 0});

}

MNT3dstorage::MNT3dstorage(const std::string baseName, const std::string pathTexture)
{
    std::string path0 = "../src/data/DONNEES_BDORTHO/"+baseName+"_0.obj";
    std::cout << path0 << std::endl;
    MNT3DFactory *mnt0 = new MNT3DFactory(path0, pathTexture);

    std::string path1 = "../src/data/DONNEES_BDORTHO/"+baseName+"_1.obj";
    MNT3DFactory *mnt1 = new MNT3DFactory(path1, pathTexture);

    std::string path2 = "../src/data/DONNEES_BDORTHO/"+baseName+"_2.obj";
    MNT3DFactory *mnt2 = new MNT3DFactory(path2, pathTexture);

    std::string path3 = "../src/data/DONNEES_BDORTHO/"+baseName+"_3.obj";
    MNT3DFactory *mnt3 = new MNT3DFactory(path3, pathTexture);

    std::string path4 = "../src/data/DONNEES_BDORTHO/"+baseName+"_4.obj";
    MNT3DFactory *mnt4 = new MNT3DFactory(path4, pathTexture);

    std::string path5 = "../src/data/DONNEES_BDORTHO/"+baseName+"_5.obj";
    MNT3DFactory *mnt5 = new MNT3DFactory(path5, pathTexture);

    this->mnt_tiles = {
        {0, mnt0},
        {1, mnt1},
        {2, mnt2},
        {3, mnt3},
        {4, mnt4},
        {5, mnt5},
    };


    this->first = true;

//	this->setTranslation({5, 0, 0});
}

MNT3dstorage::~MNT3dstorage()
{
    for (auto pair: this->mnt_tiles) {
        delete pair.second;
    }
}

void MNT3dstorage::GetObjectsInEmprise(
        const Emprise &/*emprise*/,
        std::map<int, Object3DFactory *> &new_buildings,
        std::vector<int> &show_objects,
        std::vector<int> &forget_objects)
{
    new_buildings.clear();

    // compute displayed DTM with emprise
    show_objects = {0, 1, 2, 3, 4, 5};

    // give new DTM
    if (this->first) {
        this->first = false;
        for (int id: show_objects) {
            new_buildings[id] = mnt_tiles[id];
        }

    }

    // DTM id to forget
    forget_objects={};
}
