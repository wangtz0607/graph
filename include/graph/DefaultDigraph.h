#pragma once

#include <iterator>
#include <list>
#include <ranges>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "graph/detail/ArrowProxy.h"
#include "graph/detail/CompressedPair.h"
#include "graph/detail/hash.h"
#include "graph/detail/MapKeyIterator.h"
#include "graph/Empty.h"
#include "graph/GeneralizedMaps.h"

namespace graph {

template <typename VertexT, typename VertexPropsT, typename EdgePropsT>
class DefaultDigraphEdge;

template <typename VertexT, typename VertexPropsT = Empty, typename EdgePropsT = Empty>
class DefaultDigraph;

template <typename VertexT, typename VertexPropsT, typename EdgePropsT>
class DefaultDigraphEdge {
private:
    VertexT source_;
    std::list<detail::CompressedPair<VertexT, EdgePropsT>>::const_iterator node_;

    DefaultDigraphEdge(VertexT source, std::list<detail::CompressedPair<VertexT, EdgePropsT>>::const_iterator node)
        : source_(source), node_(node) {}

    friend DefaultDigraph<VertexT, VertexPropsT, EdgePropsT>;
    friend std::hash<DefaultDigraphEdge<VertexT, VertexPropsT, EdgePropsT>>;

    friend bool operator==(DefaultDigraphEdge lhs, DefaultDigraphEdge rhs) {
        return lhs.source_ == rhs.source_ && lhs.node_ == rhs.node_;
    }
};

template <typename VertexT, typename VertexPropsT, typename EdgePropsT>
class DefaultDigraph {
public:
    using Vertex = VertexT;
    using Edge = DefaultDigraphEdge<VertexT, VertexPropsT, EdgePropsT>;
    using VertexProps = VertexPropsT;
    using EdgeProps = EdgePropsT;

    template <typename V>
    using VertexMap = std::conditional_t<std::is_same_v<V, bool>,
                                         std::unordered_set<Vertex>,
                                         std::unordered_map<Vertex, V>>;

    template <typename V>
    using EdgeMap = std::conditional_t<std::is_same_v<V, bool>,
                                       std::unordered_set<Edge>,
                                       std::unordered_map<Edge, V>>;

    using VertexIterator =
        detail::MapKeyIterator<
            std::unordered_map<
                Vertex,
                detail::CompressedPair<
                    VertexProps,
                    std::list<detail::CompressedPair<Vertex, EdgeProps>>>>>;

    class OutEdgeIterator {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using Value = Edge;
        using difference_type = ptrdiff_t;

        OutEdgeIterator() = default;

        Edge operator*() const {
            return Edge(source_, node_);
        }

        detail::ArrowProxy<Edge> operator->() const {
            return detail::ArrowProxy<Edge>::make(source_, node_);
        }

        OutEdgeIterator &operator++() {
            ++node_;
            return *this;
        }

        OutEdgeIterator operator++(int) {
            return OutEdgeIterator(source_, node_++);
        }

        OutEdgeIterator &operator--() {
            --node_;
            return *this;
        }

        OutEdgeIterator operator--(int) {
            return OutEdgeIterator(source_, node_--);
        }

        friend bool operator==(OutEdgeIterator lhs, OutEdgeIterator rhs) {
            return lhs.source_ == rhs.source_ && lhs.node_ == rhs.node_;
        }

    private:
        Vertex source_;
        std::list<detail::CompressedPair<Vertex, EdgeProps>>::const_iterator node_;

        OutEdgeIterator(Vertex source,
                          std::list<detail::CompressedPair<Vertex, EdgeProps>>::const_iterator node)
            : source_(source), node_(node) {}

        friend DefaultDigraph;
    };

    template <typename T>
    class VertexProp {
    public:
        using Key = Vertex;
        using Value = T;

    private:
        DefaultDigraph *graph_;
        T VertexProps::*member_;

        VertexProp(DefaultDigraph *graph, T VertexProps::*member) : graph_(graph), member_(member) {}

        friend DefaultDigraph;

        friend T &get(VertexProp prop, Vertex vertex) {
            return (*prop.graph_)[vertex].*prop.member_;
        }

        friend void put(VertexProp prop, Vertex vertex, T value) {
            (*prop.graph_)[vertex].*prop.member_ = std::move(value);
        }
    };

    template <typename T>
    class ConstVertexProp {
    public:
        using Key = Vertex;
        using Value = T;

