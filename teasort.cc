#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iterator>
#include <numeric>
#include <random>
#include <vector>

struct Vertex
{
  typedef std::vector<Vertex*> Neighbours;
  unsigned int id;
  void *value;
  bool visited;
  Neighbours neighbours;
  inline void visit() { visited = true; }
  Vertex() { id = 0; value = NULL; visited = false; }
};

struct Graph
{
  typedef Vertex* iterator;
  size_t size;
  Vertex* vertices;
  Graph(unsigned int nsize)
  {
    size = nsize;
    vertices = new Vertex[size];
    for (int i = 0; i < size; i++)
      vertices[i].id = i;
  }
  ~Graph(void) { delete[] vertices; }
  inline iterator begin(void) { return vertices; }
  inline iterator end(void) { return vertices + size; }
  inline Vertex &operator[] (unsigned int n) { return vertices[n]; }
  inline void add_edge(unsigned int x, unsigned int y)
  {
    assert(x < size && y < size);
    vertices[x].neighbours.push_back(vertices + y);
  }
};

void dfs_visit(Vertex &u, std::vector<Vertex*> &result)
{
  assert(!u.visited);
  u.visit();
  for (Vertex::Neighbours::iterator vpp = u.neighbours.begin(); vpp != u.neighbours.end(); vpp++)
  {
    Vertex &v = **vpp;
    if (!v.visited)
      dfs_visit(v, result);
  }
  result.push_back(&u);
}

std::vector<Vertex*> dfs(Graph &graph)
{
  std::vector<Vertex*> result;
  for (Graph::iterator vp = graph.begin(); vp < graph.end(); vp++)
  {
    if (!vp->visited)
      dfs_visit(*vp, result);
  }
  return result;
}

unsigned int teasort(int *p, int *q)
{
  int n = q - p;
  int m = 2 * n * std::ilogb((double)n);

  static std::random_device rdev;
  static std::default_random_engine rng(rdev());
  std::uniform_int_distribution<> rand(0, n - 1);

  Graph graph(n);
  for (int i = 0; i < n; i++)
    graph[i].value = p + i;

  for (int i = 0; i < m; i++)
  {
    int x = rand(rng);
    int y = rand(rng);
    if (p[y] > p[x])
      std::swap(x, y);
    graph.add_edge(x, y);
  }

  std::vector<Vertex*> result = dfs(graph);

  assert(result.size() == n);

  int *t = new int[n + 1];
  t[0] = std::numeric_limits<int>::min();
  for (int i = 0; i < n; i++)
    t[i + 1] = *((int*)result[i]->value);
  for (int i = 2; i <= n; i++)
  {
    int j = i;
    int v = t[i];
    while (m++, t[j - 1] > v) {
      t[j] = t[j - 1];
      j--;
    }
    t[j] = v;
  }
  for (int i = 0; i < n; i++)
    assert(t[i] <= t[i + 1]);
  std::copy(t + 1, t + n + 1, p);
  delete[] t;

  return m;
}

#define TEST_MIN (1 << 5)
#define TEST_ITER 16

int main(void)
{
  unsigned int inv;
  double finv;
  for (int n = TEST_MIN; ; n *= 2)
  {
    int *p = new int[n];
    std::iota(p, p + n, 1);
    std::random_shuffle(p, p + n);
    inv = 0;
    for (int i = 0; i < TEST_ITER; i++)
      inv += teasort(p, p + n);
    finv = (double)inv / (double)TEST_ITER;
    printf("%8d\t%16.2f\n", n, finv);
    fflush(stdout);
  }
  return 0;
}

// vim:ts=2 sts=2 sw=2 et
