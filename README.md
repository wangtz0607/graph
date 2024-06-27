# Graph

C++20 header-only generic graph library.

## Supported Algorithms

- `DepthFirstSearch`
- `BreadthFirstSearch`
- `KahnTopologicalSort`
- `KruskalMinimumSpanningTree`
- `PrimMinimumSpanningTree`
- `BFSShortestPath`
- `DAGShortestPath`
- `DijkstraShortestPath`
- `BellmanFordShortestPath`
- `DFSConnectedComponents`
- `BFSConnectedComponents`
- `TarjanCutVertices`
- `TarjanBridges`
- `TarjanStronglyConnectedComponents`
- `DFSBipartitenessCheck`
- `BFSBipartitenessCheck`

## Examples

### `DepthFirstSearch`

```cpp
#include <cstddef>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/DepthFirstSearch.h"
#include "graph/DefaultVisitor.h"
#include "graph/Color.h"

class CustomVisitor : public graph::DefaultVisitor {
public:
    template <typename Graph>
    void onDiscoverVertex(Graph &g, typename Graph::Vertex v) {
        // ...
    }
};

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    std::unordered_map<Vertex, graph::Color> colors;
    CustomVisitor visitor;

    graph::DepthFirstSearch(g, &colors)(visitor);

    return 0;
}
```

### `BreadthFirstSearch`

```cpp
#include <cstddef>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/BreadthFirstSearch.h"
#include "graph/DefaultVisitor.h"
#include "graph/Color.h"

class CustomVisitor : public graph::DefaultVisitor {
public:
    template <typename Graph>
    void onDiscoverVertex(Graph &g, typename Graph::Vertex v) {
        // ...
    }
};

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    std::unordered_map<Vertex, graph::Color> colors;
    CustomVisitor visitor;

    graph::BreadthFirstSearch(g, &colors)(visitor);

    return 0;
}
```

### `KahnTopologicalSort`

```cpp
#include <cassert>
#include <cstddef>
#include <vector>

#include "graph/DefaultDigraph.h"
#include "graph/KahnTopologicalSort.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    std::vector<Vertex> sorted = graph::KahnTopologicalSort(g)();

    assert(sorted == std::vector<Vertex>{0, 1, 2});

    return 0;
}
```

### `KruskalMinimumSpanningTree`

```cpp
#include <cassert>
#include <cstddef>
#include <unordered_set>

#include "graph/DefaultDigraph.h"
#include "graph/Empty.h"
#include "graph/KruskalMinimumSpanningTree.h"

int main() {
    using Vertex = size_t;
    
    struct EdgeProps {
        int weight;
    };

    using Graph = graph::DefaultDigraph<Vertex, graph::Empty, EdgeProps>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1, {1}); g.addEdge(1, 0, {1});
    g.addEdge(1, 2, {2}); g.addEdge(2, 1, {2});
    g.addEdge(0, 2, {4}); g.addEdge(2, 0, {4});

    std::unordered_set<Graph::Edge> mstEdges;

    graph::KruskalMinimumSpanningTree(g, g[&EdgeProps::weight], &mstEdges)();

    int weight = 0;
    for (auto e : mstEdges) {
        weight += g[e].weight;
    }
    assert(weight == 3);

    return 0;
}
```

### `PrimMinimumSpanningTree`

```cpp
#include <cassert>
#include <cstddef>
#include <optional>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/Empty.h"
#include "graph/PrimMinimumSpanningTree.h"

int main() {
    using Vertex = size_t;
    
    struct EdgeProps {
        int weight;
    };

    using Graph = graph::DefaultDigraph<Vertex, graph::Empty, EdgeProps>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1, {1}); g.addEdge(1, 0, {1});
    g.addEdge(1, 2, {2}); g.addEdge(2, 1, {2});
    g.addEdge(0, 2, {4}); g.addEdge(2, 0, {4});

    Vertex s = 0;

    std::unordered_map<Vertex, int> dists;
    std::unordered_map<Vertex, std::optional<Vertex>> preds;

    graph::PrimMinimumSpanningTree(g, s, g[&EdgeProps::weight], &dists, &preds)();

    int weight = 0;
    for (Vertex v : g.vertices()) {
        weight += dists[v];
    }
    assert(weight == 3);

    return 0;
}
```

