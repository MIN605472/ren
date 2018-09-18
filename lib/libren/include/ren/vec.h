// Generic implementation of a vector class and some operations on them like the
// dot product, the cross product, etc.

#ifndef REN_VEC3_H_
#define REN_VEC3_H_

#include <algorithm>
#include <cassert>
#include <cmath>
#include <initializer_list>
#include <iostream>

namespace ren {

template <typename T, int N>
struct VecData {
  T data[N];
};

template <typename T>
struct VecData<T, 2> {
  union {
    T data[2];
    struct {
      T x;
      T y;
    };
  };
};

template <typename T>
struct VecData<T, 3> {
  union {
    T data[3];
    struct {
      T x;
      T y;
      T z;
    };
  };
};

template <typename T>
struct VecData<T, 4> {
  union {
    T data[4];
    struct {
      T x;
      T y;
      T z;
      T w;
    };
  };
};

template <typename T, int N>
struct VecFun : public VecData<T, N> {
  VecFun() {
    for (int i = 0; i < N; ++i) {
      this->data[i] = 0;
    }
  }

  VecFun(T v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] = v;
    }
  }

  VecFun(std::initializer_list<T> list) {
    if (list.size() != N) {
      throw "Too many or too little arguments";
    }
    int i = 0;
    for (auto &e : list) {
      this->data[i++] = e;
    }
  }

  template <int N1 = N, typename = typename std::enable_if<N1 == 2>::type>
  VecFun(T x, T y) {
    this->data[0] = x;
    this->data[1] = y;
  }

  template <int N1 = N, typename = typename std::enable_if<N1 == 3>::type>
  VecFun(const VecFun<T, 4> &v) {
    this->data[0] = v[0];
    this->data[1] = v[1];
    this->data[2] = v[2];
  }

  template <int N1 = N, typename = typename std::enable_if<N1 == 3>::type>
  VecFun(T x, T y, T z) {
    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = z;
  }

  template <int N1 = N, typename = typename std::enable_if<N1 == 4>::type>
  VecFun(T x, T y, T z, T w) {
    this->data[0] = x;
    this->data[1] = y;
    this->data[2] = z;
    this->data[3] = w;
  }

  template <int N1 = N, typename = typename std::enable_if<N1 == 4>::type>
  VecFun(const VecFun<T, 3> &v, T w) {
    this->data[0] = v[0];
    this->data[1] = v[1];
    this->data[2] = v[2];
    this->data[3] = w;
  }
  const T &operator[](int i) const { return this->data[i]; }

  T &operator[](int i) { return this->data[i]; }

  VecFun<T, N> &operator+=(T v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] += v;
    }
    return *this;
  }

  VecFun<T, N> &operator-=(T v) { return operator+=(-v); }

  VecFun<T, N> &operator*=(T v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] *= v;
    }
    return *this;
  }

  VecFun<T, N> &operator/=(T v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] /= v;
    }
    return *this;
  }

  VecFun<T, N> &operator+=(const VecFun<T, N> &v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] += v.data[i];
    }
    return *this;
  }

  VecFun<T, N> &operator-=(const VecFun<T, N> &v) { return operator+=(-v); }

  VecFun<T, N> &operator*=(const VecFun<T, N> &v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] *= v.data[i];
    }
    return *this;
  }

  VecFun<T, N> &operator/=(const VecFun<T, N> &v) {
    for (int i = 0; i < N; ++i) {
      this->data[i] /= v.data[i];
    }
    return *this;
  }
};

template <typename T, int N>
VecFun<T, N> operator-(const VecFun<T, N> &v) {
  VecFun<T, N> c(v);
  c *= -1;
  return c;
}

template <typename S, typename T, int N>
VecFun<T, N> operator+(const VecFun<T, N> &vector, S value) {
  VecFun<T, N> c(vector);
  c += value;
  return c;
}

template <typename S, typename T, int N>
VecFun<T, N> operator+(S value, const VecFun<T, N> &vector) {
  return operator+(vector, value);
}

template <typename S, typename T, int N>
VecFun<T, N> operator-(const VecFun<T, N> &vector, S value) {
  VecFun<T, N> c(vector);
  c -= value;
  return c;
}

template <typename S, typename T, int N>
VecFun<T, N> operator-(S value, const VecFun<T, N> &vector) {
  return operator-(vector, value);
}

template <typename S, typename T, int N>
VecFun<T, N> operator*(const VecFun<T, N> &vector, S value) {
  VecFun<T, N> c(vector);
  c *= value;
  return c;
}

