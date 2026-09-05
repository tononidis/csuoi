#include "TexturesInfo.hpp"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#ifndef ASSET_DIR
static_assert(false, "ASSET_DIR was not defined!")
#endif

#include <spdlog/spdlog.h>

#include <mutex>
#include <array>

namespace BraXaPsaIII {

namespace TexturesInfo {

struct TexturePair {
    const char *textureImgFile;
    GLuint &textureId;

    TexturePair(const char *textureImgFile_in, GLuint &textureId_in)
        : textureImgFile(textureImgFile_in), textureId(textureId_in)
    {
    }
};

void loadTextures()
{
    static std::once_flag loadOnceFlag;
    std::call_once(loadOnceFlag, []() {
        constexpr char rockAsset[]    = ASSET_DIR "/rock.bmp";
        constexpr char paperAsset[]   = ASSET_DIR "/paper.bmp";
        constexpr char scissorAsset[] = ASSET_DIR "/scissor.bmp";
        constexpr char bombAsset[] = ASSET_DIR "/bomb.bmp";
        const std::array texturePairsArr{TexturePair{rockAsset, rock}, TexturePair{paperAsset, paper}, TexturePair{scissorAsset, scissor}, TexturePair{bombAsset, bomb}};

        for (const auto &texturePair : texturePairsArr) {
            SPDLOG_DEBUG("Loading texture from {} ...", texturePair.textureImgFile);

            glGenTextures(1, &texturePair.textureId);
            glBindTexture(GL_TEXTURE_2D, texturePair.textureId);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            int width, height;
            auto data = stbi_load(texturePair.textureImgFile, &width, &height, nullptr, STBI_rgb);
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
} // namespace TexturesInfo

} // namespace BraXaPsaIII
