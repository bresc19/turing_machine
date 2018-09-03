# Turing Machine                                    <img align="right" width="100" height="120" src="https://upload.wikimedia.org/wikipedia/commons/thumb/a/a1/Alan_Turing_Aged_16.jpg/220px-Alan_Turing_Aged_16.jpg">

 ###  :computer: A program simulator for  Non-Deterministic Turing-Machines  :computer:
The simulator was implemented with C-Language, for a project of Algorithms and Data Structure course, at Politecnico of Milan. 

### ⌨️ input file ⌨️
it's formed by:
- **List of Turing Machine's transitions**;
- **Final State(s)**: if a final state is reached, "1" will print, if not "0";
- **Max number of Stapes**: if a track croossed more than *Max step* during computation, "U" would be printed;
- **Strings** that machine have to compute.

An *example* of input file with clear explaination is [Here](https://github.com/bresc19/turing_machine/blob/master/Info.pdf)


All *tests* used are [Here](https://github.com/bresc19/turing_machine/tree/master/Tests) 

### :package: Data Structure :package: ### 
- **Transition**: they'are stored in a **n-ary tree**. 
- **Computation**: It's a BFS, in order to get a final state or not. BFS is made *iterately*, so using a **circular array** to store transition crossed and tapes of every non-deterministic track; 
