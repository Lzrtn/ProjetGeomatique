#ifndef OBJMODEL_H
#define OBJMODEL_H


#include <vector>
#include <map>
#include <string>


class ObjModel
{
public:
    ObjModel();
    ~ObjModel();

    void LoadFromFile(const char* fileName);
    std::vector<float> GetVertexData();
    int GetVertexCount();

private:
        struct Position { float x, y, z; };
        struct Color { float r, g, b; };
        struct Normal { float x, y, z; };

        void LoadMaterialFile(const char* fileName);
        bool StartWith(std::string& line, const char* text);

        void AddVertexData(int vIdx, int nIdx, const char* mtl,
                           std::vector<Position>& vertices, std::vector<Normal>& normals);


        std::map<std::string, Color> mMaterialMap;
        std::vector<float> mVertexData;

};

#endif // OBJMODEL_H
