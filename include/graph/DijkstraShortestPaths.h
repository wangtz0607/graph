#pragma once

#include <set>
#include <optional>
#include <utility>

#include "graph/GeneralizedMaps.h"
#include "graph/Infinity.h"

namespace graph {

template <typename Graph, typename Weights, typename Dists, typename Preds>
class DijkstraShortestPaths {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;
    using Weight = GeneralizedMapTraits<Weights>::Value;

public:
    DijkstraShortestPaths(Graph &g, Vertex s, Weights weights, Dists dists, Preds preds)
        : g_(g),
          s_(s),
          weights_(std::move(weights)),
          dists_(std::move(dists)),
          preds_(std::move(preds)) {}

    void operator()() {
        for (Vertex v : g_.vertices()) {
            put(dists_, v, Infinity());
            put(preds_, v, std::nullopt);
        }
        put(dists_, s_, 0);
        put(preds_, s_, std::nullopt);
        std::set<std::pair<Weight, Vertex>> U{{0, s_}};
        while (!U.empty()) {
            Vertex u = U.begin()->second;
            U.erase(U.begin());
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                Weight w = get(weights_, e);
                if (get(dists_, v) > get(dists_, u) + w) {
                    U.erase({get(dists_, v), v});
                    put(dists_, v, get(dists_, u) + w);
                    put(preds_, v, u);
                    U.insert({get(dists_, v), v});
                }
            }
        }
    }

private:
    Graph &g_;
    Vertex s_;
    Weights weights_;
    Dists dists_;
    Preds preds_;
};

} // namespace graph
