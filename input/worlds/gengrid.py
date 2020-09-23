import networkx as nx 


N = 1000

G = nx.generators.grid_2d_graph(N,N)
G =nx.convert_node_labels_to_integers(G,first_label=1)

nx.write_edgelist(G, 'grid'+str(N), delimiter=' ', data= False)