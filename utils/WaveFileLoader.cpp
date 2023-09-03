//
// Created by maxime on 02/09/23.
//

#include "WaveFileLoader.h"

std::vector<uint8_t> WaveFileLoader::getFileData(const char *filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.good()) {
        printf("File does not exist or is corrupted");
    }

    std::vector<uint8_t> fileData;
    size_t length;
    file.unsetf (std::ios::skipws);
    file.seekg(0, std::ios::end);
    length = file.tellg();
    file.seekg(0, std::ios::beg);

    fileData.resize(length);
    file.read(reinterpret_cast<char *>(fileData.data()), length);

    file.close();

    return fileData;
}

std::vector<uint8_t> WaveFileLoader::getPCMData(const char *filename) {
    std::vector<uint8_t> fileData = getFileData(filename);
    long dataStartIndex = getChunkStartIndex("data", fileData);
    long pcmDataStartIndex = dataStartIndex + 4;
    int32_t nbSamples = fileData[dataStartIndex+3] << 24 | fileData[dataStartIndex+2] << 16 | fileData[dataStartIndex+1] << 8 | fileData[dataStartIndex];

    std::vector<uint8_t> pcmData;
    pcmData.clear();

    for (int32_t i = 0; i < nbSamples; i++) {
        pcmData.push_back(fileData[pcmDataStartIndex+i]);
    }

    return pcmData;
}

WaveFileLoader::WAVFormat WaveFileLoader::getFormat(const char *filename) {
    WAVFormat format {};

    std::vector<uint8_t> fileData = getFileData(filename);

    format.channels_number  = fileData[23] << 8 | fileData[22];
    format.sample_rate      = fileData[27] << 24 | fileData[26] << 16 | fileData[25] << 8 | fileData[24];
    format.bits_per_sample  = fileData[35] << 8 | fileData[34];

    return format;
}

long WaveFileLoader::getChunkStartIndex(std::string chunk, std::vector<uint8_t> fileData) {
    for (long j = 0; j < fileData.size(); j++) {
        if (fileData[j] == chunk[0]) {
            for (uint8_t i = 1; chunk.length(); i++) {
                if (fileData[j+i] != chunk[i]) {
                    break;
                }
                return j + chunk.length();
            }
        }
    }

    return -1;
}
