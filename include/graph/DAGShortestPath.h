#pragma once

#include <optional>
#include <utility>
#include <vector>

#include "graph/GeneralizedMaps.h"
#include "graph/Infinity.h"
#include "graph/KahnTopologicalSort.h"

namespace graph {

template <typename Graph, typename Weights, typename Dists, typename Preds>
class DAGShortestPath {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;
    using Weight = GeneralizedMapTraits<Weights>::Value;

public:
    DAGShortestPath(Graph &g, Vertex s, Weights weights, Dists dists, Preds preds)
        : g_(g), s_(s), weights_(std::move(weights)), dists_(std::move(dists)), preds_(std::move(preds)) {}

    void operator()() {
        for (Vertex v : g_.vertices()) {
            put(dists_, v, Infinity());
            put(preds_, v, std::nullopt);
        }
        put(dists_, s_, 0);
        put(preds_, s_, std::nullopt);
        std::vector<Vertex> sorted = KahnTopologicalSort(g_)();
        for (Vertex u : sorted) {
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                Weight w = get(weights_, e);
                if (get(dists_, v) > get(dists_, u) + w) {
                    put(dists_, v, get(dists_, u) + w);
                    put(preds_, v, u);
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
