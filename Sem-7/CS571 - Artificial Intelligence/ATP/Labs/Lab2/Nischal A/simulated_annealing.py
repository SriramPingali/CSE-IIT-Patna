import numpy as np
from math import exp
import io
from copy import deepcopy
import random
from datetime import datetime
import sys

# convert string to matrix
def string2matrix(state_string):
    state_matrix = [[0 for i in range(3)] for j in range(3)]
    
    for i in range(3):
        for j in range(3):
            state_matrix[i][j] = int(state_string[3*i+j])
    
    return state_matrix

def matrix2string(matrix):
    return ''.join(map(str,np.array(matrix).reshape(-1)))

def print_matrix(matrix):
    for row in matrix:
        print(row)

def show_menu(items_list):
    for i in range(len(items_list)):
        print(f"{i+1}. {items_list[i]}")
    return

# python class to represent state of the 8-puzzle
class StateClass:
    # constructor
    def __init__(self, state_info, h=0):
        self.cur_state = state_info
        self.hvalue = h

    # find location of blank in the puzzle
    def find_blank(self, matrix):
        blankX, blankY = -1, -1
        for i in range(3):
            for j in range(3):
                if matrix[i][j] == 0:
                    blankX = i
                    blankY = j
                    break
        return blankX, blankY

    
    # calculate all possible next states from current states by making move in each of the direction (up/right/down/left)
    def calc_all_next_states(self, heur_choice, final_config):
        movesX = [1, -1, 0, 0]
        movesY = [0, 0, 1, -1]
        
        matrix = string2matrix(self.cur_state)
        blankX, blankY = self.find_blank(matrix)

        next_state = []
        for (del_x, del_y) in zip(movesX, movesY):
            if 0 <= blankX + del_x < 3 and 0 <= blankY + del_y < 3:
                next_mat = deepcopy(matrix)

                temp = next_mat[blankX+del_x][blankY+del_y]
                next_mat[blankX+del_x][blankY+del_y] = 0
                next_mat[blankX][blankY] = temp
                new_state = matrix2string(next_mat)
                next_state.append(StateClass(new_state, Heuristic(final_config).get_heur_est(new_state,heur_choice)))

        return next_state

    # magic function to check equality between different StateClass objects
    def __eq__(self, other):
        return self.cur_state == other.cur_state

    # magic function to compare between different StateClass objects
    def __lt__(self, other):
        return self.hvalue < other.hvalue


# Specifies whether the blank tile is also considered a tile or not.
is_tile_include = False

class Heuristic:
    def __init__(self, goal_state="123456780"):
        self.goal_state = goal_state

    def tilesDisplacedHeuristic(self, state):
        cur_state = string2matrix(state)
        goal_state = string2matrix(self.goal_state)
        h = 0
        for i in range(3):
            for j in range(3):
                if cur_state[i][j] != goal_state[i][j]:
                    h += 1
                if cur_state[i][j] == 0 and cur_state[i][j] != goal_state[i][j] and is_tile_include == False:
                    h -= 1
        return h

    def manhattanHeuristic(self, state):
        cur_state = string2matrix(state)
        goal_state = string2matrix(self.goal_state)
        cur_coords = np.arange(18).reshape((9, 2))

        for i in range(3):
            for j in range(3):
                cur_coords[cur_state[i][j]][0] = i
                cur_coords[cur_state[i][j]][1] = j

        h = 0
        for i in range(3):
            for j in range(3):
                if goal_state[i][j] != 0:
                    h += abs(i - cur_coords[goal_state[i][j]][0]) + abs(j - cur_coords[goal_state[i][j]][1])
                if goal_state[i][j] == 0 and is_tile_include:
                    h += abs(i - cur_coords[goal_state[i][j]][0]) + abs(j - cur_coords[goal_state[i][j]][1])
        return h

    def get_heur_est(self, state, HeuristicChoice):
        return {
            1: self.tilesDisplacedHeuristic(state),
            2: self.manhattanHeuristic(state),
            3: 3*self.tilesDisplacedHeuristic(state) - 2*self.manhattanHeuristic(state),
            4: 2*self.tilesDisplacedHeuristic(state) * self.manhattanHeuristic(state)
        }[HeuristicChoice]


# keep track of visited states
state_visited = {}
# keep track of parent state
parent_state = {}

