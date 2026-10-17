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
