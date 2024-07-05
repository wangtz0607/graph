#pragma once

#include <optional>
#include <queue>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename Dists, typename Preds>
class BFSShortestPaths {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

public:
    BFSShortestPaths(Graph &g, Vertex s, Dists dists, Preds preds)
        : g_(g), s_(s), dists_(std::move(dists)), preds_(std::move(preds)) {}
    
    void operator()() {
        for (Vertex v : g_.vertices()) {
            put(dists_, v, -1);
            put(preds_, v, std::nullopt);
        }
        put(dists_, s_, 0);
        put(preds_, s_, std::nullopt);
        std::queue<Vertex> Q;
        Q.push(s_);
        while (!Q.empty()) {
            Vertex u = Q.front();
            Q.pop();
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                if (get(dists_, v) == -1) {
                    put(dists_, v, get(dists_, u) + 1);
                    put(preds_, v, u);
                    Q.push(v);
                }
            }
        }
    }

private:
    Graph &g_;
    Vertex s_;
    Dists dists_;
    Preds preds_;
};

} // namespace graph
