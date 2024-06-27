#pragma once

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph>
class DFSBipartitenessCheck {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

    template <typename V>
    using VertexMap = Graph::template VertexMap<V>;

public:
    explicit DFSBipartitenessCheck(Graph &g) : g_(g) {}

    bool operator()() {
        for (Vertex v : g_.vertices()) {
            put(colors_, v, 0);
        }
        for (Vertex u : g_.vertices()) {
            if (get(colors_, u) == 0) {
                put(colors_, u, 1);
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

    bool visit(Vertex u) {
        for (Edge e : g_.outEdges(u)) {
            Vertex v = g_.target(e);
            if (get(colors_, v) == 0) {
                put(colors_, v, -get(colors_, u));
                if (!visit(v)) {
                    return false;
                }
            } else {
                if (get(colors_, v) == get(colors_, u)) {
                    return false;
                }
            }
        }
        return true;
    }
};

} // namespace graph
