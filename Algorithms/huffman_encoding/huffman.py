import os
import sys
import marshal
import array
import pdb

try:
    import cPickle as pickle
except:
    import pickle

import heapq

#MY HELPER FUNCTIONS
#-----------------------------

    
#Node class for tree construction
class Node:
    def __init__(self, char, freq):
        self.char = char
        self.freq = freq
        self.left = None
        self.right = None


    # Define '<' for heapq to compare nodes based on frequency
    def __lt__(self, other):
        return self.freq < other.freq

#Get character freq from msg
def getFrequencies(msg):
    #takes in a message and returns a dictionary of frequencies
    freq = {}
    for char in msg:
        if char in freq:
            freq[char] += 1
        else:
            freq[char] = 1

    #store freq in a min heap
    heap = []
    for item in freq:
        #create a node for each char and freq
        node = Node(item, freq[item])
        heapq.heappush(heap, node)
        #print(len(heap))
    
    #print heap
    return heap


def construct_tree(heap):

    #while heap has more than one node
    while len(heap) > 1:
        #pop two smallest freq nodes
        left = heapq.heappop(heap)
        right = heapq.heappop(heap)
        #create a new node with the two nodes as children
        new_node = Node(None, left.freq + right.freq)

        #add it back to the min heap
        new_node.left = left
        new_node.right = right
        heapq.heappush(heap, new_node)

    return heap[0]


def traverse_tree(tree, code):
    #create dict that will store codes
    codes = {}

    if tree.char is not None:
        codes[tree.char] = code
    else:
        codes.update(traverse_tree(tree.left, code + "0"))
        codes.update(traverse_tree(tree.right, code + "1"))
    
    
    return codes;



#----------------------------

def code(msg):
    #take the file and turn it into it's ASCII represented coded variant
    #create a heap of frequencies
    freq = getFrequencies(msg)
    
    #construct the tree
    tree = construct_tree(freq)

    #traverse tree to give codes
    codes = traverse_tree(tree, "")

    # Convert codes to list of tuples for storage efficiency
    codes_list = [(k, codes[k]) for k in codes]

    #traverse message and encode
    encoded = ""
    for char in msg:
        #turn char into str
        if char not in codes:
            raise ValueError("Character", char, "not in tree")
        encoded += codes[char]

    return encoded, codes_list

def decode(msg, codes):
    # Reconstruct the codes dictionary: mapping code -> integer byte
    codes_dict = {char: code for char, code in codes}
    reverse_codes = {v: k for k, v in codes_dict.items()}

    decoded_bytes = bytearray()
    current = ""
    for bit in msg:
        current += bit
        if current in reverse_codes:
            # Append the integer value directly
            decoded_bytes.append(reverse_codes[current])
            current = ""
    return bytes(decoded_bytes)



num_of_padded_bits = 0

def compress(msg):
    compressed = array.array('B')
    encoded, codes = code(msg)
    
    # Calculate the number of padded bits
    num_of_padded_bits = (8 - len(encoded) % 8) % 8

    # Pad the encoded message with zeros
    encoded += '0' * num_of_padded_bits

    # Convert encoded bit string to bytes
    for i in range(0, len(encoded), 8):
        byte_str = encoded[i:i+8]
        byte = int(byte_str, 2)
        compressed.append(byte)

    # Return the padded bits count along with compressed data and codes
    return compressed, codes, num_of_padded_bits

def decompress(msg, codes, num_of_padded_bits):
    byteArray = array.array('B', msg)

    # Convert the byte array back to a bit string
    bits = ""
    for byte in byteArray:
        bits += format(byte, '08b')

    # Remove the padded bits using the provided count
    if num_of_padded_bits > 0:
        bits = bits[:-num_of_padded_bits]

    # Decode the bit string using your decode function
    decoded = decode(bits, codes)
    return decoded


def usage():
    sys.stderr.write("Usage: {} [-c|-d|-v|-w] infile outfile\n".format(sys.argv[0]))
    exit(1)

if __name__ == '__main__':
    usage = f'Usage: {sys.argv[0]} [ -c | -d | -v | -w ] infile outfile'
    if len(sys.argv) != 4:
        raise Exception(usage)

    operation = sys.argv[1]
    if operation not in {'-c', '-d', '-v', '-w'}:
        raise Exception(usage)

    infile, outfile = sys.argv[2], sys.argv[3]
    if not os.path.exists(infile):
        raise FileExistsError(f'{infile} does not exist.')

    if operation in {'-c', '-v'}:
        with open(infile, 'rb') as fp:
            _message = fp.read()

        if operation == '-c':
            _message, _decoder_ring, padded = compress(_message)
            # Store the padded bit count together with the other data
            with open(outfile, 'wb') as fp:
                marshal.dump((pickle.dumps((_decoder_ring, padded)), _message), fp)
        else:
            _message, _decoder_ring = code(_message)
            #print(_message)
            with open(outfile, 'wb') as fp:
                marshal.dump((pickle.dumps(_decoder_ring), _message), fp)
    else:
        with open(infile, 'rb') as fp:
            pickleRick, _message = marshal.load(fp)
                # Unpack the decoder ring and padded bit count
            try:
                _decoder_ring, padded = pickle.loads(pickleRick)
            except:
                _decoder_ring = pickle.loads(pickleRick)
                padded = 0

        if operation == '-d':
                bytes_message = decompress(array.array('B', _message), _decoder_ring, padded)
        else:
                bytes_message = decode(_message, _decoder_ring)
        with open(outfile, 'wb') as fp:
                fp.write(bytes_message)
