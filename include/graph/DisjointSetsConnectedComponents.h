#pragma once

#include <cstddef>
#include <utility>

#include "graph/detail/DisjointSets.h"
#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename RepresentativeVertices>
class DisjointSetsConnectedComponents {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

public:
    DisjointSetsConnectedComponents(Graph &g, RepresentativeVertices representativeVertices)
        : g_(g), representativeVertices_(std::move(representativeVertices)) {}
    
    size_t operator()() {
        detail::DisjointSets<Vertex> D;
        size_t componentCount = 0;
        for (Vertex v : g_.vertices()) {
            D.makeSet(v);
            ++componentCount;
        }
        for (Vertex u : g_.vertices()) {
            for (Edge e : g_.outEdges(u)) {
                Vertex v = g_.target(e);
                if (D.find(u) != D.find(v)) {
                    D.union_(u, v);
                    --componentCount;
                }
            }
        }
        for (Vertex v : g_.vertices()) {
            put(representativeVertices_, v, D.find(v));
        }
        return componentCount;
    }

private:
    Graph &g_;
    RepresentativeVertices representativeVertices_;
};

} // namespace graph
