#pragma once

#include <queue>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph>
class BFSBipartitenessCheck {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

    template <typename V>
    using VertexMap = Graph::template VertexMap<V>;

public:
    explicit BFSBipartitenessCheck(Graph &g) : g_(g) {}

    bool operator()() {
        for (Vertex v : g_.vertices()) {
            put(colors_, v, 0);
        }
        for (Vertex u : g_.vertices()) {
            if (get(colors_, u) == 0) {
                if (!visit(u)) {
                    return false;
                }
            }
        }
        return true;
    }

private:
    Graph &g_;
    VertexMap<int> colors_;

    bool visit(Vertex s) {
        std::queue<Vertex> Q;

        put(colors_, s, 1);
        Q.push(s);
        while (!Q.empty()) {
            Vertex u = Q.front();
            Q.pop();
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                if (get(colors_, v) == 0) {
                    put(colors_, v, -get(colors_, u));
                    Q.push(v);
                } else {
                    if (get(colors_, v) == get(colors_, u)) {
                        return false;
                    }
                }
            }
        }
        return true;
    }
};

} // namespace graph