### `BFSShortestPath`

```cpp
#include <cassert>
#include <cstddef>
#include <optional>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/BFSShortestPath.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1);
    g.addEdge(1, 2);
    g.addEdge(0, 2);

    Vertex s = 0;

    std::unordered_map<Vertex, size_t> dists;
    std::unordered_map<Vertex, std::optional<Vertex>> preds;

    graph::BFSShortestPath(g, s, &dists, &preds)();

    assert(dists[0] == 0);
    assert(dists[1] == 1);
    assert(dists[2] == 1);

    assert(preds[0] == std::nullopt);
    assert(preds[1] == 0);
    assert(preds[2] == 0);

    return 0;
}
```

### `DAGShortestPath`

```cpp
#include <cassert>
#include <cstddef>
#include <optional>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/Empty.h"
#include "graph/DAGShortestPath.h"

int main() {
    using Vertex = size_t;

    struct EdgeProps {
        int weight;
    };

    using Graph = graph::DefaultDigraph<Vertex, graph::Empty, EdgeProps>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1, {10});
    g.addEdge(1, 2, {100});
    g.addEdge(0, 2, {1000});

    Vertex s = 0;

    std::unordered_map<Vertex, int> dists;
    std::unordered_map<Vertex, std::optional<Vertex>> preds;

    graph::DAGShortestPath(g, s, g[&EdgeProps::weight], &dists, &preds)();

    assert(dists[0] == 0);
    assert(dists[1] == 10);
    assert(dists[2] == 110);

    assert(preds[0] == std::nullopt);
    assert(preds[1] == 0);
    assert(preds[2] == 1);

    return 0;
}
```

### `DijkstraShortestPath`

```cpp
#include <cassert>
#include <cstddef>
#include <optional>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/Empty.h"
#include "graph/DijkstraShortestPath.h"

int main() {
    using Vertex = size_t;

    struct EdgeProps {
        int weight;
    };

    using Graph = graph::DefaultDigraph<Vertex, graph::Empty, EdgeProps>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1, {10});
    g.addEdge(1, 2, {100});
    g.addEdge(0, 2, {1000});

    Vertex s = 0;

    std::unordered_map<Vertex, int> dists;
    std::unordered_map<Vertex, std::optional<Vertex>> preds;

    graph::DijkstraShortestPath(g, s, g[&EdgeProps::weight], &dists, &preds)();

    assert(dists[0] == 0);
    assert(dists[1] == 10);
    assert(dists[2] == 110);

    assert(preds[0] == std::nullopt);
    assert(preds[1] == 0);
    assert(preds[2] == 1);

    return 0;
}
```

### `BellmanFordShortestPath`

```cpp
#include <cassert>
#include <cstddef>
#include <optional>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/Empty.h"
#include "graph/BellmanFordShortestPath.h"

int main() {
    using Vertex = size_t;

    struct EdgeProps {
        int weight;
    };

    using Graph = graph::DefaultDigraph<Vertex, graph::Empty, EdgeProps>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1, {-10});
    g.addEdge(1, 2, {-100});
    g.addEdge(0, 2, {-1000});

    Vertex s = 0;

    std::unordered_map<Vertex, int> dists;
    std::unordered_map<Vertex, std::optional<Vertex>> preds;

    graph::BellmanFordShortestPath(g, s, g[&EdgeProps::weight], &dists, &preds)();

    assert(dists[0] == 0);
    assert(dists[1] == -10);
    assert(dists[2] == -1000);

    assert(preds[0] == std::nullopt);
    assert(preds[1] == 0);
    assert(preds[2] == 0);

    return 0;
}
```

### `DFSConnectedComponents`

