#pragma once

/*----------------------------------------------------------------------
                                Methods
*----------------------------------------------------------------------*/

/**
 * @brief Write data to local file
 * @param arg - pointer to raw data
 * @param path - path to local file
 */
void write_data_to_file(char *arg, const std::string path);

/**
 * @brief Read data from local file
 * @param path - path to local file
 */
std::string get_data_from_file(const std::string path);