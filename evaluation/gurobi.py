import gurobipy as gp

# Create the model
model = gp.Model("Path")

# Receive number of vertices, edges and paths
n, m, p = map(int, input().split())

# Receive the edges
edges = list()
for i in range(m):
    a, b = map(int, input().split())
    edges.append([a, b])

# Receive the paths and create the variable 
paths = list()
variables = list()
for i in range(p):
    sz = int(input())
    paths.append(list(map(int, input().split())))
    variables.append(model.addVar(vtype=gp.GRB.BINARY))

model.setObjective(
    gp.quicksum(variables[i] for i in range(p)), sense=gp.GRB.MINIMIZE)

# Generate the restrictions for each edge
for i in range(m):
    constraint = list()
    for j in range(p):
        contains = False
        for k in range(len(paths[j]) - 1):
            if (edges[i][0] == paths[j][k] and edges[i][1] == paths[j][k + 1]) or (edges[i][0] == paths[j][k + 1] and edges[i][1] == paths[j][k]):
                contains = True
        if contains:
            constraint.append(variables[j])
    model.addConstr(
        gp.quicksum(constraint[j] for j in range(len(constraint))) == 1)

# Solve the model
model.optimize()

# Print which paths were used
# for i in range(p):
#     if variables[i].X == 1:
#         print("Caminho", paths[i], "foi utilizado!")
#     else: 
#         print("Caminho", i, "não foi utilizado!")