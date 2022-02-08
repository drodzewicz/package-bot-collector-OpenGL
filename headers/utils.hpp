#ifndef __UTILS
#define __UTILS

#include "libs.hpp"


typedef struct MaterialParam
{
	glm::vec3 Ambient;
	glm::vec3 Diffuse;
	glm::vec3 Specular;
	GLfloat Shininess;

} MaterialParam;


struct Character
{
    unsigned int TextureID; // ID handle of the glyph texture
    glm::ivec2 Size;        // Size of glyph
    glm::ivec2 Bearing;     // Offset from baseline to left/top of glyph
    unsigned int Advance;   // Horizontal offset to advance to next glyph
};

struct Vertex
{
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture
{
    unsigned int id;
    std::string type;
};

struct LightParam
{
    glm::vec3 Ambient;
    glm::vec3 Diffuse;
    glm::vec3 Specular;
    glm::vec3 Att;
    glm::vec3 Position;
};

std::vector<int> randUnique(int amount, int range, int min = 0) {

    std::vector<int> indexes;
    while(indexes.size() < amount) {
        int num = rand() % range + min;
        if(!(std::find(indexes.begin(), indexes.end(), num) != indexes.end())) {
            indexes.push_back(num);
        }
    }
    return indexes;
}
void createWindow(const char *title, int width, int height)
{
    glutInitWindowSize(width, height);

    // CENTER WINDOW ON THE SCREEN
    glutGet(GLUT_SCREEN_WIDTH);
    glutGet(GLUT_SCREEN_HEIGHT);
    glutInitWindowPosition((glutGet(GLUT_SCREEN_WIDTH) - width) / 2,
                           (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);
    glutCreateWindow(title);
}


unsigned long getFileLength(std::ifstream &file)
{
    if (!file.good())
        return 0;

    unsigned long pos = file.tellg();
    file.seekg(0, std::ios::end);
    unsigned long len = file.tellg();
    file.seekg(std::ios::beg);

    return len;
}

GLchar *LoadShaderFile(const char *filename)
{
    std::ifstream file;
    file.open(filename, std::ios::in); // opens as ASCII!
    if (!file)
    {
        printf("ERROR::[CAN'T_OPEN_FILE:{%s}]\n", filename);
        exit(1);
    }

    unsigned long len = getFileLength(file);

    if (len == 0)
    {
        printf("ERROR::[FILE_IS_EMPTY:{%s}]\n", filename);
        exit(1);
    }; // Error: Empty File

    GLchar *ShaderSource = new GLchar[len + 1];
    if (ShaderSource == NULL)
    {
        printf("ERROR::[CAN'T_RESERVE_MEMORY: %d]\n", len + 1);
        exit(1);
    } // can't reserve memory

    // len isn't always strlen cause some characters are stripped in ascii read...
    // it is important to 0-terminate the real length later, len is just max possible value...
    ShaderSource[len] = 0;

    unsigned int i = 0;
    while (file.good())
    {
        ShaderSource[i] = file.get();
        if (!file.eof())
            i++;
    }

    ShaderSource[i] = 0;
    file.close();
    return ShaderSource;
}

std::vector<Vertex> loadModel(const char *objectPath)
{
    std::vector<Vertex> vertices;
    std::cout << "LOADING_OBJ [" << objectPath << "] ... \n";

    std::vector<unsigned int> vertexIndices, uvIndices, normalIndices;
    std::vector<glm::vec3> temp_vertices;
    std::vector<glm::vec2> temp_uvs;
    std::vector<glm::vec3> temp_normals;

    FILE *file = fopen(objectPath, "r");
    if (file == NULL)
    {
        std::cout << "ERROR::[CAN'T_OPEN_FILE]\n";
        exit(1);
    }

    while (1)
    {

        char lineHeader[128];

        // read the first word of the line
        int res = fscanf(file, "%s", lineHeader);

        if (res == EOF)
            break; // EOF = End Of File. Quit the loop.

        // read vertex coordinates
        if (strcmp(lineHeader, "v") == 0)
        {
            glm::vec3 vertex;
            fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z);
            temp_vertices.push_back(vertex);
        }
        // read texture coordinates
        else if (strcmp(lineHeader, "vt") == 0)
        {
            glm::vec2 uv;
            fscanf(file, "%f %f\n", &uv.x, &uv.y);
            // Invert V coordinate since we will only use DDS texture,
            // which are inverted. Remove if you want to use TGA or BMP loaders.
            uv.y = -uv.y;
            temp_uvs.push_back(uv);
        }
        // read normal vectors
        else if (strcmp(lineHeader, "vn") == 0)
        {
            glm::vec3 normal;
            fscanf(file, "%f %f %f\n", &normal.x, &normal.y, &normal.z);
            temp_normals.push_back(normal);
        }
        // read faces (triangles)
        else if (strcmp(lineHeader, "f") == 0)
        {
            std::string vertex1, vertex2, vertex3;
            unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
            int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0],
                                 &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2]);

            if (matches != 9)
            {
                printf("\nFile can't be read by our simple parser. ");
                printf("Try exporting with other options and make sure you export normals and uvs.\n");
                fclose(file);
                exit(1);
            }

            // Create new triangle
            vertexIndices.push_back(vertexIndex[0]);
            vertexIndices.push_back(vertexIndex[1]);
            vertexIndices.push_back(vertexIndex[2]);
            uvIndices.push_back(uvIndex[0]);
            uvIndices.push_back(uvIndex[1]);
            uvIndices.push_back(uvIndex[2]);
            normalIndices.push_back(normalIndex[0]);
            normalIndices.push_back(normalIndex[1]);
            normalIndices.push_back(normalIndex[2]);
        }
        else
        {
            // Probably a comment, eat up the rest of the line
            char stupidBuffer[1000];
            fgets(stupidBuffer, 1000, file);
        }
    }

    // For each vertex of each triangle
    for (unsigned int i = 0; i < vertexIndices.size(); i++)
    {

        // Get the indices of its attributes
        unsigned int vertexIndex = vertexIndices[i];
        unsigned int uvIndex = uvIndices[i];
        unsigned int normalIndex = normalIndices[i];

        // Get the attributes thanks to the index
        Vertex vertex;
        vertex.Position = temp_vertices[vertexIndex - 1];
        vertex.Normal = temp_normals[normalIndex - 1];
        vertex.TexCoords = temp_uvs[uvIndex - 1];

        // Put the attributes in buffers
        vertices.push_back(vertex);
    }
    fclose(file);
    std::cout << "OBJECT LOADED!\n";
    return vertices;
}


#endif