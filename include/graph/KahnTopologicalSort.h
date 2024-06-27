#pragma once

#include <queue>
#include <vector>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph>
class KahnTopologicalSort {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

    template <typename V>
    using VertexMap = Graph::template VertexMap<V>;

public:
    explicit KahnTopologicalSort(Graph &g) : g_(g) {}

    std::vector<Vertex> operator()() {
        std::vector<Vertex> sorted;
        VertexMap<size_t> in;
        for (Vertex v : g_.vertices()) {
            put(in, v, 0);
        }
        for (Vertex u : g_.vertices()) {
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                put(in, v, get(in, v) + 1);
            }
        }
        std::queue<Vertex> Q;
        for (Vertex v : g_.vertices()) {
            if (get(in, v) == 0) {
                Q.push(v);
            }
        }
        while (!Q.empty()) {
            Vertex u = Q.front();
            Q.pop();
            sorted.push_back(u);
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                put(in, v, get(in, v) - 1);
                if (get(in, v) == 0) {
                    Q.push(v);
                }
            }
        }
        return sorted;
    }

private:
    Graph &g_;
};

} // namespace graph
