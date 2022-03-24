from datetime import datetime
import numpy
import sys
import random
from copy import deepcopy

visited_state = {}
state_parent = {}

def show_menu(items_list):
    for i in range(len(items_list)):
        print(f"{i+1}. {items_list[i]}")
    return

# convert string to matrix
def string2matrix(state_string):
    state_matrix = [[0 for i in range(3)] for j in range(3)]
    
    for i in range(3):
        for j in range(3):
            state_matrix[i][j] = int(state_string[3*i+j])
    
    return state_matrix

# print matrix
def print_matrix(matrix):
    for row in matrix:
        print(row)

class hill_climbing:
    def __init__(self, intial_state, goal_state):
        self.initial_configuration = intial_state
        self.final_configuration = goal_state
        self.state_explored = 0
        self.admissible = True

    def solve_eight_puzzle(self, current_state, heuristic_choice,depth = 0):
        stack = [current_state]
        print("\nSolving the puzzle...")
        print("Output being printed to output.txt...\n")
        while len(stack) != 0 :
            print(f"State {self.state_explored}:")
            print_matrix(string2matrix(current_state))
            print("-----------")

            current_state = stack.pop()

            if current_state == self.final_configuration: # We got the result!
                self.state_explored += 1
                print("Goal State Reached!!")
                out = output_result(self.initial_configuration, "output.txt", state_parent,
                                    self.state_explored)
                out.write_output_path(current_state, self.admissible)
                return 0
            else:
                self.state_explored += 1
                visited_state[current_state] = 1
                state = State(current_state, Heuristic(self.final_configuration).obtainHeuristicEstimation(
                    current_state, heuristic_choice
                ))
                neighbours = state.getAllSuccessor(heuristic_choice, self.final_configuration) # Get the successors.
                local_maxima = True
                options = []
                for neighbour in neighbours:
                    if neighbour.hvalue > state.hvalue or neighbour.puzzleState in visited_state:
                        continue
                    else:
                        local_maxima = False
                        options.append(neighbour)

                if local_maxima:
                    print("Oops! Stuck in local maxima")
                else:
                    sz = len(options)
                    idx = random.randrange(0, sz)
                    state_parent[options[idx].puzzleState] = current_state
                    stack.append(options[idx].puzzleState)

        return 1



#State imported
class State:
    def __init__(self, stateInfo, h=0):
        self.puzzleState = stateInfo
        self.hvalue = h

    def getAllSuccessor(self, heuristic_choice, final_configuration):
        x = [1, -1, 0, 0]
        y = [0, -0, 1, -1]
        
        puzzleMatrix = StringToMatrix(self.puzzleState)
        for i in range(3):
            for j in range(3):
                if puzzleMatrix[i][j] == 0:
                    blankX = i
                    blankY = j
                    break

        successorState = []
        for (xMove, yMove) in zip(x, y):
            if 0 <= blankX + xMove < 3 and 0 <= blankY + yMove < 3:
                successorPuzzleMat = deepcopy(puzzleMatrix)
                temp = successorPuzzleMat[blankX + xMove][blankY + yMove]
                successorPuzzleMat[blankX + xMove][blankY + yMove] = 0
                successorPuzzleMat[blankX][blankY] = temp
                new_state = MatrixToString(successorPuzzleMat)
                successorState.append(State(new_state, Heuristic(final_configuration).obtainHeuristicEstimation(new_state,heuristic_choice)))

        return successorState

    def __eq__(self, other):
        return self.puzzleState == other.puzzleState

    def __lt__(self, other):
        return self.hvalue < other.hvalue



# class to write output to file
class output_result:
    def __init__(self, start_state, file_name, parent_map,state_explored):
        self.start_state = start_state
        self.file_name = file_name
        self.parent = parent_map
        self.state_explored = state_explored
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
            f.write("Total Number of state explored : {}\n".format(str(self.state_explored)))
            f.write("Search Status : Successful\n(Sub) Optimal Path length: {} \n".format(str(self.path_length)))
            f.write("(Sub) Optimal Path \n")
            #f.write(puzzle_state[:3] + "\n" + puzzle_state[3:6] + "\n" + puzzle_state[6:] + "\n " + "\n")
            for i in range(len(puzzle_state)):
                f.write(puzzle_state[i])
                if i%3 == 2:
                    f.write("\n")
                else:
                    f.write(", ")
            f.write("\n")
            f.close()

        while len(stack)!=0 :
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


