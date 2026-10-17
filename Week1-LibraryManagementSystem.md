| Member | Assigned Tasks |
| :--- | :--- |
| **Vũ Thành Huy** | <ul><li>Implement BFS algorithm using `std::queue` and implement Shortest Path reconstruction using the `parent[]` array.</li><li>Write Unit Tests replicating the exact Week-1 BFS hand-trace on the 6-node graph and export execution test output evidence.</li><li>Draft the mathematical proof for BFS time and space complexity $O(V + E)$ based on the code's loop structure.</li><li>Identify, verify, and document AI failures/limitations when handling BFS edge cases (empty graph, unreachable destination, source identical to destination).</li><li>Author individual Teach-back section: provide a line-level code explanation, state the loop invariant for the queue, and compile the group report (including Syllabus Mapping).</li></ul> |
| **Lê Thiện Luân** | <ul><li>Construct the core `Graph` class using an Adjacency List representation supporting both directed and undirected graphs.</li><li>Implement the Iterative DFS algorithm using an explicit `std::stack` (strictly avoiding recursion) and integrate Cycle Detection.</li><li>Write Unit Tests replicating the Week-1 DFS hand-trace, verify back-edges, and export execution test output evidence.</li><li>Draft the mathematical proof for Iterative DFS time and space complexity $O(V + E)$.</li><li>Author individual Teach-back section: provide a line-level code explanation and state the loop invariant for the explicit stack.</li></ul> |




### **1. Draft UML Class Diagram**


```text
                       +--------------------------------------------------------------+
                       |                  <<abstract>> LibraryItem                    |
                       +--------------------------------------------------------------+
                       | # Id : string                                                |
                       | # Title : string                                             |
                       | # Author : string                                            |
                       | # IsAvailable : bool                                         |
                       +--------------------------------------------------------------+
                       | + MarkAsBorrowed() : void                                    |
                       | + MarkAsReturned() : void                                    |
                       | + <<abstract>> CalculateOverdueFee(overdueDays: int): decimal|
                       | + <<abstract>> ToDataString() : string                       |
                       | + DisplayInfo() : void                                       |
                       +--------------------------------------------------------------+
                                      ^                                ^
                                      | (is-a:                         | (is-a:
            +-------------------------+  Inheritance)                  +-------------------------+
            |                                ^                                                   |
+--------------------------------------+     | (has-a: linked       +--------------------------------------+
|             PhysicalBook             |     |  by ItemId)          |                EBook                 |
+--------------------------------------+     |                      +--------------------------------------+
| - ShelfLocation : string             |     |                      | - FileSizeMB : double                |
| - ISBN : string                      |     |                      +--------------------------------------+
+--------------------------------------+     |                      | + CalculateOverdueFee(overdueDays)   |
| + CalculateOverdueFee(overdueDays)   |     |                      |   --> returns 0m                     |
|   --> returns days * 5000m           |     |                      | + ToDataString() : string            |
| + ToDataString() : string            |     |                      | + DisplayInfo() : void               |
| + DisplayInfo() : void               |     |                      +--------------------------------------+
+--------------------------------------+     |
                                             |
     +---------------------------------------+
     |
+----+----------------------------------+                   +--------------------------------------+
|                 Loan                  |                   |                Member                |
+---------------------------------------+                   +--------------------------------------+
| - LoanId : string                     |        MemberId   | - Id : string                        |
| - MemberId : string                   | * -------------- 1| - FullName : string                  |
| - ItemId : string                     |                   | - MaxBorrowLimit : int = 3           |
| - BorrowDate : DateTime               |                   +--------------------------------------+
| - DueDate : DateTime                  |                   | + ToDataString() : string            |
| - ReturnDate : DateTime?              |                   | + DisplayInfo(count: int) : void     |
| - Status : string ("Active"/"Returned")                   +--------------------------------------+
| - FineAmount : decimal                |
+---------------------------------------+
| + IsOverdue(atDate: DateTime) : bool  |
| + GetOverdueDays(atDate: DateTime):int|
| + CompleteReturn(date, fee) : void    |
| + ToDataString() : string             |
| + DisplayInfo() : void                |
+---------------------------------------+
