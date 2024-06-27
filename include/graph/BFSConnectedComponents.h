#pragma once

#include <cstddef>
#include <queue>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename ComponentNumbers>
class BFSConnectedComponents {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

public:
    BFSConnectedComponents(Graph &g, ComponentNumbers componentNumbers)
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

    void visit(Vertex s, size_t componentNumber) {
        std::queue<Vertex> Q;

        put(componentNumbers_, s, componentNumber);
        Q.push(s);
        while (!Q.empty()) {
            Vertex u = Q.front();
            Q.pop();
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                if (get(componentNumbers_, v) == -1) {
                    put(componentNumbers_, v, componentNumber);
                    Q.push(v);
                }
            }
        }
    }
};

} // namespace graph
