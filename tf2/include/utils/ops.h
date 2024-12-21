#ifndef tf2_utils_ops_h_
#define tf2_utils_ops_h_

#include "../includes.h"
#include "data.h"

namespace tf2 {

  /**
   * @brief Utility functions for vector operations.
   */
  namespace ops {

    // Print
    /* ============================= */

    /**
     * @brief Print a 2D array.
     *
     * This function prints the elements of a 2D array to the standard output.
     *
     * @tparam T The type of the array elements.
     * @param array The 2D array to be printed.
     */
    template <typename T>
    void print(
      const std::vector<std::vector<T>>& array
    ) {
      for (const std::vector<T>& vector : array) {
        for (const T& value : vector) {
          std::cout << value << ' ';
        }
        std::cout << std::endl;
      }
    }

    /**
     * @brief Print a 1D array.
     *
     * This function prints the elements of a 1D array to the standard output.
     *
     * @tparam T The type of the array elements.
     * @param vector The 1D array to be printed.
     */
    template <typename T>
    void print(
      const std::vector<T>& vector
    ) {
      for (const T& value : vector) {
        std::cout << value << std::endl;
      }
    }

    // Transpose
    /* ============================= */

    /**
     * @brief Transpose a 3D array according to the specified order.
     *
     * This function transposes a 3D array based on the
     * provided order of dimensions. The resulting transposed
     * array has the same dimensions as the original array.
     *
     * @tparam T The type of the array elements.
     * @param array The 3D array to be transposed.
     * @param order The order of dimensions for transposing (default: {0, 2, 1}).
     * @return A transposed 3D array.
     *
     * @note The default order {0, 2, 1} corresponds to transposing the
     *       second and third dimensions. For example, if the input array
     *       has dimensions (A, B, C), the resulting transposed array
     *       will have dimensions (A, C, B).
     *
     * @warning If the input array is empty, a corresponding empty
     *          array will be returned.
     */
    template <typename T>
    tf2::data::vector_3d<T> transpose(
      const tf2::data::vector_3d<T>& array,
      const tf2::data::vector_1d<T>& order = {0,2,1}
    ) {
      // Handle empty input array
      if (array.empty() || array[0].empty() || array[0][0].empty()) {
        return tf2::data::vector_3d<T>();
      }
      // Get dimensions
      const std::size_t dim1 = array.size();
      const std::size_t dim2 = array[0].size();
      const std::size_t dim3 = array[0][0].size();
      // Create a transposed array with the same dimensions
      tf2::data::vector_3d<T> transposed(
        dim1, tf2::data::vector_2d<T>(
          dim2, tf2::data::vector_1d<T>(
            dim3
          )
        )
      );
      // Transpose the original array
      for (std::size_t i = 0; i < dim1; ++i) {
        for (std::size_t j = 0; j < dim2; ++j) {
          for (std::size_t k = 0; k < dim3; ++k) {
            auto ti = order[0] == 0 ? i : (order[0] == 1 ? j : k);
            auto tj = order[1] == 0 ? i : (order[1] == 1 ? j : k);
            auto tk = order[2] == 0 ? i : (order[2] == 1 ? j : k);
            transposed[ti][tj][tk] = array[i][j][k];
          }
        }
      }
      return transposed;
    }

    /**
     * @brief Transpose a 2D array.
     *
     * This function transposes a 2D array by converting
     * it to a temporary 3D array, transposing the 3D array,
     * and then converting it back to a 2D array.
     *
     * @tparam T The type of the array elements.
     * @param array The 2D array to be transposed.
     * @return A transposed 2D array.
     *
     * @note This function internally converts the 2D array
     *       to a 3D array, transposes it, and then converts
     *       it back to a 2D array. The resulting transposed
     *       2D array has the same dimensions as the original array.
     *
     * @warning If the input array is empty, a corresponding empty
     *          array will be returned.
     */
    template <typename T>
    tf2::data::vector_2d<T> transpose(
      const tf2::data::vector_2d<T>& array
    ) {
      // Handle empty input array
      if (array.empty() || array[0].empty()) {
        return tf2::data::vector_2d<T>();
      }
      // Convert the 2D array to a 3D array
      tf2::data::vector_3d<T> array_3d;
      array_3d.push_back(array);
      // Transpose the original array
      auto transposed_3d = transpose<T>(array_3d);
      // Convert the 3D array to a 2D array
      return transposed_3d[0];
    }

