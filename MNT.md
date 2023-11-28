https://gis.stackexchange.com/questions/121561/seeking-tool-to-generate-mesh-from-dtm

https://users.cs.northwestern.edu/~gaming/libtif_help.html



Texture::Texture(const std::string& path)
    : m_RendererID(0), m_FilePath(path), m_LocalBuffer(nullptr), m_Width(0), m_Height(0), m_BPP(0)
{
    stbi_set_flip_vertically_on_load(1);

    // Check if the file has a .tif extension
    if (path.substr(path.find_last_of(".") + 1) == "tif") {
        readTiffImage((char*)path.c_str(), &m_RendererID);
    } else {
        // Load other image formats using stb_image
        m_LocalBuffer = stbi_load(path.c_str(), &m_Width, &m_Height, &m_BPP, 4);

        GLCall(glGenTextures(1, &m_RendererID));
        GLCall(glBindTexture(GL_TEXTURE_2D, m_RendererID));

        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
        GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

        GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_LocalBuffer));
        GLCall(glBindTexture(GL_TEXTURE_2D, 0));

        if (m_LocalBuffer) {
            stbi_image_free(m_LocalBuffer);
        }
    }
}
