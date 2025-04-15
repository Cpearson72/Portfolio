import math
import sys
from typing import List
from typing import Tuple

EPSILON = sys.float_info.epsilon
Point = Tuple[int, int]


def y_intercept(p1: Point, p2: Point, x: int) -> float:
    """
    Given two points, p1 and p2, an x coordinate from a vertical line,
    compute and return the the y-intercept of the line segment p1->p2
    with the vertical line passing through x.
    """
    x1, y1 = p1
    x2, y2 = p2
    slope = (y2 - y1) / (x2 - x1)
    return y1 + (x - x1) * slope


def triangle_area(a: Point, b: Point, c: Point) -> float:
    """
    Given three points a,b,c,
    computes and returns the area defined by the triangle a,b,c.
    Note that this area will be negative if a,b,c represents a clockwise sequence,
    positive if it is counter-clockwise,
    and zero if the points are collinear.
    """
    ax, ay = a
    bx, by = b
    cx, cy = c
    return ((cx - bx) * (by - ay) - (bx - ax) * (cy - by)) / 2


def is_clockwise(a: Point, b: Point, c: Point) -> bool:
    """
    Given three points a,b,c,
    returns True if and only if a,b,c represents a clockwise sequence
    (subject to floating-point precision)
    """
    return triangle_area(a, b, c) < -EPSILON


def is_counter_clockwise(a: Point, b: Point, c: Point) -> bool:
    """
    Given three points a,b,c,
    returns True if and only if a,b,c represents a counter-clockwise sequence
    (subject to floating-point precision)
    """
    return triangle_area(a, b, c) > EPSILON


def collinear(a: Point, b: Point, c: Point) -> bool:
    """
    Given three points a,b,c,
    returns True if and only if a,b,c are collinear
    (subject to floating-point precision)
    """
    return abs(triangle_area(a, b, c)) <= EPSILON


def clockwise_sort(points: List[Point]):
    """
    Given a list of points, sorts those points in clockwise order about their centroid.
    Note: this function modifies its argument.
    """
    # get mean x coord, mean y coord
    x_mean = sum(p[0] for p in points) / len(points)
    y_mean = sum(p[1] for p in points) / len(points)

    def angle(point: Point):
        return (math.atan2(point[1] - y_mean, point[0] - x_mean) + 2 * math.pi) % (2 * math.pi)

    points.sort(key=angle)
    return


def are_hull_points_collinear(hull_points):
    #see if all points have the same x value
    x_values = [p[0] for p in hull_points]

    #my program accounts for cases of the same y value implicitly so we only need to check for all the same x values, as that
    #is the only case of failure for how I compute the merge step.
    x = x_values[0]
    for i in range(1, len(x_values)):
        if x_values[i] != x:
            return False
        
    return True


def solve_three_base_case(points: List[Point]) -> List[Point]:
    #return points as hull
    clockwise_sort(points)
    return points;