template <typename S, typename T, int N>
VecFun<T, N> operator*(S value, const VecFun<T, N> &vector) {
  return operator*(vector, value);
}

template <typename S, typename T, int N>
VecFun<T, N> operator/(const VecFun<T, N> &vector, S value) {
  VecFun<T, N> c(vector);
  c /= value;
  return c;
}

template <typename S, typename T, int N>
VecFun<T, N> operator/(S value, const VecFun<T, N> &vector) {
  return operator/(vector, value);
}

template <typename T, int N>
VecFun<T, N> operator+(const VecFun<T, N> &lhs, const VecFun<T, N> &rhs) {
  VecFun<T, N> c(lhs);
  c += rhs;
  return c;
}

template <typename T, int N>
VecFun<T, N> operator-(const VecFun<T, N> &lhs, const VecFun<T, N> &rhs) {
  VecFun<T, N> c(lhs);
  c -= rhs;
  return c;
}

template <typename T, int N>
VecFun<T, N> operator*(const VecFun<T, N> &lhs, const VecFun<T, N> &rhs) {
  VecFun<T, N> c(lhs);
  c *= rhs;
  return c;
}

template <typename T, int N>
VecFun<T, N> operator/(const VecFun<T, N> &lhs, const VecFun<T, N> &rhs) {
  VecFun<T, N> c(lhs);
  c /= rhs;
  return c;
}

template <typename T, int N>
std::ostream &operator<<(std::ostream &os, const VecFun<T, N> &v) {
  os << "[";
  for (int i = 0; i < N - 1; ++i) {
    os << v.data[i] << ",";
  }
  os << v.data[N - 1];
  os << "]";
  return os;
}

template <typename T, int N>
inline T MaxComp(const VecFun<T, N> &v) {
  return *std::max_element(&v.data[0], &v.data[N]);
}

template <typename T, int N>
inline T MinComp(const VecFun<T, N> &v) {
  return *std::min_element(&v.data[0], &v.data[N]);
}

template <typename T, int N>
inline T Avg(const VecFun<T, N> &v) {
  auto total = T(0);
  for (int i = 0; i < N; ++i) {
    total += v.data[i];
  }
  return total / N;
}

template <typename T, int N>
inline VecFun<T, N> Clamp(const VecFun<T, N> &v, T min, T max) {
  VecFun<T, N> res;
  for (int i = 0; i < N; ++i) {
    if (v[i] < min) {
      res[i] = min;
    } else if (v[i] > max) {
      res[i] = max;
    } else {
      res[i] = v[i];
    }
  }
  return res;
}

template <typename T, int N>
inline T Length(const VecFun<T, N> &v) {
  return std::sqrt(Dot(v, v));
}

template <typename T, int N>
inline T Length2(const VecFun<T, N> &v) {
  return Dot(v, v);
}

template <typename T, int N>
inline VecFun<T, N> Normalize(const VecFun<T, N> &v) {
  return v / Length(v);
}

template <typename T, int N>
inline T Dot(const VecFun<T, N> &a, const VecFun<T, N> &b) {
  T acc = 0;
  for (int i = 0; i < N; ++i) {
    acc += a.data[i] * b.data[i];
  }
  return acc;
}

template <typename T>
inline VecFun<T, 3> Cross(const VecFun<T, 3> &a, const VecFun<T, 3> &b) {
  return VecFun<T, 3>(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z,
                      a.x * b.y - a.y * b.x);
}

template <typename T>
VecFun<T, 3> NormalTo(const VecFun<T, 3> &v) {
  if (v.x != 0) {
    return VecFun<T, 3>(-(v.y + v.z) / v.x, 1, 1);
  }
  if (v.y != 0) {
    return VecFun<T, 3>(1, -(v.x + v.z) / v.y, 1);
  }
  if (v.z != 0) {
    return VecFun<T, 3>(1, 1, -(v.x + v.y) / v.z);
  }
  throw "Zero vector";
}

template <typename T>
VecFun<T, 3> ReflectAbout(const VecFun<T, 3> &v, const VecFun<T, 3> &n) {
  return 2 * Dot(v, n) * n - v;
}

template <typename T, int N>
bool IsZero(const VecFun<T, N> &v) {
  for (int i = 0; i < N; ++i) {
    if (v.data[i] != 0) {
      return false;
    }
  }
  return true;
}

}  // namespace ren
#endif  // REN_VEC3_H_
