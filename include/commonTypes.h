#pragma once

/**
 * @brief Represents a 2D point with coordinates of type T.
 *
 * @tparam T The numeric type of the coordinates (e.g., int, float, double).
 */
template <typename T> struct Point {
  T x;
  T y;
};

/**
 * @brief Represents a 2D vector defined by its start and end coordinates.
 *
 * @tparam T The numeric type of the coordinates (e.g., int, float, double).
 */
template <typename T> struct Vector {
  T x1;
  T y1;
  T x2;
  T y2;

  /**
   * @brief Equality operator to compare two vectors.
   *
   * @param other The vector to compare with.
   * @return true if all coordinates of both vectors are equal.
   * @return false otherwise.
   */
  bool operator==(const Vector<T> &other) const {
    return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
  }
};
