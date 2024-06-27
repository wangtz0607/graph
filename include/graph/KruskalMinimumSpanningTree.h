#pragma once

#include <algorithm>
#include <utility>
#include <vector>

#include "graph/GeneralizedMaps.h"
#include "graph/detail/DisjointSets.h"

namespace graph {

template <typename Graph, typename Weights, typename MstEdges>
class KruskalMinimumSpanningTree {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;
    using Weight = graph::GeneralizedMapTraits<Weights>::Value;

public:
    KruskalMinimumSpanningTree(Graph &g, Weights weights, MstEdges mstEdges)
        : g_(g), weights_(std::move(weights)), mstEdges_(std::move(mstEdges)) {}
    
    void operator()() {
        std::vector<Edge> edges;
        for (Vertex u : g_.vertices()) {
            for (Edge e : g_.outEdges(u)) {
                edges.push_back(e);
            }
        }
        std::ranges::sort(edges, [this](Edge &lhs, Edge &rhs) {
            return get(weights_, lhs) < get(weights_, rhs);
        });
        detail::DisjointSets<Vertex> D;
        for (Vertex v : g_.vertices()) {
            D.makeSet(v);
        }
        for (Edge e : edges) {
            put(mstEdges_, e, false);
        }
        for (Edge e : edges) {
            if (D.find(g_.source(e)) != D.find(g_.target(e))) {
                D.union_(g_.source(e), g_.target(e));
                put(mstEdges_, e, true);
            }
        }
    }

private:
    Graph &g_;
    Weights weights_;
    MstEdges mstEdges_;
};

} // namespace graph
