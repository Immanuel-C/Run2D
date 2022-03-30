#pragma once

#include <vector>
#include <fstream>
#include <ILog.h>

namespace Run {
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
        }
    }
}