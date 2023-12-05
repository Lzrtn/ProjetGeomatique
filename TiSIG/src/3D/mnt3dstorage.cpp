#include "mnt3dstorage.h"
#include "building3d.h"
#include "mnt3d.h"
#include <iostream>


MNT3dstorage::MNT3dstorage()
{

    this->mnt_tiles = {
        {0, new MNT3DFactory(0)},
        {1, new MNT3DFactory(1)},
        {2, new MNT3DFactory(2)},
        {3, new MNT3DFactory(3)},
        {4, new MNT3DFactory(4)},
        {5, new MNT3DFactory(5)},
        {6, new MNT3DFactory(6)},
        {7, new MNT3DFactory(7)},
    };

//	this->setTranslation({5, 0, 0});
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

    std::string path6 = "../src/data/DONNEES_BDORTHO/"+baseName+"_6.obj";
    MNT3DFactory *mnt6 = new MNT3DFactory(path6, pathTexture);

    std::string path7 = "../src/data/DONNEES_BDORTHO/"+baseName+"_7.obj";
    MNT3DFactory *mnt7 = new MNT3DFactory(path7, pathTexture);

    this->mnt_tiles = {
        {0, mnt0},
        {1, mnt1},
        {2, mnt2},
        {3, mnt3},
        {4, mnt4},
        {5, mnt5},
        {6, mnt6},
        {7, mnt7},
    };

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

    // compute displayed buildings with emprise
//    show_objects = {0, 1, 2, 3, 4};
    show_objects = {0, 1, 2, 3, 4, 5, 6, 7};

    // give new buildings
    for (int id: show_objects) {
        new_buildings[id] = mnt_tiles[id];
    }

    // building id to forget
    forget_objects={};
}
