#ifndef tf2_utils_csv_h_
#define tf2_utils_csv_h_

#include "../includes.h"

namespace tf2 {

  /**
   * @brief Utility functions for handling CSV files.
   */
  namespace csv {

    /**
     * @brief Read data from a CSV file into a 2D vector.
     *
     * This function reads data from a CSV file and returns it as a 2D vector.
     *
     * @tparam T The type of data to read (e.g., int, double).
     * @param filename The path to the CSV file.
     * @param row_lim Limits for rows to read (default: {0, std::numeric_limits<int>::max()}).
     * @param col_lim Limits for columns to read (default: {0, std::numeric_limits<int>::max()}).
     * @return A 2D vector containing the read data.
     */
    template <typename T>
    std::vector<std::vector<T>> read(
      const std::string filename,
      const std::vector<int> row_lim = {0,std::numeric_limits<int>::max()},
      const std::vector<int> col_lim = {0,std::numeric_limits<int>::max()}
    ) {
      std::ifstream file(filename);
      std::vector<std::vector<T>> array;
      if (file) {
        int row = 0;
        std::string line;
        while (std::getline(file, line)) {
          if (row_lim[0] <= row && row < row_lim[1]) {
            std::stringstream sep(line);
            std::string field;
            array.push_back(std::vector<T>());
            int col = 0;
            while (std::getline(sep, field, ',')) {
              if (col_lim[0] <= col && col < col_lim[1]) {
                array.back().push_back(std::stod(field));
              }
              col++;
            }
          }
          row++;
        }
      }
      return array;
    }

    /**
     * @brief Write data to a CSV file from a vector.
     *
     * This function writes data to a CSV file from a vector,
     * considering the specified shape, row-major order, and precision.
     *
     * @tparam T The type of data to write (e.g., int, double).
     * @param filename The path to the CSV file.
     * @param vec The vector containing the data.
     * @param shape The shape of the data.
     * @param rowmajor Flag indicating if data is in row-major order (default: true).
     * @param precision The precision for writing floating-point numbers (default: 6).
     */
    template <typename T>
    void write(
      const std::string filename,
      const std::vector<T>& vec,
      const std::vector<int> shape,
      const bool rowmajor = true,
      const int precision = 6
    ) {
      std::ofstream file(filename);
      for (int i = 0; i < shape[0]; ++i) {
        for (int j = 0; j < shape[1]; ++j) {
          auto v = (rowmajor == true) ? vec[i*shape[1]+j] : vec[j*shape[0]+i];
          file << std::setprecision(precision) << v;
          if (j != shape[1]-1) {
            file << ",";
          }
        }
        file << "\n";
      }
      file.close();
    }

    /**
     * @brief Write data to a CSV file from a 2D vector.
     *
     * This function writes data to a CSV file from a 2D vector,
     * considering the specified precision.
     *
     * @tparam T The type of data to write (e.g., int, double).
     * @param filename The path to the CSV file.
     * @param vec The 2D vector containing the data.
     * @param precision The precision for writing floating-point numbers (default: 6).
     */
    template <typename T>
    void write(
      const std::string filename,
      const std::vector<std::vector<T>>& vec,
      const int precision = 6
    ) {
      std::ofstream file(filename);
      for (auto i = 0; i < vec.size(); i++) {
        for (auto j = 0; j < vec[i].size(); j++) {
          file << std::setprecision(precision) << vec[i][j];
          if (j != vec[i].size()-1) {
            file << ",";
          }
        }
        file << "\n";
      }
      file.close();
    }

  } // namespace csv

} // namespace tf2

#endif // tf2_utils_csv_h_
