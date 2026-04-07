#include "TexturesInfo.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#include <spdlog/spdlog.h>

#include <mutex>

namespace BraXaPsaIII {

namespace TexturesInfo {

    struct TexturePair {
        const char* textureImgFile;
        GLuint& textureId;

        TexturePair(const char* textureImgFile_in, GLuint& textureId_in)
            : textureImgFile(textureImgFile_in)
            , textureId(textureId_in)
        {
        }
    };

    void loadTextures()
    {
        static std::once_flag loadOnceFlag;
        std::call_once(loadOnceFlag, []() {
            const std::array texturePairsArr { TexturePair { "images/rock.bmp", rock }, TexturePair { "images/paper.bmp", paper }, TexturePair { "images/scissor.bmp", scissor } };

            for (const auto& texturePair : texturePairsArr) {
                SPDLOG_DEBUG("Loading texture from {} ...", texturePair.textureImgFile);

                glGenTextures(1, &texturePair.textureId);
                glBindTexture(GL_TEXTURE_2D, texturePair.textureId);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                int width, height;
                auto data = stbi_load(texturePair.textureImgFile, &width, &height, nullptr, 0);
                if (data != nullptr) {
                    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                }

                else {
                    SPDLOG_CRITICAL("Failed to load texture!");
                    std::exit(EXIT_FAILURE);
                }

                stbi_image_free(data);
            }
        });
    }
}

}
