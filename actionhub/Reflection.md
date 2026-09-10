 1. Real-World Justification for Each Algorithm

* Task A : In a live auction, new bids arrive almost in order, meaning the data is nearly sorted. Insertion Sort runs in O(n) best-case time for nearly-sorted data because elements only need to shift a few positions. This makes it very fast and efficient for live updates.  
* Task B : Selection Sort works by finding the maximum (or minimum) element in each pass and putting it in place. If we only need the Top 3 Bidders, we only need 3 outer passes instead of sorting the whole list. It also sorts in-place without using extra memory.  
* Task C : This algorithm has very simple logic and is easy to write and verify for errors. Because the batch is tiny ($\\le 50$ records), its O(n^2) time is not a problem and runs in less than a millisecond.  
* Task D : Bubble Sort checks adjacent items one by one. If a full pass finishes with zero swaps, the boolean flag immediately tells us that prices have stopped changing ("STABLE") in O(n) time.  
* Task E :*Reports contain millions of historical records, so we need an O(n log n) average-time algorithm. Quick Sort is fast in practice, uses little extra memory, and takes advantage of CPU cache locality.

 2. Measured runtime of Quick Sort

* Dataset size: 100,000 records.  
* Measured runtime: about 132ms.

 3.Bug encountered and how it was solved

* (Task D)The nested comparison loop used j \< (int)v.size() \- 2 \- i, preventing the final adjacent pair (v\[n-2\], v\[n-1\]) from ever being compared. How it solved: Adjusted the loop bound to j \< (int)v.size() \- 1 \- i so that the scan properly reaches the end of the unsorted segment. 

 4. Three Prompts Sent to the LLM and Policy Classification**

* Prompt 1: T đang viết hàm insertionsort cho struct bid trong C++ nhưng bị báo lỗi kiểu dữ liệu và sai điều kiện lặp for (int i; i=1; i\<v.size()-1). giải thích các lỗi cú pháp và cách sửa điều kiện dừng  
* Prompt 2:tại sao vòng lặp trong bubblesort  (j \< v.size() \- 2 \- i) lại sai   
* Prompt 3: thuật toán pseudo code cho quicksort và giải thích từng cụm cho t dễ hiểu