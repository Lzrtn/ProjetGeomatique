#ifndef MNT3DSTORAGE_H
#define MNT3DSTORAGE_H


#include "i_openglcityview.h"

/**
 * @brief The ExempleObject3DStorage class
 *
 * Exemple of Object3DStorage
 */
class MNT3dstorage : public Object3DStorage
{
public:
    MNT3dstorage();
    MNT3dstorage(const std::string baseName, const std::string pathTexture);
    ~MNT3dstorage();

    void GetObjectsInEmprise(
            const Emprise &/*emprise*/,
            std::map<int, Object3DFactory*> &new_buildings,
            std::vector<int> &show_objects,
            std::vector<int> &forget_objects);

private:
    std::map<int, Object3DFactory*> mnt_tiles;
};


#endif // MNT3DSTORAGE_H