def solve_four_base_case(points: List[Point]) -> List[Point]:
    #sort list by x value
    list_sorted_by_x = sorted(points, key=lambda p: p[0])

    #cut list in half
    left = list_sorted_by_x[:2]
    right = list_sorted_by_x[2:]

    solving_upper_tangent = True

    #set starting points
    most_right = left[1]
    most_left = right[0]

    #get highest y on either side
    left_sorted_by_y = sorted(left, key=lambda p: p[1])
    right_sorted_by_y = sorted(right, key=lambda p: p[1])

    lowest_y_on_left = left_sorted_by_y[0]
    lowest_y_on_right = right_sorted_by_y[0]

    while solving_upper_tangent == True:
        if(is_counter_clockwise(left[0], most_right, lowest_y_on_right) == True):
            #then most right is the upper tangent
            upper_left_tangent = left[0]
        else:
            upper_left_tangent = most_right
        if(is_counter_clockwise(right[1], lowest_y_on_left, most_left) == True):
            #then most left is the upper tangent
            upper_right_tangent = right[1]
        else:
            upper_right_tangent = most_left

        solving_upper_tangent = False
    


    highest_y_on_left = left_sorted_by_y[1];
    highest_y_on_right = right_sorted_by_y[1];
    
    solving_lower_tangent = True;
    while solving_lower_tangent == True:
        if(is_clockwise(left[0], most_right, highest_y_on_right) == True):
            #then most right is the loqwe tangent
            lower_left_tangent = left[0]
        else:
            lower_left_tangent = most_right
        if(is_clockwise(right[1], highest_y_on_left, most_left) == True):
            #then most left is the upper tangent
            lower_right_tangent = right[1]
        else:
            lower_right_tangent = most_left

        solving_lower_tangent = False

    hull = []

    ## in this case, the extra point that is neither will not be included, ie it is encapsulated in the hull.
    if(lower_left_tangent == upper_left_tangent):
        hull.append(lower_left_tangent)
        hull.append(upper_right_tangent)
        hull.append(lower_right_tangent)

    #same as above
    if(lower_right_tangent == upper_right_tangent):
        hull.append(lower_right_tangent)
        hull.append(upper_left_tangent)
        hull.append(lower_left_tangent)
    
    #if not, all points are included
    if(lower_left_tangent != upper_left_tangent and lower_right_tangent != upper_right_tangent):
        hull.append(lower_left_tangent)
        hull.append(upper_left_tangent)
        hull.append(upper_right_tangent)
        hull.append(lower_right_tangent)

    clockwise_sort(hull)

    return hull;


        

def base_case_hull(points: List[Point]) -> List[Point]:
    """ Base case of the recursive algorithm.
    """
    if len(points) <= 3:
        return solve_three_base_case(points)
    if len(points) == 4:
        return solve_four_base_case(points)


