# Import required libraries
from heapq import heappush, heappop
from math import sqrt
import time
import os


# StateClass: Representation of State as Python Class
class StateClass:
    """
    constructor of StateClass
    parameters:
        state - current state
        prev - previous state
        move - current move
        depth - depth searched
        cost - cost of the path
        hval - heuristic value
    """
    def __init__(self, state, prev, move, depth, cost, hval):
        self.state = state
        self.prev = prev
        self.move = move
        self.depth = depth
        self.cost = cost
        self.hval = hval

        if self.state:
            self.map = ''.join(str(e) for e in self.state)
    
    # magic function for equality comparision operator
    def __eq__(self, other):
        return self.map == other.map
    
    # method for less than comparision operator
    def __lt__(self, other):
        return self.map < other.map


# Initialization
start_state = []
goal_state = []

goal_node = StateClass

len_puzzle = 0       
side_puzzle = 0      

count_explored_nodes = 0   
max_depth_reached = 0 
max_frontier_size = 0   
moves = []
optimal_path = []


"""Directory containing input files StatrtState.txt and GoalState.txt"""
d = os.getcwd()

# Read Start State and Goal State from input files
def getStartGoal():
    start = None
    goal = None

    for filename in os.listdir(d):
        if not filename.endswith('StartState.txt'):
            continue      
        with open(filename, 'r') as f:
            start = f.read()
            f.close()
    
    for filename in os.listdir(d):
        if not filename.endswith('GoalState.txt'):
            continue
        with open(filename, 'r') as f:
            goal=f.read()
            f.close()
    
    if start == None:
        print('StartState.txt is not found in current directory')
    
    if goal == None:
        print('GoalState.txt is not found in current directory')

    return start, goal


# Storing start_state and goal_state as a list
def extractStartGoal(start_string, goal_string):
    global len_puzzle, side_puzzle

    for element in start_string.split(','):
        start_state.append(int(element))
    
    for element in goal_string.split(','):
        goal_state.append(int(element))   
    
    len_puzzle = len(start_state)                  
    side_puzzle = int(sqrt(len_puzzle))


# Function for BFS Algorithm
def bfs(start_state, h_fun):
    global goal_node, max_frontier_size, max_depth_reached

    visited = set()
    pq = list()

    hval = h_fun(start_state)
    root = StateClass(start_state, None, None, 0, 0, hval)
    entry = (hval, 0, root)
    heappush(pq, entry)
    while pq:
        node = heappop(pq)
        visited.add(node[2].map)
        if node[2].state == goal_state:
            goal_node = node[2]
            return True, pq
        
        neighbors = generate_child(node[2])
        for neighbor in neighbors:
            # Calculating  f(n) = h(n)
            neighbor.hval = h_fun(neighbor.state)
            entry = (neighbor.hval, neighbor.move, neighbor)
            if neighbor.map not in visited:
                heappush(pq, entry)
                visited.add(neighbor.map)
                if neighbor.depth > max_depth_reached:
                    max_depth_reached += 1                    
        
        if len(pq) > max_frontier_size:
            max_frontier_size = len(pq)   
    
    return False, None


#  h1 : number of tiles displaced from their destined position.
def h1(state):
    count = 0
    for i in range(0,len_puzzle):
        try:
            if not (state.index(i) == goal_state.index(i)) : 
                count+=1
        except:
            print(i, len_puzzle)
            pass
    #print(count)
    return count