# Writes the info about the problem to the file.
def write_into_file(file, heuristic_choice, start_state, goal_state):
    file.write("Heuristic Chosen : ")
    if heuristic_choice == 1:
        file.write("number of tiles displaced from their destined position \n")
    elif heuristic_choice == 2:
        file.write("Total manhattan distance \n")
    else:
        file.write("displace tiles + manhatten distance\n")
    
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
    file.close()

# Utils imported
def StringToMatrix(state):
    puzzleMatrix = [[0 for i in range(3)] for j in range(3)]
    puzzleState = state
    for i in range(3):
        for j in range(3):
            puzzleMatrix[i][j] = int(puzzleState[i * 3 + j])
    return puzzleMatrix

def MatrixToString(puzzleMatrix):
    stringRep = ""
    for i in range(3):
        for j in range(3):
            stringRep += str(puzzleMatrix[i][j])
    return stringRep


# Specifies whether the blank tile is also considered a tile or not.
hasIncludeTile = False

# Python class for Heuristics
class Heuristic:
    def __init__(self, goalState="123456780"):
        self.goalState = goalState

    def tilesDisplacedHeuristic(self, state):
        currentPuzzleState = StringToMatrix(state)
        goalPuzzleState = StringToMatrix(self.goalState)
        h = 0
        for i in range(3):
            for j in range(3):
                if currentPuzzleState[i][j] != goalPuzzleState[i][j]:
                    h += 1
                if currentPuzzleState[i][j] == 0 and currentPuzzleState[i][j] != goalPuzzleState[i][j] and hasIncludeTile == False:
                    h -= 1
        return h

    def manhattanHeuristic(self, state):
        currentPuzzleState = StringToMatrix(state)
        goalPuzzleState = StringToMatrix(self.goalState)
        currentCoordinate = numpy.arange(18).reshape((9, 2))

        for i in range(3):
            for j in range(3):
                currentCoordinate[currentPuzzleState[i][j]][0] = i
                currentCoordinate[currentPuzzleState[i][j]][1] = j

        h = 0
        for i in range(3):
            for j in range(3):
                if goalPuzzleState[i][j] != 0:
                    h += abs(i - currentCoordinate[goalPuzzleState[i][j]][0]) + \
                         abs(j - currentCoordinate[goalPuzzleState[i][j]][1])
                if goalPuzzleState[i][j] == 0 and hasIncludeTile:
                    h += abs(i - currentCoordinate[goalPuzzleState[i][j]][0]) + \
                         abs(j - currentCoordinate[goalPuzzleState[i][j]][1])
        return h

    # Get the value of the heuristic estimation
    def obtainHeuristicEstimation(self, state, heur_choice):
        return {
            1: self.tilesDisplacedHeuristic(state),
            2: self.manhattanHeuristic(state),
            3: self.tilesDisplacedHeuristic(state) + self.manhattanHeuristic(state)
        }[heur_choice]

# show the menu
def startSearching(startState, goalState, file):
    # Menu
    print("Enter 1/2/3 to choose a heuristic :")
    show_menu([
        "h1(n) = Number of tiles displaced from their final position",
        "h2(n) = Total Manhattan distance",
        "h3​(n) = h1​(n) + h2(n)"
    ])

    # Input
    heur_choice = int(input("Enter your choice... "))
    write_into_file(file, heur_choice, startState, goalState)
    print("\nTo consider blank tile as another tile, enter 1. Otherwise, enter 0")
    tile_included = int(input("Enter your choice... "))
    if tile_included:
        Heuristic.hasIncludeTile = True

    # Solving the problem.
    start_time = datetime.now()
    solver_puzzle = hill_climbing(startState, goalState) # initializing the object
    status = solver_puzzle.solve_eight_puzzle(startState, heur_choice) # solving the puzzle
    file = open("output.txt", "a")
    if status == 1:
        file.write("Search Status : Failed\n")
    file.write("Time Taken : {} ".format(str(datetime.now() - start_time)))
    file.close()    


# Driver function to run the main program
if __name__ == '__main__':
    # Opens the output file in write mode
    file = open("output.txt", "w+")

    startState = ""
    # Convert the start state to string form for easy manipulation
    with open("start_state.txt") as f:
        for line in f:
            line = line.strip()
            line = line.replace(" ", "") # Preprocessing the file input
            startState += line

    startState = startState.replace("T", "").replace("B", "0")
    goalState = "123456780"

    # Execute the program
    startSearching(startState, goalState, file)
