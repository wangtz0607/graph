#pragma once

#include <cstddef>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename ComponentNumbers>
class DFSConnectedComponents {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

public:
    DFSConnectedComponents(Graph &g, ComponentNumbers componentNumbers)
        : g_(g), componentNumbers_(std::move(componentNumbers)) {}

    size_t operator()() {
        for (Vertex v : g_.vertices()) {
            put(componentNumbers_, v, -1);
        }
        size_t componentCount = 0;
        for (Vertex u : g_.vertices()) {
            if (get(componentNumbers_, u) == -1) {
                visit(u, componentCount);
                ++componentCount;
            }
        }
        return componentCount;
    }

private:
    Graph &g_;
    ComponentNumbers componentNumbers_;

    void visit(Vertex u, size_t componentNumber) {
        put(componentNumbers_, u, componentNumber);
        for (Edge e : g_.outEdges(u)) {
            Vertex v = g_.target(e);
            if (get(componentNumbers_, v) == -1) {
                visit(v, componentNumber);
            }
        }
    }
};

} // namespace graph