# python class to implement simulated 
class simulated_annealing:
    def __init__(self, intial_state, goal_state, temp):
        self.temp = temp
        self.max_temp = temp
        self.initial_config = intial_state
        self.final_config = goal_state
        self.step = 0.0000001
        self.x = -temp / 5
        self.states_explored = 0
        self.is_admissible = True
        sys.setrecursionlimit(181440)

    def energy_difference(self, cur, new):
        return (new.hvalue-cur.hvalue)

    def cooling_temp(self, choice):
        return {
            1: self.linear_strategy(),
            2: self.random_strategy(),
            3: self.negative_exponential(),
            4: self.positive_exponential(),
        }[choice]

    def puzzle_solver(self, cur_state, heur_choice, cooling_choice):
        stack = [cur_state]
        print("\nSolving the puzzle...")
        print("Output being printed to simulated_annealing_output.txt...\n")
        while len(stack) != 0:
            cur_state = stack.pop()
            
            print(f"State {self.states_explored}:")
            print_matrix(string2matrix(cur_state))
            print("-----------")
            
            if cur_state == self.final_config:
                self.states_explored += 1
                out = output_result(self.initial_config, "simulated_annealing_output.txt", parent_state,
                                    self.states_explored)
                out.write_output_path(cur_state, self.is_admissible)
                print("Goal State Reached!!")
                return 0
            elif cur_state in state_visited:
                continue
            else:
                self.cooling_temp(cooling_choice)
                self.states_explored += 1
                state_visited[cur_state] = 1
                state = StateClass(cur_state, Heuristic(self.final_config).get_heur_est(cur_state, heur_choice))
                neighbours = state.calc_all_next_states(heur_choice, self.final_config)
                neighbours.sort()
                idx, cur = 0, 0
                sz = len(neighbours)
                li = []
                mark = [0]*sz
                cnt = 0
                while cnt < len(neighbours):
                    h = Heuristic(self.final_config).get_heur_est(neighbours[cur].cur_state, heur_choice)
                    if(state.hvalue > h + 1): # Monotonicity implies admissibility
                        self.is_admissible = False
                    
                    e = self.energy_difference(state, neighbours[cur])
                    if mark[cur] == 1 :
                        cur = (cur + 1) % sz
                        continue
                    if neighbours[cur].cur_state in state_visited :
                        mark[cur] = 1
                        cnt += 1
                    elif e <= 0:
                        mark[cur] = 1
                        cnt += 1
                        parent_state[neighbours[cur].cur_state] = cur_state
                        li.append(neighbours[cur].cur_state)
                    elif exp(-e / self.temp) < random.uniform(0, 1):
                        mark[cur] = 1
                        cnt += 1
                        parent_state[neighbours[cur].cur_state] = cur_state
                        li.append(neighbours[cur].cur_state)
                    cur = (cur+1)%sz
                li.reverse()
                stack.extend(li)

        return 1

    def linear_strategy(self):
        self.temp = abs(self.max_temp + self.x)
        self.x += self.step

    def random_strategy(self):
        self.temp = random.uniform(0, 1) * abs(self.max_temp + self.x)
        self.x += self.step

    def negative_exponential(self):
        self.temp = exp(-1 * self.x) * self.max_temp
        self.x += self.step

    def positive_exponential(self):
        self.temp = exp(self.x) * self.max_temp
        self.x += self.step


#output_util imported
class output_result:
    def __init__(self, start_state, file_name, parent_map,states_explored):
        self.start_state = start_state
        self.file_name = file_name
        self.parent = parent_map
        self.states_explored = states_explored
        self.path_length = 0
        sys.setrecursionlimit(181440)

    def write_output_path(self, puzzle_state, admissibilty):
        stack = [puzzle_state]

        while puzzle_state != self.start_state:
            stack.append(self.parent[puzzle_state])
            puzzle_state = self.parent[puzzle_state]
            self.path_length += 1
        stack.pop()
        with open(self.file_name, "a") as f:
            if admissibilty:
                f.write("The chosen heuristic is admissible!\n")
            else:
                f.write("The chosen heuristic is not admissible!\n")
            
            f.write("Total Number of state explored : {}\n".format(str(self.states_explored)))
            f.write("Search Status : Successful\n(Sub) Optimal Path length: {} \n".format(str(self.path_length)))
            f.write("(Sub) Optimal Path \n")
            for i in range(len(puzzle_state)):
                f.write(puzzle_state[i])
                if i%3 == 2:
                    f.write("\n")
                else:
                    f.write(", ")
            f.write("\n")
            # f.write(puzzle_state[:3] + "\n" + puzzle_state[3:6] + "\n" + puzzle_state[6:] + "\n " + "\n")
            f.close()

        while len(stack)!=0:
            puzzle_state = stack.pop()
            with open(self.file_name, "a") as f:
                for i in range(len(puzzle_state)):
                    f.write(puzzle_state[i])
                    if i%3 == 2:
                        f.write("\n")
                    else:
                        f.write(", ")
                f.write("\n")
                f.close()




