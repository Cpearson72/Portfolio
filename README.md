# Portfolio

## 📌 Overview

This portfolio consists of C, C++, and Python projects that showcase my understanding of:

- Low-level coding concepts: memory management, pointer arithmetic, and multi-threading.
- Higher-level concepts: algorithms, problem decomposition, and complex data structures.

I’ve selected a few key projects to highlight these concepts:

1. **Snake** – Demonstrates memory management in C.
2. **Inklings** – A multi-threaded simulation using mutexes.
3. **Convex Hull**, **Image Seam Carving**, and **Compression** – Show my ability to decompose complex problems into recursive subproblems.

Each project includes a short overview, the skills demonstrated, and clear compilation instructions.

---

## 🐍 Snake

### 🔹 Overview

**Snake** is a fully functional recreation of the classic Snake game, written in C. It features:

- A customizable board using RLE encoding.
- Dynamic memory allocation and pointer arithmetic for game state updates.
- Robust memory management to avoid leaks, double frees, and use-after-free errors.

For example, one edge case handled is freeing the snake's tail memory correctly after an apple is consumed, ensuring the linked list isn't corrupted.

### 🔹 Skills Demonstrated

- Dynamic memory allocation (`malloc`, `free`)
- Edge case handling
- Debugging memory leaks and segmentation faults

### 🔹 How to Compile

```bash
make

## 🎨 Inklings

### 🔹 Overview

**Inklings** is a multi-threaded C application that simulates autonomous agents—called "inklings"—moving on a grid. Each inkling:

- Is assigned a color: **Red**, **Green**, or **Blue**
- Competes for a shared, limited resource of their respective color
- Operates concurrently with other inklings via **POSIX threads**

Thread synchronization was the primary challenge. Since multiple threads (inklings) attempt to access the same color resource pool, I implemented mutex locks to manage access. Careful design was needed to prevent race conditions and **deadlocks**, especially as mutex usage grew more complex.

### 🔹 Skills Demonstrated

- Thread creation using `pthread`
- Mutex-based synchronization for shared resource management
- Deadlock avoidance and thread-safe programming
- Debugging concurrency issues in a multi-threaded environment

### 🔹 How to Compile

To compile the project, use:

```bash
make

