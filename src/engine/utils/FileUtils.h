#pragma once

#include <vector>
#include <fstream>
#include <ILog.h>
#include "StbImage.h"

namespace Run {
    struct Image {
        unsigned char* data;
        int width, height, ch;

        void destroy() {
            stbi_image_free(data);
        }
    };

    typedef Image Cursor;
    typedef Image Icon;

    namespace FileUtils {
        // linker complaines saying Run::FileUtils::read is already defined :(
        // thats why its wrapped in an extra namespace
        namespace {
            bool justStarted = true;
            std::vector<char> read(const std::string& fileName, int fileStreamFlags = 0) {
                // ate = at end
                std::ifstream fs{fileName, std::ios::ate | fileStreamFlags};

                if (!fs.is_open()) {
                    I_LOG_WARNING("Failed to open file: %s", fileName.c_str());
                    if (justStarted) {
                        I_FILE_LOG("Run.log", "Failed to open file: %s", "w", fileName.c_str());
                        justStarted = false;
                    } else 
                        I_FILE_LOG("Run.log", "Failed to open file: %s", "a", fileName.c_str());
                }

                size_t fileSize = fs.tellg();
                std::vector<char> buffer(fileSize);

                // Seek the start of the file
                fs.seekg(0);

                fs.read(buffer.data(), fileSize);

                fs.close();

                return buffer;
            }

            std::string strRead(const std::string& fileName, int fileStreamFlags) {
                std::vector<char> data = read(fileName, fileStreamFlags);

                std::string out;
                out.append(data.data());

                return out;
            }


            const char* cstrRead(const std::string& fileName, int fileStreamFlags) {
                std::vector<char> data = read(fileName, fileStreamFlags);

                const char* out = data.data(); 

                return out;
            }


            Image loadImage(const std::string& fileName) {
                Image img{};

                img.data = stbi_load(fileName.c_str(), &img.width, &img.height, &img.ch, NULL);

                I_ASSERT_ERROR(!img.data, "Image %s failed to load!", fileName.c_str());

                return img;
            }

            Cursor loadCursor(const std::string& fileName) {
                Cursor cursor = loadImage(fileName);

                I_ASSERT_ERROR(cursor.ch != 4, "Cursor image %s does not have 4 colour channels. Use .png images with transparency on!", fileName.c_str());

                return cursor;
            }

            Icon loadIcon(const std::string& fileName) {
                Icon icon = loadImage(fileName);

                I_ASSERT_ERROR(icon.ch != 4, "icon image %s does not have 4 colour channels. Use .png images with transparency on!", fileName.c_str());

                return icon;
            }
        }
    }
}