# Write output to the file
def output_fn(file, heur_choice, start_state, goal_state, cooling_function, temp):
    file.write("Chosen Heuristic: ")
    if heur_choice == 1:
        file.write("Number of tiles displaced from their final position \n")
    elif heur_choice == 2:
        file.write("Total Manhattan distance \n")
    else:
        file.write("Total Manhattan distance * Number of tiles displaced from their final position\n")
    
    file.write("Start State: \n")
    for i in range(len(start_state)):
        file.write(start_state[i])
        if i%3 == 2:
            file.write("\n")
        else:
            file.write(", ")
    file.write("\n")
    
    file.write("Goal State: \n")
    for i in range(len(goal_state)):
        file.write(goal_state[i])
        if i%3 == 2:
            file.write("\n")
        else:
            file.write(", ")
    file.write("\n")
    
    
    if cooling_function == 1:
        file.write("Cooling Function : Linear Function \n")
    elif cooling_function == 2:
        file.write("Cooling Function : Random Strategy \n")
    elif cooling_function == 3:
        file.write("Cooling Function : Negative Exponential Function \n")
    else:
        file.write("Cooling Function : Positive Exponential function \n")
    
    file.write("TMAX : {}\n".format(str(temp)))
    file.close()


# Show the menu
def start(startState, goal_state, file):
    print("Enter 1/2/3 to choose a heuristic :")
    show_menu([
        "h1(n) = Number of tiles displaced from their final position",
        "h2(n) = Total Manhattan distance",
        "h3​(n) = h1​(n) * h2(n)"
    ])
    heur_choice = int(input("Enter your choice... "))
    if heur_choice == 4:
    	heur_choice = 3

    print("\nTo consider blank tile as another tile, enter 1. Otherwise, enter 0")
    is_blank_tile_included = int(input("Enter your choice... "))
    if is_blank_tile_included:
        Heuristic.is_tile_include = True
    
    print("\nEnter 1/2/3/4 to choose a cooling function:")
    show_menu([
        "Linear Function",
        "Random Strategy",
        "Negative Exponential Function",
        "Positive Exponential function"
    ])
    cooling_function_choice = int(input("Enter your choice... "))

    initial_temp = int(input("\nEnter the value of TMAX... "))

    output_fn(file, heur_choice, startState, goal_state, cooling_function_choice, initial_temp)

    start_time = datetime.now()
    puzzle_solver = simulated_annealing(startState, goal_state, initial_temp)
    search_status = puzzle_solver.puzzle_solver(startState, heur_choice, cooling_function_choice)
    file = open("simulated_annealing_output.txt", "a")
    if search_status == 1:
        file.write("Search Status : Failed\n")
    file.write("Time Taken : {} ".format(str(datetime.now() - start_time)))
    file.close()

# Driver function to run the main program
if __name__ == '__main__':
    startState = ""
    goal_state = ""

    # Opens the output file in write mode
    file = open("simulated_annealing_output.txt", "w+")

    # Convert the start state to string form for easy manipulation
    with open("StartState.txt") as f:
        for line in f:
            line = line.strip()
            line = line.replace(" ", "")
            startState += line

    # Convert the goal state to string form for easy manipulation
    with open("GoalState.txt") as f:
        for line in f:
            line = line.strip()
            line = line.replace(" ", "")
            goal_state += line

    startState = startState.replace("T", "").replace("B", "0")
    goal_state = goal_state.replace("T", "").replace("B", "0")

    # Execute the program
    start(startState, goal_state, file)
