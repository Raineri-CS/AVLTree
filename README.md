Algorithms 2
========================
## AVL trees
C linux program which supports the following opperations:
* Search
* Insertion
* Removal of nodes with int keys
* Hierarchical representation of said tree using the gfx library (made by my teacher).
* Un-allocation
* Must be able to save the tree itself into a binary file
* Also must be able to read the binary file file

##### "The code must compile under gcc's C99 with no warnings or errors with the -Wall -Wextra -pedantic flags."
##### It was required to use predef c structs in the project, which are as follows:
    struct s_no{
        int32_t chave:28;
        int32_t bal:2;
        int32_t reservado:2; /* sem uso */
        struct s_no* esq;
        struct s_no* dir;
    };

    struct s_arq_no{
        int32_t chave:28;
        int32_t bal:2;
        uint32_t esq:1;
        uint32_t dir:1;
    };
##### It was also required for all the things to be inside the main.c file.

##### The insertion, removal, search algorithms were implemented in accord to the teacher's specifications.

###### FreeSans.ttf is distributed under GPL