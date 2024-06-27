#pragma once

#include <utility>

#include "graph/Color.h"
#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename Colors>
class DepthFirstSearch {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

public:
    DepthFirstSearch(Graph &g, Colors colors)
        : g_(g), colors_(std::move(colors)) {}

    template <typename Visitor>
    void operator()(Visitor &visitor) {
        initialize();
        for (Vertex u : g_.vertices()) {
            if (get(colors_, u) == Color::kWhite) {
                visit(u, visitor);
            }
        }
    }

    void initialize() {
        for (Vertex v : g_.vertices()) {
            put(colors_, v, Color::kWhite);
        }
    }

    template <typename Visitor>
    void visit(Vertex u, Visitor &visitor) {
        put(colors_, u, Color::kGray);
        visitor.onDiscoverVertex(g_, u);

        for (Edge e : g_.outEdges(u)) {
            visitor.onDiscoverEdge(g_, e);

            Vertex v = g_.target(e);
            if (get(colors_, v) == Color::kWhite) {
                visit(v, visitor);
            }

            visitor.onFinishEdge(g_, e);
        }

        visitor.onFinishVertex(g_, u);
        put(colors_, u, Color::kBlack);
    }

private:
    Graph &g_;
    Colors colors_;
};

} // namespace graph
