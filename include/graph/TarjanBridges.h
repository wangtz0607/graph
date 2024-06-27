#pragma once

#include <algorithm>
#include <cstddef>
#include <optional>
#include <utility>

#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename Graph>
class TarjanBridges {
    using Vertex = Graph::Vertex;
    using Edge = Graph::Edge;

    template <typename V>
    using VertexMap = Graph::template VertexMap<V>;

public:
    class IsBridge {
    public:
        bool operator()(Vertex u, Vertex v) const {
            return (get(bridges_, u) && get(parents_, u) == v) ||
                   (get(bridges_, v) && get(parents_, v) == u);
        }

        bool operator()(Edge e) const {
            return isBridge(g_->source(e), g_->target(e));
        }

    private:
        Graph *g_;
        VertexMap<std::optional<Vertex>> parents_;
        VertexMap<bool> bridges_;

        IsBridge(Graph *g,
                 VertexMap<std::optional<Vertex>> parents,
                 VertexMap<bool> bridges)
            : g_(g), parents_(std::move(parents)), bridges_(std::move(bridges)) {}

        friend TarjanBridges;
    };

    explicit TarjanBridges(Graph &g) : g_(g) {}

    IsBridge operator()() {
        for (Vertex v : g_.vertices()) {
            put(dfn_, v, -1);
            put(parents_, v, std::nullopt);
            put(bridges_, v, false);
        }
        for (Vertex u : g_.vertices()) {
            if (get(dfn_, u) == -1) {
                visit(u);
            }
        }
        return IsBridge(&g_, std::move(parents_), std::move(bridges_));
    }

private:
    Graph &g_;
    VertexMap<size_t> dfn_, low_;
    size_t timer_{};
    VertexMap<std::optional<Vertex>> parents_;
    VertexMap<bool> bridges_;

    void visit(Vertex u) {
        put(dfn_, u, timer_);
        put(low_, u, timer_);
        ++timer_;
        for (Edge e : g_.outEdges(u)) {
            Vertex v = g_.target(e);
            if (get(dfn_, v) == -1) {
                put(parents_, v, u);
                visit(v);
                put(low_, u, std::min(get(low_, u), get(low_, v)));
                if (get(low_, v) > get(dfn_, u)) {
                    put(bridges_, v, true);
                }
            } else if (v != get(parents_, u)) {
                put(low_, u, std::min(get(low_, u), get(dfn_, v)));
            }
        }
    }
};

} // namespace graph
