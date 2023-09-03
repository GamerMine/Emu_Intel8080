//
// Created by maxime on 02/09/23.
//

#ifndef EMU_INTEL8080_WAVEFILELOADER_H
#define EMU_INTEL8080_WAVEFILELOADER_H

#include <vector>
#include <cstdint>
#include <fstream>
#include <AL/al.h>

/*
 * This utility class is intended to work only with :
 * - unsigned 8-bit PCM mono WAV file
 *
 * The WAV file specifications can be found here : https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
 * These informations have been used to make this WAV file reader.
 */
class WaveFileLoader {
public:
    struct WAVFile {
        uint16_t channels_number;
        uint32_t sample_rate;
        uint16_t bits_per_sample;
        std::vector<uint8_t> data;
    };

    static WAVFile getWaveFileData(const char *filename);
    static ALuint  getBufferForFile(const char *filename);

private:
    static std::vector<uint8_t> getFileData(const char *filename);
    static long getChunkStartIndex(std::string chunk, std::vector<uint8_t> fileData);
};


#endif //EMU_INTEL8080_WAVEFILELOADER_H
