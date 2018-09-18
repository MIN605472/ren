// Generic implementation of a square matrix class and some operations on them
// like inverse calculation, multiplication, division, etc..
// Matrix inverse, LUP decomposition and solve algorithm are from "Introduction
// to Algorithms (3rd Edition) by Cormen, Leiserson, Rivest and Stein"

#ifndef REN_MAT4_H_
#define REN_MAT4_H_

#include <algorithm>
#include <cmath>
#include "ren/vec.h"

namespace ren {
template <typename T, int N>
class MatBase {
 public:
  MatBase() {
    for (int i = 0; i < N; ++i) {
      VecFun<T, N> vector;
      vector[i] = 1;
      column_[i] = vector;
    }
  }

  MatBase(T v) {
    for (int i = 0; i < N; ++i) {
      VecFun<T, N> vector;
      vector[i] = v;
      column_[i] = vector;
    }
  }

  template <typename = typename std::enable_if<N == 4>::type>
  MatBase(const VecFun<T, N> &v0, const VecFun<T, N> &v1,
          const VecFun<T, N> &v2, const VecFun<T, N> &v3) {
    column_[0] = v0;
    column_[1] = v1;
    column_[2] = v2;
    column_[3] = v3;
  }

  MatBase<T, N> operator*(const MatBase<T, N> &rhs) const {
    MatBase<T, N> res(0);
    for (int i = 0; i < N; ++i) {
      for (int j = 0; j < N; ++j) {
        res[i] += column_[j] * rhs.column_[i][j];
      }
    }
    return res;
  }

  MatBase<T, N> &operator*=(const MatBase<T, N> &rhs) {
    return (*this = *this * rhs);
  }

  VecFun<T, N> operator*(const VecFun<T, N> &rhs) const {
    VecFun<T, N> res;
    for (int j = 0; j < N; ++j) {
      res += column_[j] * rhs[j];
    }
    return res;
  }

  const VecFun<T, N> &operator[](int i) const { return column_[i]; }

  VecFun<T, N> &operator[](int i) {
    return const_cast<VecFun<T, N> &>(
        static_cast<const MatBase<T, N> &>(*this)[i]);
  }

 private:
  VecFun<T, N> column_[N];
};

template <typename T, int N>
void LupDecomposition(MatBase<T, N> a, MatBase<T, N> &lower,
                      MatBase<T, N> &upper, VecFun<int, N> &permutations) {
  for (int i = 0; i < N; ++i) {
    permutations[i] = i;
  }
  int kprime = 0;
  for (int k = 0; k < N; ++k) {
    T p = 0;
    for (int i = k; i < N; ++i) {
      T aik = std::abs(a[k][i]);
      if (aik > p) {
        p = aik;
        kprime = i;
      }
    }
    if (p == 0) {
      throw "Singular matrix";
    }
    std::swap(permutations[k], permutations[kprime]);
    for (int i = 0; i < N; ++i) {
      std::swap(a[i][k], a[i][kprime]);
    }
    for (int i = k + 1; i < N; ++i) {
      a[k][i] = a[k][i] / a[k][k];
      for (int j = k + 1; j < N; ++j) {
        a[j][i] = a[j][i] - a[k][i] * a[j][k];
      }
    }
  }
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      if (i > j) {
        lower[j][i] = a[j][i];
      } else {
        upper[j][i] = a[j][i];
      }
    }
  }
}

template <typename T, int N>
VecFun<T, N> LupSolve(const MatBase<T, N> &l, const MatBase<T, N> &u,
                      const VecFun<int, N> &p, const VecFun<T, N> &b) {
  VecFun<T, N> x;
  VecFun<T, N> y;
  for (int i = 0; i < N; ++i) {
    T sum = 0;
    for (int j = 0; j < i; ++j) {
      sum += l[j][i] * y[j];
    }
    y[i] = b[p[i]] - sum;
  }
  for (int i = N - 1; i >= 0; --i) {
    T sum = 0;
    for (int j = i + 1; j < N; ++j) {
      sum += u[j][i] * x[j];
    }
    x[i] = (y[i] - sum) / u[i][i];
  }
  return x;
}

template <typename T, int N>
MatBase<T, N> Inverse(const MatBase<T, N> &m) {
  MatBase<T, N> l;
  MatBase<T, N> u;
  VecFun<int, N> p;
  LupDecomposition(m, l, u, p);
  MatBase<T, N> inv;
  for (int i = 0; i < N; ++i) {
    auto v = VecFun<T, N>();
    v[i] = 1;
    inv[i] = LupSolve(l, u, p, v);
  }
  return inv;
}
}  // namespace ren
#endif  // REN_MAT4_H_
