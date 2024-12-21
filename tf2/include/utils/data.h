#ifndef tf2_utils_data_h_
#define tf2_utils_data_h_

#include "../includes.h"
#include <cppflow/cppflow.h>

namespace tf2 {

  /**
   * @brief Namespace containing custom data structures for TF2.
   *
   * The data namespace defines custom data structures used in
   * the TF2 library. Two primary structures are defined:
   *
   * 1. list_tuple_tensor: A vector of tuples, where each tuple consists
   *    of a string identifier and a corresponding TensorFlow tensor.
   *
   * 2. list_tuple_vector: A vector of tuples, where each tuple consists
   *    of a string identifier and a vector of a specified type T.
   *
   * These structures are designed to facilitate the manipulation
   * and organization of data within the TF2 library.
   */
  namespace data {

    /**
     * @brief Represents a vector of tuples, each containing a string
     * identifier and a corresponding TensorFlow tensor.
     */
    using list_tuple_tensor = std::vector<std::tuple<std::string, cppflow::tensor>>;

    /**
     * @brief Represents a vector of tuples, each containing a string
     * identifier and a vector of a specified type T.
     *
     * This structure is often used for storing input/output data
     * associated with TF2 operations.
     *
     * @tparam T The type of data stored in the vector.
     */
    template <typename T>
    using list_tuple_vector = std::vector<std::tuple<std::string, std::vector<T>>>;

    /**
     * @brief Alias for a 1D vector of type T.
     *
     * The vector_3d is used to represent a 3D vector
     * containing elements of type T.
     *
     * @tparam T The type of elements in the vector.
     */
    template <typename T>
    using vector_1d = std::vector<T>;

    /**
     * @brief Alias for a 2D vector of type T.
     *
     * The vector_3d is used to represent a 3D vector
     * containing elements of type T.
     *
     * @tparam T The type of elements in the vector.
     */
    template <typename T>
    using vector_2d = std::vector<std::vector<T>>;

    /**
     * @brief Alias for a 3D vector of type T.
     *
     * The vector_3d is used to represent a 3D vector
     * containing elements of type T.
     *
     * @tparam T The type of elements in the vector.
     */
    template <typename T>
    using vector_3d = std::vector<std::vector<std::vector<T>>>;

  } // namespace data

} // namespace tf2

#endif // tf2_utils_data_h_