    /**
     * @brief Transpose a flattened 3D array.
     *
     * This function transposes a 3D array represented by a 1D 
     * vector, where the array has dimensions dim1 x dim2 x dim3.
     * The transposed array is returned as a 1D vector.
     *
     * @tparam T The type of elements in the array.
     * @param array The input 1D vector representing the 3D array.
     * @param dim1 The size of the first dimension.
     * @param dim2 The size of the second dimension.
     * @param dim3 The size of the third dimension (default is 1).
     * @return A 1D vector representing the transposed array.
     * @thdim1 std::invalid_argument If the dimensions are invalid or the
     *         input array size doesn't match the specified dimensions.
     */
    template <typename T>
    std::vector<T> transpose(
      const std::vector<T>& array,
      const std::int32_t dim1,
      const std::int32_t dim2,
      const std::int32_t dim3 = 1
    ) {
      // Check if the dimensions are valid
      if (dim1 <= 0 || dim2 <= 0 || dim3 <= 0) {
        std::ostringstream message;
        message << "\nFrom tf2::ops::transpose():"
                << "\n> Invalid dimensions.";
        throw std::invalid_argument(message.str());
      }
      // Ensure the size of the input array matches 
      // the size defined by the dimensions
      auto size = dim1 * dim2 * dim3;
      if (array.size() != size) {
        std::ostringstream message;
        message << "\nFrom tf2::ops::transpose():"
                << "\n> Input array size doesn't match specified dimensions.";
        throw std::invalid_argument(message.str());
      }
      // Perform transposition
      std::vector<T> transposed(size);
      for (std::int32_t i = 0; i < dim1; ++i) {
        for (std::int32_t j = 0; j < dim2; ++j) {
          for (std::int32_t k = 0; k < dim3; ++k) {
            auto ijk = i * dim2 * dim3 + j * dim3 + k;
            auto kji = k * dim1 * dim2 + j * dim1 + i;
            transposed[kji] = array[ijk];
          }
        }
      }
      return transposed;
    }

    // Flatten
    /* ============================= */

    /**
     * @brief Flatten a 2D array.
     *
     * This function flattens a given 2D array, optionally
     * transposing it during flattening.
     *
     * @tparam T The type of the array elements.
     * @param array The 2D array to be flattened.
     * @param transpose_in Flag indicating whether to transpose
     *                     the array during flattening.
     * @return The flattened 2D array.
     */
    template <typename T>
    std::vector<T> flatten(
      const std::vector<std::vector<T>>& array,
      const bool transpose_in = false
    ) {
      // Handle empty input array
      if (array.empty() || array[0].empty()) {
        return std::vector<T>();
      }
      // Get dimensions
      const std::size_t dim1 = array.size();
      const std::size_t dim2 = array[0].size();
      // Perform flattening
      std::vector<T> flattened(dim1 * dim2);
      for (std::size_t i = 0; i < dim1; ++i) {
        for (std::size_t j = 0; j < dim2; ++j) {
          if (transpose_in) {
            flattened[j * dim1 + i] = array[i][j];
          } else {
            flattened[i * dim2 + j] = array[i][j];
          }
        }
      }
      return flattened;
    }

    // Reshape
    /* ============================= */

    /**
     * @brief Reshape a 1D array into a 2D array.
     *
     * This function reshapes a given 1D array into
     * a 2D array with specified dimensions.
     *
     * @tparam T The type of the array elements.
     * @param vector The 1D array to be reshaped.
     * @param shape The target shape of the 2D array (2-element vector).
     * @param transpose_out Flag indicating whether to transpose
     *                      the array after reshaping.
     * @return The reshaped 2D array.
     */
    template <typename T>
    std::vector<std::vector<T>> reshape(
      const std::vector<T>& vector,
      const std::vector<std::int32_t> shape,
      const bool transpose_out = false
    ) {
      // Error message
      std::ostringstream message;
      message << "From tf2::ops::reshape(): ";
      // Ensure shape is of size 2
      if (shape.size() != 2) {
        message << "\n> Shape argument must be a 2-element vector.";
        throw std::invalid_argument(message.str());
      }
      // Get dimensions
      const std::int32_t dim1 = shape[0];
      const std::int32_t dim2 = shape[1];
      // Ensure the size of the input vector
      // matches the size defined by the shape
      if (dim1 * dim2 != vector.size()) {
        message << "\n> Input array size doesn't match specified dimensions.";
        throw std::invalid_argument(message.str());
      }
      // Perform reshaping
      std::vector<std::vector<T>> array(dim1, std::vector<T>(dim2));
      for (std::int32_t i = 0; i < dim1; ++i) {
        for (std::int32_t j = 0; j < dim2; ++j) {
          if (transpose_out) {
            array[i][j] = vector[j * dim1 + i];
          } else {
            array[i][j] = vector[i * dim2 + j];
          }
        }
      }
      return array;
    }

    // Major ordering
    /* ============================= */

    /**
     * @brief Invert the major order of a flattened 2D array.
     *
     * This function inverts the major order (row-major to
     * column-major or vice versa) of a given flattened 2D
     * array based on specified dimensions and order preference.
     *
     * @tparam T The type of the array elements.
     * @param x The flattened 2D array to be inverted.
     * @param dim1 The size of the first dimension.
     * @param dim2 The size of the second dimension.
     * @param col2row Flag indicating the order preference
     *                (true for column-major to row-major).
     */
    template <typename T>
    void invert_major(
      std::vector<T>& x,
      const std::int32_t dim1,
      const std::int32_t dim2,
      const bool col2row
    ) {
      if ((dim2 > 1 && col2row) || (dim1 > 1 && !col2row)) {
        // Transpose the matrix to convert
        // between row-major and column-major
        x = std::move(transpose<T>(x, dim1, dim2));
      }
    }

  } // namespace ops

} // namespace tf2

#endif // tf2_utils_ops_h_
