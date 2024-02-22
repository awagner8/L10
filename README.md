# Lab 10 -- Project Alloc Warmup

## Overview

This lab gives you a little exposure to writing a fast memory allocator for a
known specific data structure by maintaining your own memory with an associated
free list and calling the system `malloc` only when necessary.

You are to modify the file `node_alloc.c` and complete the two functions
`get_node` and `free_node` as follows.

---

### `get_node`

This function returns the first available node in `free_list`. If the list is
empty, the function should obtain memory using `malloc` by allocating
sufficient space for `NUM_ALLOC` nodes in the list. 

After a `malloc` call, the function should populate `free_list` with the
returned memory. Look back at **Lab 07** (The one with `pointers2.c`) for
creating a linked list from a block of memory.

`get_node` should return the head of the free list if `free_list` is not empty
and allocate another slab if `free_list` is `NULL` (empty).

For debugging you also have access to the utility function `print_free_list`
which prints each node in `free_list`.

---

### `free_node`

This function simply places the node pointed to at the head of the free list.
It should do nothing if the input node is `NULL`.

This type of allocator is useful if you have many frequent calls to `malloc`
and free for a known data structure and you want to avoid the performance
overhead of `malloc` and `free`.

---

### Local Testing and Submission

There are five simple tests in the lab, you can run each individually by using
the -t <test #> option. For example,
```bash
./node_alloc -t 3 
```
will run test 3. 

To test everything locally, run the following on the command line:
```bash
python3 test_kit.py ALL
```

You should upload your completed `node_alloc.c` to Gradescope. As always, you
can just use the GitLab submission button.
