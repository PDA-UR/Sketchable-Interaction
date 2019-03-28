
class LinkingGraph(dict):
    def __init__(self):
        super(LinkingGraph, self).__init__()

    def add_node(self, nnode, *linked):
        # for each node in linked add an edge to nnode

        if nnode not in self:
            self[nnode] = []

        if len(linked) == 0:
            self[nnode] = []
        else:
            for link in linked:
                if link not in self:
                    self[link] = [nnode]
                else:
                    self[link].append(nnode)

    def find_all_paths(self, start, end, path=[]):
        path = path + [start]

        if start == end:
            return [path]

        if start not in self.keys():
            return []

        paths = []

        for node in self[start]:
            if node not in path:
                newpaths = self.find_all_paths(node, end, path)

                for newpath in newpaths:
                    paths.append(newpath)

        return paths

    def get_node_reach(self, nnode, onode):  # restrict this to include nnode and exclude onode
        paths = []
        path = set()

        def visit(node):
            b = False

            if node == onode:
                return True
            else:
                path.add(node)
                paths.append(list(path))

                if node in self:
                    for neighbour in self[node]:
                        if neighbour in path or visit(neighbour):
                            b = True

            return b

        visit(nnode)

        ret = []

        for p in paths:
            ret += p

        ret = list(set(ret)) + [onode]  # reduce list of duplicate nodes

        is_legal, error = self.is_legal_sub_graph(nnode, onode, ret)

        if not is_legal:
            raise SyntaxError("Illegal subgraph: {0}".format(error))

        return ret

    def is_legal_sub_graph(self, nnode, onode, sub_graph):
        for node in self:
            if node in sub_graph:
                for n in self[node]:
                    if n not in sub_graph:
                        if node is onode:
                            continue

                        return False, "region {0} is not part of the desired sub-graph".format(n)
            else:
                for n in self[node]:
                    if n in sub_graph:
                        if n == nnode or n == onode:
                            continue

                        return False, "region {0} is not the entry region ({1} is), however regions which are not part of the sub-graph link to {0}!".format(n, nnode)

        return True, ""

    def retrieve_sub_linking_graph(self, reach):
        sub = LinkingGraph()

        for node in reach:
            sub[node] = self[node]

        return sub


if __name__ == '__main__':
    lg = LinkingGraph()

    lg.add_node("A", "Q")
    lg.add_node("B", "A")
    lg.add_node("B", "C")
    lg.add_node("C", "B")
    lg.add_node("E", "B")
    lg.add_node("B", "E")
    lg.add_node("D", "C")
    lg.add_node("D", "E")
    lg.add_node("E", "D")
    lg.add_node("F", "E")
    lg.add_node("R", "F")
    lg.add_node("P", "R")
    lg.add_node("U", "F")

    print(lg)
    #print()
    lg.retrieve_sub_linking_graph(lg.get_node_reach("C", "R"))
