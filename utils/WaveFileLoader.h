//
// Created by maxime on 02/09/23.
//

#ifndef EMU_INTEL8080_WAVEFILELOADER_H
#define EMU_INTEL8080_WAVEFILELOADER_H

#include <vector>
#include <cstdint>
#include <fstream>

/*
 * This utility class is intended to work only with :
 * - unsigned 8-bit PCM mono WAV file
 *
 * The WAV file specifications can be found here : https://www.mmsp.ece.mcgill.ca/Documents/AudioFormats/WAVE/WAVE.html
 * These informations have been used to make this WAV file reader.
 */
class WaveFileLoader {
public:
    struct WAVFormat {
        uint16_t channels_number;
        uint32_t sample_rate;
        uint16_t bits_per_sample;
    };

    static std::vector<uint8_t> getPCMData(const char *filename);
    static WAVFormat getFormat(const char *filename);

private:
    static std::vector<uint8_t> getFileData(const char *filename);
    static long getChunkStartIndex(std::string chunk, std::vector<uint8_t> fileData);
};


#endif //EMU_INTEL8080_WAVEFILELOADER_H
