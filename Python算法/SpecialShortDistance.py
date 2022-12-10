from heapq import heappush, heappop
# 建立图
n, m, k = map(int, input().split())
graph = {}

for i in range(m):
    u, v, w = map(int, input().split())
    if u not in graph:
        graph[u] = [(v, w)]
    else:
        graph[u].append((v, w))

    if v not in graph:
        graph[v] = [(u, w)]
    else:
        graph[v].append((u, w))
def dijkstra(graph, start):
    # 定义堆
    heap = []
    # 定义节点1到各个节点的距离
    dist = [float("inf")] * (n + 1)
    # 设置起始节点到自身的距离为0
    dist[start] = 0
    # 设置已经遍历过的节点
    vis = [False for _ in range(n+1)]
    # 添加节点1到堆中 0 节点1到自身的距离
    heappush(heap, (0, 1))
    while heap:
        # d 从节点1到节点node的距离
        d, node = heappop(heap)
        # 如果该节点已经遍历过跳过
        if vis[node]:
            continue
        # 遍历过的节点
        vis[node] = True
        # 遍历图，获取节点node对应邻接节点v和邻接边距离w
        for v, w in graph[node]:
            # d = l(i-1)
            # 如果距离w满足 k * l(i-1)
            if w == d * k:
                # 则 w = l(i-1) * (k-1)
                w = d * (k - 1)
            # 如果当前节点v的距离比d+w大
            if dist[v] > d + w:
                # 则更换新的距离
                dist[v] = d + w
                heappush(heap, (d+w, v))
    return dist

dist = dijkstra(graph, 1)
# 遍历节点1到各个节点的距离
for i in range(1, n+1):
    if dist[i] == float("inf"):
        print(-1, end=" ")
    else:
        print(dist[i], end=" ")