#pragma once

#include <algorithm>
#include <cstddef>
#include <stack>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename SccNumbers>
class TarjanStronglyConnectedComponents {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

    template <typename V>
    using VertexMap = Graph::template VertexMap<V>;

public:
    TarjanStronglyConnectedComponents(Graph &g, SccNumbers sccNumbers)
        : g_(g), sccNumbers_(std::move(sccNumbers)) {}

    size_t operator()() {
        for (Vertex v : g_.vertices()) {
            put(dfn_, v, -1);
            put(sccNumbers_, v, -1);
        }
        for (Vertex v : g_.vertices()) {
            if (get(dfn_, v) == -1) {
                visit(v);
            }
        }
        return sccCount_;
    }

private:
    Graph &g_;
    VertexMap<size_t> dfn_, low_;
    size_t timer_{};
    std::stack<Vertex> S_;
    SccNumbers sccNumbers_;
    size_t sccCount_{};

    void visit(Vertex u) {
        put(dfn_, u, timer_);
        put(low_, u, timer_);
        ++timer_;
        S_.push(u);
        for (Edge e : g_.outEdges(u)) {
            Vertex v = g_.target(e);
            if (get(dfn_, v) == -1) {
                visit(v);
                put(low_, u, std::min(get(low_, u), get(low_, v)));
            } else if (get(sccNumbers_, v) == -1) {
                put(low_, u, std::min(get(low_, u), get(dfn_, v)));
            }
        }
        if (get(low_, u) == get(dfn_, u)) {
            Vertex v;
            do {
                v = S_.top();
                S_.pop();
                put(sccNumbers_, v, sccCount_);
            } while (v != u);
            ++sccCount_;
        }
    }
};

} // namespace graph