# h2 : sum of Manhattan distance of each tile from the goal position
def h2(state):
    return sum(abs(b % side_puzzle - g % side_puzzle) + abs(b//side_puzzle - g//side_puzzle)
               for b, g in ((state.index(i), goal_state.index(i)) 
               for i in range(1, len_puzzle)))


# Generate valid child nodes from the given node by moving the blank space 
# (represented by 0 in this program) either in the four directions {DOWN,LEFT,RIGHT,UP}
def generate_child(node):
    global count_explored_nodes
    count_explored_nodes += 1
    
    childNodes = []
    childNodes.append(StateClass(valid_move(node.state,'DOWN'),node,'DOWN',node.depth + 1, node.cost + 1, 0)) 
    childNodes.append(StateClass(valid_move(node.state,'LEFT'),node,'LOW',node.depth + 1, node.cost + 1, 0)) 
    childNodes.append(StateClass(valid_move(node.state,'RIGHT'),node,'RIGHT',node.depth + 1, node.cost + 1, 0))
    childNodes.append(StateClass(valid_move(node.state,'UP'),node,'UP',node.depth + 1, node.cost + 1, 0))
    
    nodes = [child for child in childNodes if child.state]
    return nodes


# Verify next move
def valid_move(state, position):
    newState = state[:]
    index = newState.index(0) 
    if position == 'UP': 
        if index not in range(0, side_puzzle):           
            temp = newState[index - side_puzzle]
            newState[index - side_puzzle] = newState[index]
            newState[index] = temp
            return newState
        else:
            return None
    if position == 'DOWN':        
        if index not in range(len_puzzle - side_puzzle, len_puzzle):
            temp = newState[index + side_puzzle]
            newState[index + side_puzzle] = newState[index]
            newState[index] = temp
            return newState
        else:
            return None
    if position == 'LEFT': 
        if index not in range(0, len_puzzle, side_puzzle):
            temp = newState[index - 1]
            newState[index - 1] = newState[index]
            newState[index] = temp
            return newState
        else:
            return None
    if position == 'RIGHT': 
        if index not in range(side_puzzle - 1, len_puzzle, side_puzzle):
            temp = newState[index + 1]
            newState[index + 1] = newState[index]
            newState[index] = temp
            return newState
        else:
            return None


def backtrack():
    cur_node = goal_node
    while start_state != cur_node.state : 
        moves.insert(0, cur_node.move)
        optimal_path.insert(0, cur_node.state)
        cur_node = cur_node.prev
    
    return moves

def get_unique(optimal_path):
    new_optimal = []
    for ele in optimal_path:
        if ele not in new_optimal:
            new_optimal.append(ele)
    return new_optimal

def result(frontier, time, heuristic_name):
    if frontier:
        global moves
        global optimal_path

        moves = backtrack()
        optimal_path = get_unique(optimal_path)
        moves_new = []
        for i in range(len(optimal_path)):
            moves_new.append(moves[i])
        moves = moves_new
        print("\nPUZZLE SOLVED SUCCESSFULLY")
        print("Heurestic used", heuristic_name)
        print("\nStart State : "+str(start_state))
        print("\nGoal State : "+str(goal_state))
        print("\nTotal no. of states expanded: " + str(count_explored_nodes))
        print("\nTotal no. of states to optimal path : " + str(len(moves)+1))
        print("\nMoves to goal : " + str(moves))
        print("\nOptimal Path : " + str(optimal_path))
        print("\nOptimal Path Cost: " + str(len(moves)))
        print("\nRunning Time: " + format(time, '.8f'))
    else:
        print("\nPUZZLE NOT SOLVED")
        print("Heurestic used", heuristic_name)
        print("\nStart State : "+str(start_state))
        print("\nGoal State : "+str(goal_state))
        print("\nTotal no. of states expanded before termination: " + str(count_explored_nodes))
        print("\nDepth searched before termination: " + str(max_depth_reached))
        print("\nRunning Time: " + format(time, '.8f'))


def output(frontier,time, heuristic_name):
    filename=d+os.sep+'BFS_Output_8Puzzle'+'_'+heuristic_name+'.txt'
    if frontier:
        global moves
        global optimal_path
        moves = backtrack()
        optimal_path = get_unique(optimal_path)
        moves_new = []
        for i in range(len(optimal_path)):
            moves_new.append(moves[i])
        moves = moves_new
        with open(filename, 'w') as f:
            print("\nWriting output to file "+filename+"......")
            f.write("\n\n-----Output of Best First Search-----")
            to_write = "\n\n-----Using " + heuristic_name + " Heurestic---"
            f.write(to_write)
            f.write("\nPUZZLE SOLVED SUCCESSFULLY")
            f.write("\nStart State : "+str(start_state))
            f.write("\nGoal State : "+str(goal_state))
            f.write("\nTotal no. of states expanded: " + str(count_explored_nodes))
            f.write("\nTotal no. of states to optimal path : " + str(len(moves)+1))
            f.write("\nMoves to goal : " + str(moves))
            f.write("\nOptimal Path : " + str(optimal_path))
            f.write("\nOptimal Path Cost: " + str(len(moves)))
            f.write("\nRunning Time: " + format(time, '.8f'))
            f.close()
            print("\nExport complete.")
    else:
        with open(filename, 'w') as f:
            print("\nWriting output to file "+filename+"......")
            f.write("\n\n-----Output of Best First Search-----")
            to_write = "\n\n-----Using " + heuristic_name + " Heurestic---"
            f.write(to_write)
            f.write("\nPUZZLE NOT SOLVED")
            f.write("\nStart State : "+str(start_state))
            f.write("\nGoal State : "+str(goal_state))
            f.write("\nTotal no. of states expanded before termination : " + str(count_explored_nodes))
            f.write("\nTotal no. of states to optimal path : " + str(len(moves)+1))
            f.write("\nDepth searched before termination: " + str(max_depth_reached))
            f.write("\nRunning Time: " + format(time, '.8f'))
            f.close()
            print("\nExport complete.")


# %%
def main():
    print("Best First Search Algorithm for 8-Puzzle Problem")
    print("Press 1 for Hamming Distance heurestic")
    print("Press 2 for Manhattan Distnace heurestic")
    her_inp = str(input())
    # set to either h1: Hamming Distance, or h2: Manhattan Distance
    if her_inp == '1':
        heuristic_to_use = 'h1' 
    elif her_inp == '2':
        heuristic_to_use = 'h2'
    h_fun = None
    function_map = {'bfs' : bfs}
    heuristic_name = ''

    if heuristic_to_use == 'h1':
        heuristic_map = {'hn' : h1,}
        heuristic_name = 'Hamming_Distance'
    elif heuristic_to_use == 'h2':
        heuristic_map = {'hn' : h2,}
        heuristic_name = 'Manhattan_Distance'
    
    algorithm = 'bfs'
    heuristic = 'hn'
    s,g = getStartGoal() 
    extractStartGoal(s,g)
    function = function_map[algorithm]
    h_fun = heuristic_map[heuristic]
    
    start = time.time()       
    search, frontier = function(start_state, h_fun)  
    stop = time.time()
    
    if not search : 
        result(frontier, stop-start, heuristic_name)
        output(frontier, stop-start, heuristic_name)
    else : 
        result(frontier, stop-start, heuristic_name)
        output(frontier, stop-start, heuristic_name)


# %%
if __name__ == '__main__':
    main()


