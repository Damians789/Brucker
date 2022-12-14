from graphviz import Digraph

g = Digraph("Krytyczna_sciezka")
g.body.extend(['rankdir=LR', 'size="8,5"'])
g.attr('node', shape='box')
no_tasks = int(input())
edge_list = []
node_list = []

for i in range(no_tasks+2):
	temp = []
	[a,b] = str(input()).split(' ')
	temp.append(a)
	g.node(str(a),str(a))
	temp.append(b)
	node_list.append(temp)

while True:
	temp = []
	k = str(input())
	if(k=='quit'):
		break
	[a,b] = k.split(' ')	
	temp.append(a)
	temp.append(b)
	edge_list.append(temp)

temp = str(input()).strip()
temp = temp.split(' ')
comp = []
print(temp)
for x in range(1,len(temp)):
	g.edge(node_list[int(temp[x-1])][0],node_list[int(temp[x])][0], color='red')
	te2 = []
	te2.append(temp[x-1])
	te2.append(temp[x])
	comp.append(te2)

for x in edge_list:
	if x not in comp: 
		[a,b] = x
		g.edge(node_list[int(a)][0],node_list[int(b)][0])

g.view()