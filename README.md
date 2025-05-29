This project is an implementation of a Smart Car Parking System developed as part of the CSL214: Data Structure and Program Design – II course (Batch R2). It simulates the functioning of a car parking lot with automated vehicle entry/exit management, membership tracking, billing, and statistics — all implemented in C using dynamic data structures.

📋 Project Overview
The smart parking lot consists of 50 parking spaces, each identified by a unique ID (1 to 50). The system maintains the availability status of these spaces and assigns them dynamically based on user membership levels:

Golden Membership: Spaces 1–10

Premium Membership: Spaces 11–20

No Membership: Spaces 21–50

The parking lot offers benefits such as discounted pricing, automated allocation, and membership upgrades based on accumulated parking hours.

🔧 Core Features
🅿️ Vehicle Entry
Registers new vehicles with details like:

Vehicle number (unique)

Owner name

Arrival timestamp

Membership (initially none)

Allocates the nearest available parking space according to membership

Updates parking space status (occupied)

🚙 Vehicle Exit
Calculates total parking hours for the session

Updates total accumulated hours

Applies membership upgrades:

Premium: After 100 hours

Golden: After 200 hours

Computes payment using the defined policy

Updates parking space status (freed)

💰 Payment Policy
₹100 for the first 3 hours

₹50 for each additional hour

10% discount for Premium and Golden members

🎟️ Membership Policy
No Membership: <100 hours

Premium: ≥100 hours

Golden: ≥200 hours

📊 Analytics & Sorting
Sort vehicles by:

Number of parkings done

Total parking fees paid

Sort parking spaces by:

Frequency of usage

Total revenue generated

📂 Data Management
All data (vehicles, parking spaces) is stored using linked lists:

Optimal choice (singly, doubly, or circular) based on operation

Stacks and queues are used where applicable for efficient data access

Initial data (for vehicles and parking spaces) is read from files

Data is unsorted and sorted during runtime using linked list logic

No arrays are used in the implementation

🗂️ Initial Dataset Requirements
Minimum of 10 pre-registered parking spaces (mixed membership types)

Data files must include:

Registered vehicle details

Parking space status

Past entry/exit logs to simulate real scenarios

🛠️ Technologies Used
C Programming

File Handling

Linked Lists (Singly, Doubly, Circular)

Stack and Queue ADTs

Dynamic Memory Allocation

⚠️ Constraints
Maximum 50 cars at any given time

No array usage allowed

Only .c and .h files are permitted in submission

Code must be original; plagiarism will result in disqualification
