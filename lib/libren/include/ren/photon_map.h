#ifndef REN_PHOTONMAP_H_
#define REN_PHOTONMAP_H_
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>
#include <queue>
#include <vector>
#include "ren/typedefs.h"
#include "ren/vec.h"
namespace ren {
// Small structure representing a photon.
struct Photon {
  Vec3 pos;
  Vec3 power;
  Vec3 dir;
  Photon(const Vec3 &position, const Vec3 &weight, const Vec3 &direction)
      : pos(position), power(weight), dir(direction) {}
  Photon() : pos(0), power(0), dir(0) {}
};

// Generic kd-tree.
template <typename Data>
class KdTree {
 public:
  // The result of a query operation.
  struct QueryResult {
    Data data;
    Real distance2;
    bool operator<(const KdTree::QueryResult &rhs) {
      return distance2 < rhs.distance2;
    };
    bool operator>(const KdTree::QueryResult &rhs) {
      return distance2 > rhs.distance2;
    }
    QueryResult() : data(), distance2(0) {}
    QueryResult(const Data &data_res, Real distance2_res)
        : data(data_res), distance2(distance2_res) {}
  };

  KdTree(const std::vector<Data> &data)
      : nodes_(data.size()), data_(data.size()), next_node_(1) {
    std::vector<const Data *> data_ptrs;
    data_ptrs.reserve(data.size());
    for (int i = 0; i < data.size(); ++i) {
      data_ptrs.push_back(&data[i]);
    }
    Balance(0, data.size(), 0, &data_ptrs[0]);
  }
  // Query the \p n nearest elements closest to \p p.
  // @param p the point we are interested in. The returned point should be
  // closed to this one.
  // @param results the vector where to store the results. The vector is a max
  // heap, i.e., the first element of has the largest distance to \p p squaed.
  void QueryNearest(const Vec3 &p, int n,
                    std::vector<QueryResult> &results) const {
    results.clear();
    Real r2 = std::numeric_limits<Real>::max();
    QueryRecursive(p, n, r2, results, 0);
  }

 private:
  struct Node {
    Real pos;
    unsigned int dim : 2;
    unsigned int has_left_child : 1;
    unsigned int right_child : 29;
    Node() : dim(3), right_child((1 << 29) - 1), has_left_child(0) {}
    Node(Real position, int dimension)
        : pos(position),
          dim(dimension),
          right_child((1 << 29) - 1),
          has_left_child(0) {}
  };

  void Balance(int begin, int end, int node, const Data **data_ptrs) {
    if (begin + 1 == end) {
      nodes_[node] = Node();
      data_[node] = *data_ptrs[begin];
      return;
    }

    Vec3 min(std::numeric_limits<Real>::max());
    Vec3 max(std::numeric_limits<Real>::min());
    // calculate extent of the bounding  box
    {
      for (int i = begin; i < end; ++i) {
        for (int dim = 0; dim < 3; ++dim) {
          auto value_dim = data_ptrs[i]->pos[dim];
          if (value_dim < min[dim]) min[dim] = value_dim;
          if (value_dim > max[dim]) max[dim] = value_dim;
        }
      }
    }

    int dim = 0;
    // calculate the dimension of the bounding box having maximum extent
    {
      Real distance = std::numeric_limits<Real>::min();
      for (int i = 0; i < 3; ++i) {
        Real tmp_distance = std::abs(max[i] - min[i]);
        if (tmp_distance > distance) {
          distance = tmp_distance;
          dim = i;
        }
      }
    }
    int m = (begin + end) / 2;
    std::nth_element(
        &data_ptrs[begin], &data_ptrs[m], &data_ptrs[end],
        [dim](const Data *a, const Data *b) -> bool {
          return a->pos[dim] == b->pos[dim] ? a < b : a->pos[dim] < b->pos[dim];
        });
    nodes_[node] = Node(data_ptrs[m]->pos[dim], dim);
    data_[node] = *data_ptrs[m];
    if (begin < m) {
      nodes_[node].has_left_child = 1;
      int child_num = next_node_++;
      Balance(begin, m, child_num, data_ptrs);
    }

    if (m + 1 < end) {
      nodes_[node].right_child = next_node_++;
      Balance(m + 1, end, nodes_[node].right_child, data_ptrs);
    }
  }

  void QueryRecursive(const Vec3 &p, int max, Real &r2,
                      std::vector<QueryResult> &results, int node) const {
    const Node *n = &nodes_[node];
    if (n->dim != 3) {
      Real distance2 = p[n->dim] - n->pos;
      distance2 *= distance2;
      if (p[n->dim] <= n->pos) {
        if (n->has_left_child) QueryRecursive(p, max, r2, results, node + 1);
        if (distance2 < r2 && n->right_child < nodes_.size())
          QueryRecursive(p, max, r2, results, n->right_child);
      } else {
        if (n->right_child < nodes_.size())
          QueryRecursive(p, max, r2, results, n->right_child);
        if (distance2 < r2 && n->has_left_child)
          QueryRecursive(p, max, r2, results, node + 1);
      }
    }

    Real distance2 = Length(data_[node].pos - p);
    distance2 *= distance2;
    if (distance2 < r2) {
      QueryResult result;
      result.data = data_[node];
      result.distance2 = distance2;
      results.push_back(result);
      std::push_heap(results.begin(), results.end());
      if (results.size() > max) {
        std::pop_heap(results.begin(), results.end());
        r2 = results.back().distance2;
        results.pop_back();
      }
    }
  }

  std::vector<Node> nodes_;
  std::vector<Data> data_;
  int next_node_;
};

typedef KdTree<Photon> PhotonMap;

}  // namespace ren
#endif  // REN_PHOTONMAP_H_
