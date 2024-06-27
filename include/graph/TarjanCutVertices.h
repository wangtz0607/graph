#pragma once

#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename CutVertices>
class TarjanCutVertices {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

    template <typename V>
    using VertexMap = Graph::template VertexMap<V>;

public:
    explicit TarjanCutVertices(Graph &g, CutVertices cutVertices)
        : g_(g), cutVertices_(std::move(cutVertices)) {}

    void operator()() {
        for (Vertex v : g_.vertices()) {
            put(dfn_, v, -1);
            put(cutVertices_, v, false);
        }
        for (Vertex u : g_.vertices()) {
            if (get(dfn_, u) == -1) {
                visit(u, std::nullopt);
            }
        }
    }

private:
    Graph &g_;
    VertexMap<size_t> dfn_, low_;
    size_t timer_{};
    CutVertices cutVertices_;

    void visit(Vertex u, std::optional<Vertex> parent) {
        put(dfn_, u, timer_);
        put(low_, u, timer_);
        ++timer_;
        size_t count = 0;
        for (Edge e : g_.outEdges(u)) {
            Vertex v = g_.target(e);
            if (get(dfn_, v) == -1) {
                ++count;
                visit(v, u);
                put(low_, u, std::min(get(low_, u), get(low_, v)));
                if (get(low_, v) >= get(dfn_, u)) {
                    put(cutVertices_, u, true);
                }
            } else if (v != parent) {
                put(low_, u, std::min(get(low_, u), get(dfn_, v)));
            }
        }
        if (!parent.has_value() && count == 1) {
            put(cutVertices_, u, false);
        }
    }
};

} // namespace graph
