#pragma once

#include <queue>
#include <utility>

#include "graph/Color.h"
#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph, typename Colors>
class BreadthFirstSearch {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

public:
    BreadthFirstSearch(Graph &g, Colors colors)
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
    void visit(Vertex s, Visitor &visitor) {
        std::queue<Vertex> Q;

        put(colors_, s, Color::kGray);
        visitor.onDiscoverVertex(g_, s);
        Q.push(s);

        while (!Q.empty()) {
            Vertex u = Q.front();
            Q.pop();

            for (Edge e : g_.outEdges(u)) {
                visitor.onDiscoverEdge(g_, e);

                Vertex v = g_.target(e);
                if (get(colors_, v) == Color::kWhite) {
                    put(colors_, v, Color::kGray);
                    visitor.onDiscoverVertex(g_, v);
                    Q.push(v);
                }

                visitor.onFinishEdge(g_, e);
            }

            visitor.onFinishVertex(g_, u);
            put(colors_, u, Color::kBlack);
        }
    }

private:
    Graph &g_;
    Colors colors_;
};

} // namespace graph
