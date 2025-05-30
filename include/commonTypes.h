#pragma once

template <typename T>
struct Point {
  T x;
  T y;
};

template <typename T>
struct Vector {
  T x1;
  T y1;
  T x2;
  T y2;

  bool operator==(const Vector<T> &other) const {
    return x1 == other.x1 && y1 == other.y1 && x2 == other.x2 && y2 == other.y2;
  }
};
