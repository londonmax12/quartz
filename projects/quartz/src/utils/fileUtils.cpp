#include "utils/fileUtils.hpp"

const char* Quartz::loadFileToCString(const char* filename)
{
    std::ifstream file(filename, std::ios::in | std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Failed to open the file: " << std::strerror(errno) << std::endl;
        return nullptr;
    }

    file.seekg(0, std::ios::end);
    std::streampos file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    char* buffer = new char[static_cast<size_t>(file_size) + 1];

    file.read(buffer, file_size);
    buffer[file_size] = '\0';

    file.close();

    return buffer;
}