def compute_hull(points: List[Point]) -> List[Point]:
    """
    Given a list of points, computes the convex hull around those points
    and returns only the points that are on the hull.
    """
    # TODO: Implement a correct computation of the convex hull
    #  using the divide-and-conquer algorithm
    # TODO: Document your Initialization, Maintenance and Termination invariants.


    #Base Case
    if len(points) <= 4:
        return base_case_hull(points)
    
    #sort by x to split list in half, for the case of divide and conquer
    sorted_points_by_x = sorted(points, key=lambda p: p[0])
    
    #Divide and Conguer Steps
    mid = (len(points) + 1) // 2
    left = compute_hull(sorted_points_by_x[:mid])
    right = compute_hull(sorted_points_by_x[mid:])


    #Sort left and right lists into their own list sorted by x
    left_hull_by_x = sorted(left, key=lambda p: p[0])
    right_hull_by_x = sorted(right, key=lambda p: p[0])

    #Final flag to end merges
    is_merged = False

    #Final hull to store merged hull
    new_hull = []


    while is_merged == False:
        #A is right most point of left_hull_by x
        A = left_hull_by_x[-1]
        #find A index in left list
        A_index = left.index(A)
        #A_previous is the second to last point of left hull
        A_previous = left[(A_index - 1) % len(left)]
        
        
        #B is left most point of right hull by x list
        B = right_hull_by_x[0]
        #find B index in right list
        B_index = right.index(B)
        #B_previous is the second to last point of right hull
        B_previous = right[(B_index + 1) % len(right)]

        #TANGENT LINE LOGIC:

        #left sorted by y value
        left_sorted_by_y = sorted(left, key=lambda p: p[1])
        right_sorted_by_y = sorted(right, key=lambda p: p[1])

        solved_upper_right = False
        solved_upper_left = False


        #In case of edge case where entire hull has the same x-value, we use extremes as the upper tangents.
        if are_hull_points_collinear(left):
            upper_tangent_A = left_sorted_by_y[0]
            solved_upper_left = True
        if are_hull_points_collinear(right):
            upper_tangent_B = right_sorted_by_y[0]
            solved_upper_right = True


        still_solving_upper_tangent = True
        while still_solving_upper_tangent == True:
            if is_clockwise(A_previous, A, B) == False:
                #If A_previous is clockwise of A and B, then A counter must be updated to move
                #the two points back as A is below the tangent line of A_previous and B
                A_index = (A_index - 1) % len(left)
                A = left[A_index]
                A_previous = left[(A_index - 1) % len(left)]
            
            #If not collinear, then we set A as the upper tangent.
            elif solved_upper_left == False:
                solved_upper_left = True;
                upper_tangent_A = A


            if is_counter_clockwise(B, B_previous, A) == True:
                #If B is counter clockwise of B_previous and A, then B counter must be updated to move
                #the two points back as B is above the tangent line of B_previous and A
                B_index = (B_index + 1) % len(right)
                B = right[B_index]
                B_previous = right[(B_index + 1) % len(right)]
            
            #If not collinear, we set B as the upper tangent.
            elif solved_upper_right == False:
                solved_upper_right = True;
                upper_tangent_B = B

            #if neither B_counter or A_counter is updated, then the tangent line has been found
            if solved_upper_right == True and solved_upper_left == True:
                still_solving_upper_tangent = False

        

        #SAME EXACT PROCESS, BUT FOR LOWER TANGENTS
        #Set forward points that traverse "downward" the hull
        A = left_hull_by_x[-1]
        #Set A_index in left hull
        A_index = left.index(A)

        A_forward = left[(A_index + 1) % len(left)]


        B = right_hull_by_x[0]
        #Set B_index in right hull
        B_index = right.index(B)
        B_forward = right[(B_index - 1) % len(right)]


        still_solving_lower_tangent = True

        solved_lower_left = False
        solved_lower_right = False

        lower_tangent_A = None
        lower_tangent_B = None

        #Collienar edge case
        if are_hull_points_collinear(left):
            lower_tangent_A = left_sorted_by_y[-1]
            solved_lower_left = True
        if are_hull_points_collinear(right):
            lower_tangent_B = right_sorted_by_y[-1]
            solved_lower_right = True

        
        while still_solving_lower_tangent == True:
            if is_counter_clockwise(A, A_forward, B) == True:
                #If A is counter clockwise of A_forward and B, then A counter must be updated to move
                #the two points forward as A is above the tangent line of A_forward and B
                A_index = (A_index + 1) % len(left)
                A = left[A_index]
                A_forward = left[(A_index + 1) % len(left)]

            elif solved_lower_left == False:
                solved_lower_left = True;
                lower_tangent_A = A
                
            if is_clockwise(B_forward, B, A) == False:
                #If B_forward is clockwise of B and A, then B counter must be updated to move
                #the two points forward as B is below the tangent line of B_forward and A
                B_index = (B_index - 1) % len(right)
                B = right[B_index]
                B_forward = right[(B_index - 1) % len(right)]
                
            elif solved_lower_right == False:
                solved_lower_right = True;
                lower_tangent_B = B
            
            if solved_lower_right and solved_lower_left:
                still_solving_lower_tangent = False
            



        #Now we have the upper and lower tangents from the left and right hulls. Now
        #we merge the hulls together, remembering that the hull list is sorted clockwise
        #so the points are in the correct order

        
        

        lower_tangent_A_index = left.index(lower_tangent_A)
        upper_tangent_A_index = left.index(upper_tangent_A)

        new_hull = []

        # Add points from lower_tangent_A to upper_tangent_A
        current_index = lower_tangent_A_index
        while True:
            new_hull.append(left[current_index])
            # Stop when reaching upper_tangent_A
            if left[current_index] == upper_tangent_A:  
                break
            # Move forward
            current_index = (current_index + 1) % len(left)  

        # Merge right hull into new_hull
        lower_tangent_B_index = right.index(lower_tangent_B)
        upper_tangent_B_index = right.index(upper_tangent_B)

        current_index = lower_tangent_B_index
        while True:
            new_hull.append(right[current_index])
            # Stop when reaching upper_tangent_B
            if right[current_index] == upper_tangent_B:  
                break
            # Move backward
            current_index = (current_index - 1) % len(right)  

        is_merged = True

        # Ensure the merged hull is in clockwise order
        clockwise_sort(new_hull)



   

    
    return new_hull
