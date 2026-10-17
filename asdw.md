# **WEEK 1 REPORT: ANALYZE AND DESIGN**

**PROJECT: LIBRARY MANAGEMENT SYSTEM (C#)**

## **I. System context and objectives**

The Library Management System is built using C# to manage a diverse catalog of library materials (physical books and e-books), manage member information, and oversee the entire borrow–return lifecycle, including handling overdue items and calculating late return penalties.

## **II. SYLLABUS MAPPING**

| Core OOP Concept | Specific Class & Method Applied (To be detailed in Self-Explanation) | Design Purpose & Technical Solution |
| :---- | :---- | :---- |
| **Encapsulation** | - LibraryItem: MarkAsBorrowed(), MarkAsReturned()<br>- Member: MaxBorrowLimit (private set)<br>- Loan: CompleteReturn() | Protects internal state. Sets private/protected set for all sensitive properties. Forbids external code from directly overwriting fields. |
| **Inheritance** | - PhysicalBook : LibraryItem<br>- EBook : LibraryItem | Inherits and reuses common attributes (Id, Title, Author, IsAvailable). Extends domain-specific attributes (ShelfLocation, ISBN, FileSizeMB). |
| **Polymorphism** | - Base class: LibraryItem.CalculateOverdueFee(int days) (abstract)<br>- Subclasses: override CalculateOverdueFee(...) | Implements differentiated overdue fee calculation:<br>+ Physical books incur 5,000 VND/day<br>+ E-books never expire / incur 0 VND penalty |

## **III. DRAFT UML DIAGRAM AND CRC CARDS**

### **1. Draft UML Class Diagram**

#### **a. Sơ đồ dạng văn bản (ASCII Class Diagram)**

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

Chú thích ký hiệu quan hệ:
  ───▷  Kế thừa (Inheritance / is-a)
  ---▷  Tham chiếu theo ID (Reference by ID / has-a: linked by ItemId)
  ───   Kết hợp (Association: MemberId)
```

#### **b. Sơ đồ Mermaid Class Diagram (Markdown Live Rendering)**

```mermaid
classDiagram
    direction TB

    class LibraryItem {
        <<abstract>>
        #string Id
        #string Title
        #string Author
        #bool IsAvailable
        +MarkAsBorrowed() void
        +MarkAsReturned() void
        +CalculateOverdueFee(int overdueDays)* decimal
        +ToDataString()* string
        +DisplayInfo() void
    }

    class PhysicalBook {
        -string ShelfLocation
        -string ISBN
        +CalculateOverdueFee(int overdueDays) decimal
        +ToDataString() string
        +DisplayInfo() void
    }

    class EBook {
        -double FileSizeMB
        +CalculateOverdueFee(int overdueDays) decimal
        +ToDataString() string
        +DisplayInfo() void
    }

    class Member {
        -string Id
        -string FullName
        -int MaxBorrowLimit = 3
        +ToDataString() string
        +DisplayInfo(int count) void
    }

    class Loan {
        -string LoanId
        -string MemberId
        -string ItemId
        -DateTime BorrowDate
        -DateTime DueDate
        -DateTime? ReturnDate
        -string Status
        -decimal FineAmount
        +IsOverdue(DateTime atDate) bool
        +GetOverdueDays(DateTime atDate) int
        +CompleteReturn(DateTime date, decimal fee) void
        +ToDataString() string
        +DisplayInfo() void
    }

    %% Quan hệ Kế thừa (Inheritance / is-a)
    LibraryItem <|-- PhysicalBook : is-a (Inheritance)
    LibraryItem <|-- EBook : is-a (Inheritance)

    %% Quan hệ Kết hợp & Tham chiếu (Associations & References)
    Member "1" -- "*" Loan : MemberId
    Loan "*" ..> "1" LibraryItem : has-a (linked by ItemId)
```

#### **c. Đặc tả chi tiết các lớp (Class Specifications)**

1. **`<<abstract>> LibraryItem` (Lớp trừu tượng cha)**:
   - **Thuộc tính (Attributes)**:
     - `# Id : string`: Mã định danh duy nhất của tài liệu.
     - `# Title : string`: Tựa đề tài liệu.
     - `# Author : string`: Tác giả tài liệu.
     - `# IsAvailable : bool`: Trạng thái khả dụng của tài liệu (True: có sẵn trong kho, False: đang được mượn).
   - **Phương thức (Methods)**:
     - `+ MarkAsBorrowed() : void`: Đánh dấu tài liệu đã được mượn (`IsAvailable = false`).
     - `+ MarkAsReturned() : void`: Đánh dấu tài liệu đã được trả lại kho (`IsAvailable = true`).
     - `+ <<abstract>> CalculateOverdueFee(overdueDays: int) : decimal`: Phương thức trừu tượng tính phí phạt trễ hạn.
     - `+ <<abstract>> ToDataString() : string`: Phương thức trừu tượng chuyển đổi thành chuỗi lưu file `.txt`.
     - `+ DisplayInfo() : void`: Hiển thị thông tin tài liệu lên Console.

2. **`PhysicalBook` (Sách in truyền thống - Kế thừa `LibraryItem`)**:
   - **Thuộc tính (Attributes)**:
     - `- ShelfLocation : string`: Vị trí lưu trữ trên kệ sách trong kho (ví dụ: "Kệ A1-01").
     - `- ISBN : string`: Mã số tiêu chuẩn quốc tế cho sách in.
   - **Phương thức (Methods)**:
     - `+ CalculateOverdueFee(overdueDays: int) : decimal`: Ghi đè tính tiền phạt trễ hạn `overdueDays * 5.000 VNĐ / ngày`.
     - `+ ToDataString() : string`: Ghi đè định dạng lưu file cho sách in (`PHYSICAL|Id|Title|Author|IsAvailable|ShelfLocation|ISBN`).
     - `+ DisplayInfo() : void`: Hiển thị thông tin sách in kèm vị trí kệ và mã ISBN.

3. **`EBook` (Sách điện tử kỹ thuật số - Kế thừa `LibraryItem`)**:
   - **Thuộc tính (Attributes)**:
     - `- FileSizeMB : double`: Dung lượng tệp tin sách điện tử tính theo Megabytes (MB).
   - **Phương thức (Methods)**:
     - `+ CalculateOverdueFee(overdueDays: int) : decimal`: Ghi đè chính sách miễn phí phạt trễ hạn (luôn trả về `0m`).
     - `+ ToDataString() : string`: Ghi đè định dạng lưu file cho sách điện tử (`EBOOK|Id|Title|Author|IsAvailable|FileSizeMB`).
     - `+ DisplayInfo() : void`: Hiển thị thông tin sách điện tử kèm dung lượng MB.

4. **`Member` (Độc giả thư viện)**:
   - **Thuộc tính (Attributes)**:
     - `- Id : string`: Mã độc giả (ví dụ: "M001").
     - `- FullName : string`: Họ và tên độc giả.
     - `- MaxBorrowLimit : int = 3`: Hạn mức mượn sách tối đa cùng một lúc (mặc định: 3 cuốn).
   - **Phương thức (Methods)**:
     - `+ ToDataString() : string`: Định dạng chuỗi lưu file độc giả (`Id|FullName|MaxBorrowLimit`).
     - `+ DisplayInfo(count: int) : void`: Hiển thị thông tin độc giả kèm số lượng sách hiện đang mượn.

5. **`Loan` (Phiếu mượn - Giao dịch mượn/trả)**:
   - **Thuộc tính (Attributes)**:
     - `- LoanId : string`: Mã phiếu mượn (ví dụ: "L001").
     - `- MemberId : string`: Mã độc giả mượn.
     - `- ItemId : string`: Mã tài liệu thư viện được mượn.
     - `- BorrowDate : DateTime`: Ngày bắt đầu mượn sách.
     - `- DueDate : DateTime`: Hạn chót phải trả sách.
     - `- ReturnDate : DateTime?`: Ngày thực tế trả sách (`null` nếu chưa trả).
     - `- Status : string ("Active" / "Returned")`: Trạng thái phiếu mượn.
     - `- FineAmount : decimal`: Tiền phạt phát sinh do trễ hạn.
   - **Phương thức (Methods)**:
     - `+ IsOverdue(atDate: DateTime) : bool`: Kiểm tra phiếu mượn có đang bị trễ hạn so với mốc thời gian hay không.
     - `+ GetOverdueDays(atDate: DateTime) : int`: Tính số ngày trễ hạn so với hạn chót.
     - `+ CompleteReturn(date: DateTime, fee: decimal) : void`: Hoàn tất trả sách, cập nhật ngày trả thực tế và tiền phạt.
     - `+ ToDataString() : string`: Định dạng chuỗi lưu file phiếu mượn (`LoanId|MemberId|ItemId|...`).
     - `+ DisplayInfo() : void`: In thông tin chi tiết của phiếu mượn ra màn hình.

#### **d. Các mối quan hệ trong sơ đồ (Relationships)**
- **Kế thừa (Inheritance / is-a)**:
  - `PhysicalBook` kế thừa `LibraryItem` (tái sử dụng các trường dữ liệu và hành vi chung).
  - `EBook` kế thừa `LibraryItem` (tái sử dụng các trường dữ liệu và hành vi chung).
- **Kết hợp (Association)**:
  - `Member` liên kết `Loan`: Quan hệ `1 - *` (Một độc giả có thể có nhiều phiếu mượn `[0..*]`, liên kết thông qua khóa `MemberId`).
- **Tham chiếu định danh (Reference by ID / has-a)**:
  - `Loan` tham chiếu đến `LibraryItem`: Quan hệ `* - 1` (Một phiếu mượn liên kết đến tài liệu thư viện thông qua khóa `ItemId`).

---

### **2. Set of 5 CRC cards (Class-Responsibility-Collaboration)**

| Class | Responsibilities | Collaborations |
| :---- | :---- | :---- |
| **LibraryItem** | • Manage identity metadata (Id, Title, Author)<br>• Encapsulate availability changes via MarkAsBorrowed() and MarkAsReturned()<br>• Define the abstract template method for overdue fee calculation CalculateOverdueFee() | PhysicalBook, EBook, Loan |
| **PhysicalBook** | • Manage shelf location and ISBN code<br>• Implement overdue fee formula: days * 5,000 VND | LibraryItem (inheritance) |
| **EBook** | • Manage file size<br>• Implement overdue fee exemption policy: Always return 0 VND | LibraryItem (inheritance) |
| **Member** | • Manage patron information (Id, FullName)<br>• Set and enforce maximum borrow limit (MaxBorrowLimit = 3) | Loan |
| **Loan** | • Link patron and item via MemberId and ItemId<br>• Define loan duration (14 days) and detect overdue status (IsOverdue)<br>• Encapsulate return finalization (actual return date and fine) via CompleteReturn() | Member, LibraryItem |

---

## **IV. MINIMUM TEST PLAN**

In accordance with the **Minimum Test Cases** requirements:

| Test | Test Inputs | Expected Assertions |
| :---- | :---- | :---- |
| **1. Happy Path Scenario** | - Member M001 (borrow count = 0)<br>- Physical book B001 (IsAvailable = true)<br>- Borrow period: 14 days, returned after 7 days. | - Newly created Loan has status Active<br>- B001.IsAvailable transitions to false<br>- M001.CurrentlyBorrowedCount increments from 0 to 1<br>- Upon return: Loan status becomes returned, penalty = 0 VND, book unlocked (IsAvailable = true), M001 borrow count decrements to 0. |
| **2. Validation / Error Rejection** | **Scenario A:** Member M001 attempts to borrow B001 while IsAvailable == false<br>**Scenario B:** Member M002 currently holding 3 items attempts to borrow a 4th | - **Scenario A:** Throws InvalidOperationException: Item is already borrowed; no new Loan created<br>- **Scenario B:** Throws InvalidOperationException: Member limit exceeded (Max 3); item state remains unchanged |
| **3. Polymorphism / Inheritance Verification** | - Both overdue by 5 days<br>- Loan 1: LibraryItem is a PhysicalBook<br>- Loan 2: LibraryItem is an EBook | - Loan 1: item.CalculateOverdueFee(5) returns 25,000 VND<br>- Loan 2: item.CalculateOverdueFee(5) returns 0 VND |
