## Time Cards Export Algorithm: Performance Optimization and Rewriting for Payroll Processing

### **Overview**

This algorithm is designed to fetch a large amount of data from various sources in a local database and
integrate this information into an existing payroll processing system. 
The ultimate goal is to export the data into a text file format that can be easily interpreted by our ERP system,
significantly optimizing payroll processing and massively reducing the time spent on manual adjustments.

Previously written in PHP, the algorithm has been fully rewritten in C++ to achieve enhanced performance and to serve as a benchmark for future optimizations and studies.
(However, the PHP Algorithm has also been extremely optimized by me, but C++ beats it, with C++ leading with a 0.80s export and PHP possessing a 3-5s export).

### **Key Features**

- **Data Fetching**: The algorithm fetches a substantial volume of data from various local database tables, which are populated by external APIs. These external sources are periodically updated to provide the most current and relevant time card data for payroll processing.
  
- **Data Transformation**: After gathering the data, the algorithm performs several transformations to structure and format the data in a way that aligns with the expected input for the ERP system. This involves complex business logic and data sanitization to ensure accuracy.

- **Optimized Export Process**: The final stage of the process involves generating a text file that can be quickly and easily imported into the payroll processing system. The export operation, which once took several minutes or even hours, with trials and errors,
  now can be completed in under one second, thanks to the improvements in data processing and export efficiency.

### **Performance Improvements**

#### **From PHP to C++**
The original version of the algorithm was written in PHP, a scripting language that, while flexible, wasn't optimized for performance in high-volume data processing. By rewriting the entire system in C++, a compiled language, I was able to achieve significant performance gains.

- **Reduced Execution Time**: The most noticeable improvement is the drastic reduction in processing time. Previously, the export process could take hours, primarily due to inefficiencies in how PHP handled large datasets and the amount of unnecessary API calls that used to exist.
- The C++ version, by leveraging memory management, efficient data structures, and direct access to system resources, now completes the task in just a second.
  
- **Memory Efficiency**: C++ allows for more granular control over memory allocation and deallocation, minimizing overhead and enhancing data processing speeds. This was particularly crucial given the large size of the data involved.

#### **Benchmarking and Study Purposes**
The rewrite in C++ also serves as a study to understand C++ syntax, libraries data structures and benchmark different approaches to algorithm optimization.
By comparing the PHP version with the C++ version, 
I gained valuable insights into performance bottlenecks and areas where optimization could make a substantial impact.
These lessons are now part of an ongoing study in performance engineering, focused on improving data processing workflows.