    private:
        const DefaultDigraph *graph_;
        T VertexProps::*member_;

        ConstVertexProp(const DefaultDigraph *graph, T VertexProps::*member) : graph_(graph), member_(member) {}
        
        friend DefaultDigraph;

        friend const T &get(ConstVertexProp prop, Vertex vertex) {
            return (*prop.graph)[vertex].*prop.member_;
        }
    };

    template <typename T>
    class EdgeProp {
    public:
        using Key = Edge;
        using Value = T;

    private:
        DefaultDigraph *graph_;
        T EdgeProps::*member_;

        EdgeProp(DefaultDigraph *graph, T EdgeProps::*member) : graph_(graph), member_(member) {}

        friend DefaultDigraph;

        friend T &get(EdgeProp prop, Edge edge) {
            return (*prop.graph_)[edge].*prop.member_;
        }

        friend void put(EdgeProp prop, Edge edge, T value) {
            (*prop.graph_)[edge].*prop.member_ = std::move(value);
        }
    };

    template <typename T>
    class ConstEdgeProp {
    public:
        using Key = Edge;
        using Value = T;

    private:
        const DefaultDigraph *graph_;
        T EdgeProps::*member_;

        ConstEdgeProp(const DefaultDigraph *graph, T EdgeProps::*member) : graph_(graph), member_(member) {}

        friend DefaultDigraph;

        friend const T &get(ConstEdgeProp prop, Edge edge) {
            return (*prop.graph_)[edge].*prop.member_;
        }
    };

    Vertex source(Edge edge) const {
        return edge.source_;
    }

    Vertex target(Edge edge) const {
        return edge.node_->first();
    }

    VertexProps &operator[](Vertex vertex) {
        return get(adj_, vertex).first();
    }

    const VertexProps &operator[](Vertex vertex) const {
        return get(adj_, vertex).first();
    }

    EdgeProps &operator[](Edge edge) {
        return const_cast<detail::CompressedPair<VertexT, EdgePropsT> &>(*edge.node_).second();
    }

    const EdgeProps &operator[](Edge edge) const {
        return const_cast<const detail::CompressedPair<VertexT, EdgePropsT> &>(*edge.node_).second();
    }

    template <typename T>
    VertexProp<T> operator[](T VertexPropsT::*member) {
        return VertexProp<T>(this, member);
    }

    template <typename T>
    ConstVertexProp<T> operator[](T VertexPropsT::*member) const {
        return ConstVertexProp(this, member);
    }

    template <typename T>
    EdgeProp<T> operator[](T EdgeProps::*member) {
        return EdgeProp<T>(this, member);
    }

    template <typename T>
    ConstEdgeProp<T> operator[](T EdgeProps::*member) const {
        return ConstEdgeProp(this, member);
    }

    size_t numVertices() const {
        return adj_.size();
    }

    auto vertices() const {
        return std::ranges::subrange(VertexIterator(adj_.begin()), VertexIterator(adj_.end()));
    }

    size_t numOutEdges(Vertex vertex) const {
        return get(adj_, vertex).second().size();
    }

    auto outEdges(Vertex vertex) const {
        return std::ranges::subrange(
            OutEdgeIterator(vertex, get(adj_, vertex).second().begin()),
            OutEdgeIterator(vertex, get(adj_, vertex).second().end())
        );
    }

    Vertex addVertex(Vertex vertex, VertexProps props = {}) {
        if (!contains(adj_, vertex)) {
            put(adj_, vertex, {std::move(props), {}});
        }
        return vertex;
    }

    Edge addEdge(Vertex source, Vertex target, EdgeProps props = {}) {
        auto &list = get(adj_, source).second();
        return Edge(source, list.insert(list.end(), {target, std::move(props)}));
    }

private:
    std::unordered_map<
        Vertex,
        detail::CompressedPair<
            VertexProps,
            std::list<detail::CompressedPair<Vertex, EdgeProps>>>> adj_;
};

} // namespace graph

template <typename Vertex, typename VertexProps, typename EdgeProps>
struct std::hash<graph::DefaultDigraphEdge<Vertex, VertexProps, EdgeProps>> {
    constexpr size_t operator()(graph::DefaultDigraphEdge<Vertex, VertexProps, EdgeProps> edge) const noexcept {
        return graph::detail::hashValues(edge.source_, &*edge.node_);
    }
};
