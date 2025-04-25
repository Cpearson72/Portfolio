import imagematrix
import time


pixel_energies = {}

class SeamEnergyWithBackPointer():
    def __init__(self, energy, x_coordinate_in_previous_row=None):
        self.energy = energy
        self.x_coordinate_in_previous_row = x_coordinate_in_previous_row

def find_best_parent_node(self, nodes, pixel):
    #Grab cords for parents
    left_cord = (pixel[0] - 1, pixel[1] - 1) if pixel[0] > 0 else None
    middle_cord = (pixel[0], pixel[1] - 1)
    right_cord = (pixel[0] + 1, pixel[1] - 1) if pixel[0] < self.width - 1 else None


    #Find best energy seam
    #print(nodes)
    best_energy_seam = nodes[middle_cord]
    x_cords = middle_cord[0]

    if left_cord and nodes[left_cord].energy < best_energy_seam.energy:
        best_energy_seam = nodes[left_cord]
        x_cords = left_cord[0]
    
    if right_cord and nodes[right_cord].energy < best_energy_seam.energy:
        best_energy_seam = nodes[right_cord]
        x_cords = right_cord[0]

    #return node with lowest curr_energy_seam
    return best_energy_seam, x_cords


def best_seam_naive(self, i, j):

    #edge case. penalize path
    if i < 0 or i > self.width - 1:
        return float('inf'), []
    
    #base case. return energy
    if j == self.height - 1:
        return pixel_energies[(i, j)], [(i, j)]

    #recursive step
    else:
        #recursive call
        path_left = best_seam_naive(self, i-1, j+1) 
        path_middle = best_seam_naive(self, i, j+1)
        path_right = best_seam_naive(self, i+1, j+1)  

        #sort paths
        sorted_paths = sorted([path_left, path_middle, path_right], key=lambda x: x[0])
        min_path = sorted_paths[0]

        # Create new path info with min path
        # Fix: Convert min_path[1] to list if it's not already
        return_path = [(i, j)] + min_path[1]  # Changed sorted_paths[1] to min_path[1]
        return_energy = min_path[0] + pixel_energies[(i, j)]
        
        return [return_energy, return_path]
    

paths_with_energy = {}

def best_seam_dp(self, i, j):
    #Will store and update paths during iteration
    nodes = {}
    energy_matrix = {}

    #Store energies in matrix
    #print("Height: ", self.height, " Width: ", self.width)
    for col in range(self.width):
        for row in range(self.height):
            energy_matrix[(col, row)] = self.energy(col, row)
            #print("added cords ", (row, col), "with energy ", energy_matrix[(row, col)])

    #Start at second row. Each pixel will look up and try
    #to 'grab' the pixel above it and append to a list. If grab
    #is successful, 'remove' that above pixel from list of grabs.

    #set all first row pixels as nodes:
    for x in range(self.width):
        node = SeamEnergyWithBackPointer(energy_matrix[(x, 0)], None)
        #print("node ", (0, col), "with energy ", energy_matrix[(0, col)])
        nodes[(x, 0)] = node

    #i new row
    col = 1
    #loop until we reach past last row
    while (col != self.height):
        for row in range(self.width):
            #grab parent node
            parent_node, x_cord = find_best_parent_node(self, nodes, (row, col))

            #create new node
            new_node = SeamEnergyWithBackPointer(energy_matrix[(row, col)] + parent_node.energy, x_cord)

            #add new node to nodes
            nodes[(row, col)] = new_node

            #print(col)

        col += 1

    #now that we have the best energies for all nodes, we can find the best path

    return nodes


def return_seam(nodes, pixel):
    #base case
    if nodes[pixel].x_coordinate_in_previous_row == None:
        return [(pixel)]

    #recursive case
    return [(pixel)] + return_seam(nodes, (nodes[pixel].x_coordinate_in_previous_row, pixel[1] - 1))

def return_seam_energy(self, seam):
    energy = 0
    for pixel in seam:
        energy += self.energy(pixel[0], pixel[1])
    return energy





class ResizeableImage(imagematrix.ImageMatrix):
    def best_seam(self, dp=True):
        for col in range(self.width):
            for row in range(self.height):
                pixel_energies[(col, row)] = self.energy(col, row)
        #Dyanmic
        seams = {}
        if dp:
            start_time = time.time()
            nodes = best_seam_dp(self, 0, 0)
            row = self.height - 1
            for x in range(self.width - 1):
                seams[(x, row)] = return_seam(nodes, (x, row))
            end_time = time.time()
            elapsed_time = end_time - start_time
            with open("runtime_log_dp.txt", "a") as f:
                f.write(f"Elapsed time: {elapsed_time:.4f} seconds\n")

               
        #Naive
        else:
            start_time = time.time()
            for y in range(self.width - 1):
                [energy, path] = best_seam_naive(self, y, 0)
                seams[(y, 0)] = path
            end_time = time.time()
            elapsed_time = end_time - start_time
            with open("runtime_log_naive.txt", "a") as f:
                f.write(f"Elapsed time: {elapsed_time:.4f} seconds\n")

            #print(seams)

        
        
        if dp:
            lowest_energy = return_seam_energy(self, seams[(0,self.height - 1)])
            lowest_energy_seam = seams[(0,self.height - 1)]
        else:
            lowest_energy = return_seam_energy(self, seams[(0, 0)])
            lowest_energy_seam = seams[(0, 0)]

        for pixel in seams:
            energy = return_seam_energy(self, seams[pixel])
            if energy < lowest_energy:
                lowest_energy = energy
                lowest_energy_seam = seams[pixel]

        return lowest_energy_seam



    def remove_best_seam(self):

        self.remove_seam(self.best_seam())




