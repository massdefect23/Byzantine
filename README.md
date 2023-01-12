A system where each process has a map of nodes indexed by a path string that consists of a list of process IDs that the information came through. The input and output values of the nodes are set in different rounds. The default constructor is not used but is required for storing the objects in a map.

The default constructor is never used, however it is required that we make one if the objects will be stored in a map.

A system where the Traits base class is used to define the messaging behavior for the processes. The Process class has a global object of type Traits that it uses to get all the information used to characterize the information about the role of the process such as general or lieutenant. The author mentions that the name Traits may not be the best for this usage but is close.

A system where the Process class has a static member called Traits. The methods of this Traits class are called at key points during the messaging and decision phases to determine how the process should behave. The configuration of the system can be modified by changing the Traits class.

A specific implementation of a traits class in which a general and a process 2 are faulty. The general sends either a one or a zero to all processes, and process 2 always sends a one to everyone, disregarding its intended message.

*GetSourceValue* - which returns the true value of the source's value. The source may send faulty values to other processes but the Node returned by this method will be in its root node. The example given is that of a General's node with an input value of 0 which is the desired value. However, as the General is faulty, it doesn't really matter.

*GetValue()* which is used during message passing to get the value returned by a given process. The inputs of the method are value which is the input value that the process should be sending, source is the source process ID, destination is the destination process ID and Path is the path being used for this particular message. The implementation described has two faulty processes. The source process returns a random value, and process ID 2 always returns a ONE, in contradiction of the General's desired value of 0.

*GetDefault()* which is used when breaking a tie. It is used to get the default value, which is an arbitrary decision but needs to be consistent across all processes. The most important aspect is that the processes arrive at a correct decision regardless of the default value, whether it is always 0 or always 1. The implementation described has set the default value to 1.

Member of the class that holds the process id for the source process. This information is needed a few times during the messaging and decision-making process to determine if a process is the source process. A method called *GetSource()* could have been used but since it is a constant, it is exposed directly as a member instead.

Two members of the class that hold M and N, the number of rounds of messaging and the number of processes. These values could be accessed via methods but since they are constants, they are exposed directly as public members instead.

A member of the class that can be turned on by passing true in the constructor. When turned on, it will provide additional trace output.

The constructor of the Process class. It mentions that if the static mChildren map is empty, it calls *GenerateChildren()* which is a static method that generates a map used throughout the program to diagram the tree that holds the topology of the message tree. If the process is the source process (the General), it initializes the default path with the General's source value, which is a node that contains the General's proposed value and nothing else.

Sending messages among the processes. After constructing all messages, the SendMessages method should be called on each process, once per round. The method will send the appropriate messages for each round to all the other processes listed in the vector passed in as an argument. The decision of what messages to send is based on the static map mPathsByRank, indexed by round and the processId of the process. It gives the entire set of target paths that the process needs to send messages to. The process then sends a message to the correct target process for each path in the map. The Traits class is used to get the node value that is sent in the message, which allows for faulty processes to send specifically tailored deceptive messages. Also, if the debug flag is turned on, information about the message is printed to the console.

Making a decision among the processes. After all messages have been sent, the Decide method is called. This method follows the description of the algorithm closely, it has to work its way from the leaf values up to the root of the tree. The first round consists of going to the leaf nodes and copying the input value to the output value. All subsequent rounds consist of getting the majority of the output values from each node's children and copying that to the node's output value. When the process reaches the root node, there is only one node with an output value, and that represents the process's decision.

Debug routine that is used to output the contents of the tree in text form. It is not too hard to read if the number of processes is not too big. The routine operates recursively, going through the tree and outputting the children of each node.

Debug routine that outputs the contents of the tree in a format that can be read by the dot graphics compiler, allowing the user to use the graphics tool to get a graphical image of the tree.

Routine that calculates the majority value for the children of a given path. It increments the count for all possible values over the children. If there is a clearcut majority, it returns that, otherwise it returns the default value defined by the Traits class.

Receiving a message, when some other process calls this method on the current process with a path and a node, it stores the value and uses it in the next round of messaging, which is simple.

Static method that generates a static copy of the tree in the mChildren static map. It is called by the first process that is constructed. The function is useful in finding all the children for a given path by looking up mChildren[path]. It is a recursive routine that has some debug output that is useful in debugging. If the traits object has the debug member set, it will be printed out.

Paramteters that are used to give character to the Traits class.

