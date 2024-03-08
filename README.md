# IPFS using Data Structures 
This C++ project implements a simplified version of the InterPlanetary File System (IPFS) using data structures such as Distributed Hash Table (DHT) for storing and retrieving files in a distributed environment. IPFS provides functionality similar to BitTorrent, making it easy to publish content and access it from anywhere, anytime.

Features
Content addressable files: Each file is represented by the hash of its content, making it easy to search and retrieve files based on their hash values.
Geo-distributed storage: Files are stored on multiple machines distributed across the internet, managed by a Distributed Hash Table (DHT) for efficient storage and retrieval.
Dynamic machine management: Machines responsible for storing files are mapped into the identifier space, and their responsibilities are adjusted when machines join or leave the network.

IPFS provides functionality similar to BitTorrent making it extremely easy to publish your content and
make it accessible anywhere, anytime. The obvious way to make use of IPFS is file sharing. This is
perhaps the only use of IPFS that you are going to implement in this project, however, IPFS is capable
of much more. In particular, the following properties of IPFS will be handled by your implementation.
• IPFS files are content addressable. It means that each file is represented by the hash of its content.
This hash is used to search and retrieve the file instead of filename. In other words, two files with
the same name but different content will have different hash values.

• IPFS files/folders are not located on a single machine but rather stored on multiple machines geo-
distributed across the internet. A data structure named Distributed Hash Table (DHT) is used to
manage storage/retrieval of files across different machines on the Internet.

As mentioned above, we are considering a scenario where the data (i.e., IPFS file/folder) is not located
on a single machine but rather stored on multiple machines geo-distributed across the Internet. In such
a scenario, searching or retrieval of data is very challenging, as it is hard to determine on which
machine the required data is stored. The data structures used for efficiently search data stored on a
single machine such as hash tables cannot be directly employed in a distributed environment. Figure 1
shows the concept of data/ storage and lookup in a distributed environment.
Typically, data is stored as a key, value pair. For example, key might be the name of the student, or the
student ID and value is the complete information about the student. Likewise, the key can be the name
of the patient and value can be the medical record. Similarly, the key can be the name of an image file
and value represents the file itself. Nevertheless, in IPFS the key is the hash value of the file and
value is the actual file (i.e., its content).
A distributed hash table (DHT) provides a lookup (or search) service similar to a hash table: (key,
value) pairs are stored in a DHT and any participating machine can efficiently retrieve the value
associated with a given key. Responsibility for maintaining the mapping from keys to values is
distributed among the machines. In particular,
• Each DHT has an Identifier space. Identifier space refers to the range of possible keys (or
identifiers) that can be stored. For example, if the identifier space is 4-bits, it means that a total of
sixteen key/value pairs can be stored (i.e., 16 files can be stored) with their hash values (i.e., keys
or ids) mapped to the range [0,15].
• Hash function such as SHA-1 is used to map key to identifier space. For example, in Figure 2 a key
is mapped to an id e1 in the identifier space. Another key is mapped to the id e2 and so on.
• Machines (responsible for storing the files) are also mapped into identifier space using the hash
function on their name such as google.com.
• Each machine is responsible for a range of ids in the identifier space. For example, Figure 2 depicts
that machine 1 is responsible for storing the data (i.e., key, value pairs) whose keys are mapped to
e1 and e2. Likewise, machine 3 is responsible for the ids e4 and e5.
• When a machine leaves, its id range is merged with a neighbor’s range. For example, according to
Figure 2 machine 4 will be responsible for the ids in the range e4 to e6, in case machine 3 leaves
the network.
• Likewise, when a new machine joins, the id range of neighbor is subdivided.



Contributions to improve the IPFS implementation and add new features are welcome. Please fork the repository and submit a pull request with your changes.
