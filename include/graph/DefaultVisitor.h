#pragma once

namespace graph {

class DefaultVisitor {
public:
    template <typename Graph>
    void onDiscoverVertex(Graph &g, typename Graph::Vertex v) {}

    template <typename Graph>
    void onFinishVertex(Graph &g, typename Graph::Vertex v) {}

    template <typename Graph>
    void onDiscoverEdge(Graph &g, typename Graph::Edge e) {}

    template <typename Graph>
    void onFinishEdge(Graph &g, typename Graph::Edge e) {}
};

} // namespace graph