```cpp
#include <cassert>
#include <cstddef>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/DFSConnectedComponents.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1); g.addEdge(1, 0);

    std::unordered_map<Vertex, size_t> componentNumbers;

    size_t componentCount = graph::DFSConnectedComponents(g, &componentNumbers)();

    assert(componentCount == 2);

    assert(componentNumbers[0] == componentNumbers[1]);
    assert(componentNumbers[2] != componentNumbers[0]);
    assert(componentNumbers[2] != componentNumbers[1]);

    return 0;
}
```

### `BFSConnectedComponents`

```cpp
#include <cassert>
#include <cstddef>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/BFSConnectedComponents.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1); g.addEdge(1, 0);

    std::unordered_map<Vertex, size_t> componentNumbers;

    size_t componentCount = graph::BFSConnectedComponents(g, &componentNumbers)(); // 2

    assert(componentCount == 2);

    assert(componentNumbers[0] == componentNumbers[1]);
    assert(componentNumbers[2] != componentNumbers[0]);
    assert(componentNumbers[2] != componentNumbers[1]);

    return 0;
}
```

### `TarjanCutVertices`

```cpp
#include <cassert>
#include <cstddef>
#include <unordered_set>

#include "graph/DefaultDigraph.h"
#include "graph/TarjanCutVertices.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1); g.addEdge(1, 0);
    g.addEdge(1, 2); g.addEdge(2, 1);

    std::unordered_set<Vertex> cutVertices;
    
    graph::TarjanCutVertices(g, &cutVertices)();
    
    assert(cutVertices == std::unordered_set<Vertex>{1});

    return 0;
}
```

### `TarjanBridges`

```cpp
#include <cassert>
#include <cstddef>

#include "graph/DefaultDigraph.h"
#include "graph/TarjanBridges.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);
    g.addVertex(3);

    g.addEdge(0, 1); g.addEdge(1, 0);
    g.addEdge(0, 2); g.addEdge(2, 0);
    g.addEdge(1, 2); g.addEdge(2, 1);
    g.addEdge(2, 3); g.addEdge(3, 2);

    auto isBridge = graph::TarjanBridges(g)();

    assert(isBridge(0, 1) == false);
    assert(isBridge(0, 2) == false);
    assert(isBridge(1, 2) == false);
    assert(isBridge(2, 3) == true);

    return 0;
}
```

### `TarjanStronglyConnectedComponents`

```cpp
#include <cassert>
#include <cstddef>
#include <unordered_map>

#include "graph/DefaultDigraph.h"
#include "graph/TarjanStronglyConnectedComponents.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);
    g.addVertex(2);

    g.addEdge(0, 1);
    g.addEdge(1, 0);
    g.addEdge(0, 2);

    std::unordered_map<Vertex, size_t> sccNumbers;

    size_t sccCount = graph::TarjanStronglyConnectedComponents(g, &sccNumbers)();

    assert(sccCount == 2);
    
    assert(sccNumbers[0] == sccNumbers[1]);
    assert(sccNumbers[2] != sccNumbers[0]);
    assert(sccNumbers[2] != sccNumbers[1]);

    return 0;
}
```

### `DFSBipartitenessCheck`

```cpp
#include <cassert>
#include <cstddef>

#include "graph/DefaultDigraph.h"
#include "graph/DFSBipartitenessCheck.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);

    g.addEdge(0, 1); g.addEdge(1, 0);

    assert(graph::DFSBipartitenessCheck(g)() == true);

    return 0;
}
```

### `BFSBipartitenessCheck`

```cpp
#include <cassert>
#include <cstddef>

#include "graph/DefaultDigraph.h"
#include "graph/BFSBipartitenessCheck.h"

int main() {
    using Vertex = size_t;
    using Graph = graph::DefaultDigraph<Vertex>;

    Graph g;

    g.addVertex(0);
    g.addVertex(1);

    g.addEdge(0, 1); g.addEdge(1, 0);

    assert(graph::BFSBipartitenessCheck(g)() == true);

    return 0;
}
```

## License

Graph is licensed under the [MIT license](https://opensource.org/licenses/MIT).
