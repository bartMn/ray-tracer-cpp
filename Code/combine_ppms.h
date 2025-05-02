#include <iostream>
#include <fstream>
#include <vector>
#include <string>

/**
 * @brief Combines multiple PPM images vertically into a single PPM image.
 * 
 * @param imageNames A vector of file paths to the PPM images to combine.
 * @param outputFileName The file path for the combined output PPM image.
 */
void combineImagesVertically(const std::vector<std::string>& imageNames, const std::string& outputFileName);