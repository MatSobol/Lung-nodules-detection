#include <fstream>

#include "read.hpp"
#include "const.hpp"

std::pair<std::vector<float>, std::array<float, 3>> readVertices()
{
    std::ifstream file(filePath);

    int size;

    file >> size;

    std::vector<float> vertices(size);

    float maxX, maxY, maxZ;

    file >> maxX >> maxY >> maxZ;

    std::array<float, 3> maxValues = {maxX * scale[0], maxY * scale[1], maxZ * scale[2]};

    float val;

    while (file >> val)
    {
        vertices.push_back(val);
    }

    file.close();

    return {vertices, maxValues};
}