import csv
import networkx as nx
import matplotlib.pyplot as plt


def main():

	# load vertices
	vertices = {}
	with open('rrt.txt', 'r') as f:

		reader = csv.reader(f, delimiter=',')
		
		for row in reader:
			id_ = int(row[0])
			x = float(row[1])
			y = float(row[2])
			vertices[id_] = [x,y]

	# load connections
	connection = {}
	with open('rrt_connections.txt', 'r') as f:
		reader = csv.reader(f, delimiter=',')
		for row in reader:
			id_ = int(row[0])
			vertex_conn = [int(conn) for conn in row]
			connection[id_] = vertex_conn[1:]

	# create the graph
	g = nx.Graph()

	for item in connection:
		sv = item;
		cons = connection[item];

		for con in cons:
			g.add_edge(sv,con)

	print("Graph has {0} edges ".format(g.number_of_edges()))
	edges = g.edges()
	
	# view the first 5 edges 
	print(list(g.edges())[0:5])
	print("Graph has {0} nodes ".format(g.number_of_nodes()))


	node_pos = {}

	for node in g.nodes():
		node_pos[node] = (vertices[node][0], vertices[node][1]) 

	plt.figure(figsize=(8, 6))
	nx.draw(g, node_size=50, pos=node_pos)# edge_color=edge_colors, node_size=10, node_color='black')
	plt.title('Graph Representation of Sleeping Giant Trail Map', size=15)
	plt.show()
				

if __name__ == '__main__':
	main()
	
