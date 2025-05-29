#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// Constants
#define MAX_PARKING_SPACES 50
#define FREE 0
#define OCCUPIED 1
#define GOLDEN_HOURS 200
#define PREMIUM_HOURS 100
#define GOLD 2
#define PREMIUM 1
#define NONE 0
#define BASE_FEES 100
#define EXTRA_FEES 50
#define DISCOUNT 0.10

// Structure for arrival and departure times
struct datetime {
    int time;
    int date;
    int month;
    int year;
};

// Structure for vehicle node in linked list
typedef struct vehicle_node {
    char vehicle_num[10];
    char owner_name[20];
    struct datetime arrival;
    struct datetime departure;
    int membership;
    int total_parking_hours;
    int parking_ID;
    int total_amount_paid;
    int parking_count;
    struct vehicle_node* next;
} vehicle;

// Structure for parking space node in linked list
struct parking_node {
    int parking_space_ID;
    int status;
    int occupancy_count;
    int space_revenue;
    struct parking_node* next;
};

// Global pointers for the linked lists
struct vehicle_node* vehicle_list = NULL;
struct parking_node* parking_list = NULL;
int vehicle_count = 0;

// Function declarations
struct vehicle_node* create_vehicle_node();
struct parking_node* create_parking_node(int id);
void initialize_parking_spaces();
struct vehicle_node* check_registered(char vehicle_num[]);
struct parking_node* find_parking_space(int membership);
int calculate_parking_fee(int hours_parked, int membership);
void park_vehicle();
void exit_vehicle();
void display_sorted_by_parkings();
void display_sorted_by_amount();
void display_sorted_by_occupancy();
void display_sorted_by_revenue();
void cleanup();
void load_data();
void save_data();
struct vehicle_node* copy_list(struct vehicle_node* head);

struct vehicle_node* create_vehicle_node() {
    struct vehicle_node* new_node = (struct vehicle_node*)malloc(sizeof(struct vehicle_node));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_node->next = NULL;
    new_node->total_parking_hours = 0;
    new_node->total_amount_paid = 0;
    new_node->parking_count = 0;
    new_node->membership = NONE;
    new_node->parking_ID = 0;
    return new_node;
}

// Function to create new parking node
struct parking_node* create_parking_node(int id) {
    struct parking_node* new_node = (struct parking_node*)malloc(sizeof(struct parking_node));
    if (new_node == NULL) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    new_node->parking_space_ID = id;
    new_node->status = FREE;
    new_node->occupancy_count = 0;
    new_node->space_revenue = 0;
    new_node->next = NULL;
    return new_node;
}

// Function to initialize parking spaces
void initialize_parking_spaces() {
    struct parking_node* current = NULL;
    
    for (int i = 0; i < MAX_PARKING_SPACES; i++) {
        struct parking_node* new_node = create_parking_node(i + 1);
        
        if (parking_list == NULL) {
            parking_list = new_node;
            current = new_node;
        } else {
            current->next = new_node;
            current = new_node;
        }
    }
}

// Function to check if a vehicle is already registered
struct vehicle_node* check_registered(char vehicle_num[]) {
    struct vehicle_node* current = vehicle_list;
    while (current != NULL) {
        if (strcmp(current->vehicle_num, vehicle_num) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to find a parking space based on membership
struct parking_node* find_parking_space(int membership) {
    struct parking_node* current = parking_list;
    int space_number = 1;
    
    while (current != NULL) {
        if (current->status == FREE) {
            if ((membership == GOLD && space_number <= 10) ||
                (membership == PREMIUM && space_number > 10 && space_number <= 20) ||
                (membership == NONE && space_number > 20)) {
                return current;
            }
        }
        current = current->next;
        space_number++;
    }
    return NULL;
}

// Helper function to calculate parking fee
int calculate_parking_fee(int hours_parked, int membership) {
    int fee = BASE_FEES;
    if (hours_parked > 3) {
        fee += (hours_parked - 3) * EXTRA_FEES;
    }
    if (membership > 0) {
        fee = (int)(fee * (1.0 - DISCOUNT));
    }
    return fee;
}

// Helper functions for date calculations
int days_in_month(int month) {
    int days[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    return days[month - 1];
}

int totaldays(int date, int month, int year) {
    int days = 0;
    for (int i = 1990; i < year; i++) {
        days += 365;
    }
    for (int i = 1; i < month; i++) {
        days += days_in_month(i);
    }
    days += date;
    return days;
}

int date_difference(int date1, int month1, int year1, int date2, int month2, int year2) {
    int days1 = totaldays(date1, month1, year1);
    int days2 = totaldays(date2, month2, year2);
    return days2 - days1;
}

int hours_parked(struct datetime arrival, struct datetime departure) {
    int total_days_difference = date_difference(arrival.date, arrival.month, arrival.year,
                                              departure.date, departure.month, departure.year);
    int total_hours = total_days_difference * 24 + (departure.time - arrival.time);
    if (total_hours < 0) {
        total_hours += 24;
    }
    return total_hours;
}

// Function to park a vehicle
void park_vehicle() {
    char vehicle_num[10], owner_name[20];
    struct datetime arrival;

    printf("Enter vehicle number: ");
    scanf("%s", vehicle_num);
    printf("Enter arrival time (24-hour format): ");
    scanf("%d", &arrival.time);
    printf("Enter arrival date: ");
    scanf("%d", &arrival.date);
    printf("Enter arrival month: ");
    scanf("%d", &arrival.month);
    printf("Enter arrival year: ");
    scanf("%d", &arrival.year);

    struct vehicle_node* existing_vehicle = check_registered(vehicle_num);
    struct vehicle_node* vehicle;

    if (existing_vehicle == NULL) {
        printf("Enter owner's name: ");
        scanf("%s", owner_name);

        vehicle = create_vehicle_node();
        strcpy(vehicle->vehicle_num, vehicle_num);
        strcpy(vehicle->owner_name, owner_name);
        vehicle->membership = NONE;
        vehicle->total_parking_hours = 0;
        vehicle->total_amount_paid = 0;
        vehicle->parking_count = 0;

        vehicle->next = vehicle_list;
        vehicle_list = vehicle;
        vehicle_count++;
    } else {
        vehicle = existing_vehicle;
    }

    vehicle->arrival = arrival;

    struct parking_node* parking_space = find_parking_space(vehicle->membership);
    if (parking_space != NULL) {
        vehicle->parking_ID = parking_space->parking_space_ID;
        parking_space->status = OCCUPIED;
        parking_space->occupancy_count++;
        printf("Vehicle parked at space %d\n", parking_space->parking_space_ID);
    } else {
        printf("No suitable parking space available.\n");
    }
}

// Function to handle vehicle exit
void exit_vehicle() {
    char vehicle_num[10];
    struct datetime departure;

    printf("Enter vehicle number: ");
    scanf("%s", vehicle_num);
    printf("Enter departure time (24-hour format): ");
    scanf("%d", &departure.time);
    printf("Enter departure date: ");
    scanf("%d", &departure.date);
    printf("Enter departure month: ");
    scanf("%d", &departure.month);
    printf("Enter departure year: ");
    scanf("%d", &departure.year);

    struct vehicle_node* vehicle = check_registered(vehicle_num);
    if (vehicle != NULL) {
        int parked_hours = hours_parked(vehicle->arrival, departure);
        vehicle->total_parking_hours += parked_hours;

        int fee = calculate_parking_fee(parked_hours, vehicle->membership);
        vehicle->total_amount_paid += fee;
        vehicle->parking_count++;

        struct parking_node* current = parking_list;
        while (current != NULL) {
            if (current->parking_space_ID == vehicle->parking_ID) {
                current->status = FREE;
                current->space_revenue += fee;
                break;
            }
            current = current->next;
        }

        if (vehicle->total_parking_hours >= GOLDEN_HOURS) {
            vehicle->membership = GOLD;
        } else if (vehicle->total_parking_hours >= PREMIUM_HOURS) {
            vehicle->membership = PREMIUM;
        }

        printf("\nVehicle Exit Summary:\n");
        printf("Hours parked: %d\n", parked_hours);
        printf("Parking fee: %d Rs\n", fee);
        printf("Total parking hours: %d\n", vehicle->total_parking_hours);
        printf("Membership status: %s\n", 
               vehicle->membership == GOLD ? "GOLD" : 
               vehicle->membership == PREMIUM ? "PREMIUM" : "NONE");
    } else {
        printf("Vehicle not found in the system.\n");
    }
}

// Merge functions for sorting
struct vehicle_node* merge_by_parkings(struct vehicle_node* head1, struct vehicle_node* head2) {
    struct vehicle_node* result = NULL;
    struct vehicle_node* tail = NULL;

    while (head1 != NULL && head2 != NULL) {
        if (head1->parking_count >= head2->parking_count) {
            if (result == NULL) {
                result = tail = head1;
            } else {
                tail->next = head1;
                tail = head1;
            }
            head1 = head1->next;
        } else {
            if (result == NULL) {
                result = tail = head2;
            } else {
                tail->next = head2;
                tail = head2;
            }
            head2 = head2->next;
        }
    }

    if (head1 != NULL) {
        if (result == NULL) result = head1;
        else tail->next = head1;
    }
    if (head2 != NULL) {
        if (result == NULL) result = head2;
        else tail->next = head2;
    }

    return result;
}

struct vehicle_node* merge_by_amount(struct vehicle_node* head1, struct vehicle_node* head2) {
    struct vehicle_node* result = NULL;
    struct vehicle_node* temp = NULL;
    
    while (head1 != NULL && head2 != NULL) {
        if (head1->total_amount_paid >= head2->total_amount_paid) {
            if (result == NULL) {
                result = temp = head1;
            } else {
                temp->next = head1;
                temp = temp->next;
            }
            head1 = head1->next;
        } else {
            if (result == NULL) {
                result = temp = head2;
            } else {
                temp->next = head2;
                temp = temp->next;
            }
            head2 = head2->next;
        }
    }
    
    if (head1 != NULL) {
        if (result == NULL) result = head1;
        else temp->next = head1;
    }
    if (head2 != NULL) {
        if (result == NULL) result = head2;
        else temp->next = head2;
    }
    
    return result;
}

struct parking_node* merge_by_occupancy(struct parking_node* head1, struct parking_node* head2) {
    struct parking_node* result = NULL;
    struct parking_node* tail = NULL;

    while (head1 != NULL && head2 != NULL) {
        if (head1->occupancy_count >= head2->occupancy_count) {
            if (result == NULL) {
                result = tail = head1;
            } else {
                tail->next = head1;
                tail = head1;
            }
            head1 = head1->next;
        } else {
            if (result == NULL) {
                result = tail = head2;
            } else {
                tail->next = head2;
                tail = head2;
            }
            head2 = head2->next;
        }
    }

    if (head1 != NULL) {
        if (result == NULL) result = head1;
        else tail->next = head1;
    }
    if (head2 != NULL) {
        if (result == NULL) result = head2;
        else tail->next = head2;
    }

    return result;
}

struct parking_node* merge_by_revenue(struct parking_node* head1, struct parking_node* head2) {
    struct parking_node* result = NULL;
    struct parking_node* tail = NULL;

    while (head1 != NULL && head2 != NULL) {
        if (head1->space_revenue >= head2->space_revenue) {
            if (result == NULL) {
                result = tail = head1;
            } else {
                tail->next = head1;
                tail = head1;
            }
            head1 = head1->next;
        } else {
            if (result == NULL) {
                result = tail = head2;
            } else {
                tail->next = head2;
                tail = head2;
            }
            head2 = head2->next;
        }
    }

    if (head1 != NULL) {
        if (result == NULL) result = head1;
        else tail->next = head1;
    }
    if (head2 != NULL) {
        if (result == NULL) result = head2;
        else tail->next = head2;
    }

    return result;
}

// Generic divide function
struct vehicle_node* divide(struct vehicle_node* head) {
    struct vehicle_node* fast = head;
    struct vehicle_node* slow = head;
    struct vehicle_node* prev = NULL;
    
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }
    
    if (prev != NULL) {
        prev->next = NULL;
    }
    
    return slow;
}

struct parking_node* divide_parking(struct parking_node* head) {
    struct parking_node* fast = head;
    struct parking_node* slow = head;
    struct parking_node* prev = NULL;
    
    while (fast != NULL && fast->next != NULL) {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }
    
    if (prev != NULL) {
        prev->next = NULL;
    }
    
    return slow;
}

// Merge sort functions
struct vehicle_node* mergeSort_by_parkings(struct vehicle_node* head) {
    if (head == NULL || head->next == NULL) return head;

    struct vehicle_node* middle = divide(head);
    struct vehicle_node* left = mergeSort_by_parkings(head);
    struct vehicle_node* right = mergeSort_by_parkings(middle);

    return merge_by_parkings(left, right);
}

struct vehicle_node* mergeSort_by_amount(struct vehicle_node* head) {
    if (head == NULL || head->next == NULL) return head;
    
    struct vehicle_node* middle = divide(head);
    struct vehicle_node* left = mergeSort_by_amount(head);
    struct vehicle_node* right = mergeSort_by_amount(middle);
    
    return merge_by_amount(left, right);
}

struct parking_node* mergeSort_by_occupancy(struct parking_node* head) {
    if (head == NULL || head->next == NULL) return head;
    
    struct parking_node* middle = divide_parking(head);
    struct parking_node* left = mergeSort_by_occupancy(head);
    struct parking_node* right = mergeSort_by_occupancy(middle);
    
    return merge_by_occupancy(left, right);
}

struct parking_node* mergeSort_by_revenue(struct parking_node* head) {
    if (head == NULL || head->next == NULL) return head;
    
    struct parking_node* middle = divide_parking(head);
    struct parking_node* left = mergeSort_by_revenue(head);
    struct parking_node* right = mergeSort_by_revenue(middle);
    
    return merge_by_revenue(left, right);
}

// Display functions
void display_sorted_by_parkings() {
    if (vehicle_count == 0) {
        printf("No vehicles registered.\n");
        return;
    }

    struct vehicle_node* list_copy = copy_list(vehicle_list);
    struct vehicle_node* sorted_list = mergeSort_by_parkings(list_copy);

    printf("\nVehicles sorted by total parkings:\n");
    printf("Vehicle Number | Total Parkings\n");
    printf("--------------------------------\n");

    struct vehicle_node* current = sorted_list;
    while (current != NULL) {
        printf("%-13s | %d\n", current->vehicle_num, current->parking_count);
        current = current->next;
    }

    // Free the copied list to avoid memory leaks
    while (sorted_list != NULL) {
        struct vehicle_node* temp = sorted_list;
        sorted_list = sorted_list->next;
        free(temp);
    }
}

void display_sorted_by_amount() {
    if (vehicle_count == 0) {
        printf("No vehicles registered.\n");
        return;
    }

    struct vehicle_node* list_copy = copy_list(vehicle_list);
    struct vehicle_node* sorted_list = mergeSort_by_amount(list_copy);
    
    printf("\nVehicles sorted by total amount paid:\n");
    printf("Vehicle Number | Amount Paid (Rs)\n");
    printf("--------------------------------\n");
    
    struct vehicle_node* current = sorted_list;
    while (current != NULL) {
        printf("%-13s | %d\n", current->vehicle_num, current->total_amount_paid);
        current = current->next;
    }

    // Free the copied list
    while (sorted_list != NULL) {
        struct vehicle_node* temp = sorted_list;
        sorted_list = sorted_list->next;
        free(temp);
    }
}

void display_sorted_by_occupancy() {
    struct parking_node* list_copy = NULL;
    struct parking_node* current_orig = parking_list;
    struct parking_node* tail = NULL;

    // Create a copy of parking list
    while (current_orig != NULL) {
        struct parking_node* new_node = create_parking_node(current_orig->parking_space_ID);
        new_node->status = current_orig->status;
        new_node->occupancy_count = current_orig->occupancy_count;
        new_node->space_revenue = current_orig->space_revenue;

        if (list_copy == NULL) {
            list_copy = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        current_orig = current_orig->next;
    }

    struct parking_node* sorted_list = mergeSort_by_occupancy(list_copy);

    int total_occupied_spaces = 0;
    printf("\nParking spaces sorted by occupancy:\n");
    printf("Space ID | Times Occupied\n");
    printf("-------------------------\n");

    struct parking_node* current = sorted_list;
    while (current != NULL) {
        printf("%-8d | %d\n", current->parking_space_ID, current->occupancy_count);
        if (current->occupancy_count > 0) {
            total_occupied_spaces++;
        }
        current = current->next;
    }

    printf("-------------------------\n");
    printf("Total Occupied Spaces: %d\n", total_occupied_spaces);

    // Free the copied list
    while (sorted_list != NULL) {
        struct parking_node* temp = sorted_list;
        sorted_list = sorted_list->next;
        free(temp);
    }
}

void display_sorted_by_revenue() {
    struct parking_node* list_copy = NULL;
    struct parking_node* current_orig = parking_list;
    struct parking_node* tail = NULL;

    // Create a copy of parking list
    while (current_orig != NULL) {
        struct parking_node* new_node = create_parking_node(current_orig->parking_space_ID);
        new_node->status = current_orig->status;
        new_node->occupancy_count = current_orig->occupancy_count;
        new_node->space_revenue = current_orig->space_revenue;

        if (list_copy == NULL) {
            list_copy = tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }
        current_orig = current_orig->next;
    }

    struct parking_node* sorted_list = mergeSort_by_revenue(list_copy);
    
    int total_revenue = 0;
    int total_occupancy = 0;
    
    printf("\n=== Parking Spaces Sorted by Revenue ===\n");
    printf("----------------------------------------\n");
    printf("Space ID | Revenue (Rs) | Times Occupied\n");
    printf("----------------------------------------\n");
    
    struct parking_node* current = sorted_list;
    while (current != NULL) {
        printf("   %-5d |    %-8d |      %-8d\n", 
               current->parking_space_ID,
               current->space_revenue,
               current->occupancy_count);
        total_revenue += current->space_revenue;
        total_occupancy += current->occupancy_count;
        current = current->next;
    }
    
    printf("----------------------------------------\n");
    printf("Totals   |    %-8d |      %-8d\n", total_revenue, total_occupancy);
    printf("----------------------------------------\n");

    // Free the copied list
    while (sorted_list != NULL) {
        struct parking_node* temp = sorted_list;
        sorted_list = sorted_list->next;
        free(temp);
    }
}

// Function to save all data to file
void save_data() {
    printf("Saving data to files...\n");
    
    // Save vehicle data
    FILE *vehicle_file = fopen("vehicles.txt", "w");
    if (!vehicle_file) {
        printf("Error: Unable to save vehicle data.\n");
        return;
    }

    struct vehicle_node* current_vehicle = vehicle_list;
    int saved_vehicles = 0;
    while (current_vehicle != NULL) {
        fprintf(vehicle_file, "%s %s %d %d %d %d %d\n",
                current_vehicle->vehicle_num,
                current_vehicle->owner_name,
                current_vehicle->total_parking_hours,
                current_vehicle->total_amount_paid,
                current_vehicle->parking_count,
                current_vehicle->membership,
                current_vehicle->parking_ID);
        current_vehicle = current_vehicle->next;
        saved_vehicles++;
    }
    fclose(vehicle_file);
    printf("Saved %d vehicles to vehicles.txt\n", saved_vehicles);

    // Save parking space data
    FILE *parking_file = fopen("parking_spaces.txt", "w");
    if (!parking_file) {
        printf("Error: Unable to save parking space data.\n");
        return;
    }

    struct parking_node* current_parking = parking_list;
    int saved_spaces = 0;
    while (current_parking != NULL) {
        fprintf(parking_file, "%d %d %d %d\n",
                current_parking->parking_space_ID,
                current_parking->status,
                current_parking->occupancy_count,
                current_parking->space_revenue);
        current_parking = current_parking->next;
        saved_spaces++;
    }
    fclose(parking_file);
    printf("Saved %d parking spaces to parking_spaces.txt\n", saved_spaces);
}

// Function to load data from file
void load_data() {
    printf("Loading data from files...\n");
    
    // Load vehicle data
    FILE *vehicle_file = fopen("vehicles.txt", "r");
    if (!vehicle_file) {
        printf("No previous vehicle data found. Starting fresh.\n");
    } else {
        char vehicle_num[10], owner_name[20];
        int total_hours, amount_paid, park_count, membership, parking_id;

        while (fscanf(vehicle_file, "%s %s %d %d %d %d %d",
                      vehicle_num,
                      owner_name,
                      &total_hours,
                      &amount_paid,
                      &park_count,
                      &membership,
                      &parking_id) == 7) {
            struct vehicle_node* new_vehicle = create_vehicle_node();
            strcpy(new_vehicle->vehicle_num, vehicle_num);
            strcpy(new_vehicle->owner_name, owner_name);
            new_vehicle->total_parking_hours = total_hours;
            new_vehicle->total_amount_paid = amount_paid;
            new_vehicle->parking_count = park_count;
            new_vehicle->membership = membership;
            new_vehicle->parking_ID = parking_id;

            new_vehicle->next = vehicle_list;
            vehicle_list = new_vehicle;
            vehicle_count++;
        }
        fclose(vehicle_file);
        printf("Loaded %d vehicles from vehicles.txt\n", vehicle_count);
    }

    // Load parking space data or initialize if no file exists
    FILE *parking_file = fopen("parking_spaces.txt", "r");
    if (!parking_file) {
        printf("No previous parking space data found. Initializing fresh parking spaces.\n");
        initialize_parking_spaces();
    } else {
        printf("Loading parking space data from parking_spaces.txt\n");
        struct parking_node* current = NULL;
        int id, status, occupancy_count, space_revenue;
        int loaded_spaces = 0;

        while (fscanf(parking_file, "%d %d %d %d",
                      &id,
                      &status,
                      &occupancy_count,
                      &space_revenue) == 4) {
            struct parking_node* new_parking = create_parking_node(id);
            new_parking->status = status;
            new_parking->occupancy_count = occupancy_count;
            new_parking->space_revenue = space_revenue;

            if (parking_list == NULL) {
                parking_list = new_parking;
                current = new_parking;
            } else {
                current->next = new_parking;
                current = new_parking;
            }
            loaded_spaces++;
        }
        fclose(parking_file);
        printf("Loaded %d parking spaces from parking_spaces.txt\n", loaded_spaces);
    }
    printf("Data loading completed successfully.\n");
}

// Function to free all allocated memory
void cleanup() {
    struct vehicle_node* current_vehicle = vehicle_list;
    while (current_vehicle != NULL) {
        struct vehicle_node* temp_vehicle = current_vehicle;
        current_vehicle = current_vehicle->next;
        free(temp_vehicle);
    }

    struct parking_node* current_parking = parking_list;
    while (current_parking != NULL) {
        struct parking_node* temp_parking = current_parking;
        current_parking = current_parking->next;
        free(temp_parking);
    }
}

struct vehicle_node* copy_list(struct vehicle_node* head) {
    if (head == NULL) return NULL;

    struct vehicle_node* new_head = NULL;
    struct vehicle_node* tail = NULL;

    while (head != NULL) {
        struct vehicle_node* new_node = create_vehicle_node();
        strcpy(new_node->vehicle_num, head->vehicle_num);
        strcpy(new_node->owner_name, head->owner_name);
        new_node->arrival = head->arrival;
        new_node->departure = head->departure;
        new_node->membership = head->membership;
        new_node->total_parking_hours = head->total_parking_hours;
        new_node->parking_ID = head->parking_ID;
        new_node->total_amount_paid = head->total_amount_paid;
        new_node->parking_count = head->parking_count;

        if (new_head == NULL) {
            new_head = new_node;
            tail = new_node;
        } else {
            tail->next = new_node;
            tail = new_node;
        }

        head = head->next;
    }

    return new_head;
}

// Main function
int main() {
    printf("Initializing Smart Parking System...\n");
    
    // First load data (this will initialize parking spaces if no file exists)
    load_data();
    
    int choice;
    printf("\nWelcome to Smart Parking System\n");

    do {
        printf("\n=== Smart Parking System Menu ===\n");
        printf("1. Park Vehicle\n");
        printf("2. Exit Vehicle\n");
        printf("3. Display Vehicles by Total Parkings\n");
        printf("4. Display Vehicles by Total Amount Paid\n");
        printf("5. Display Parking Spaces by Occupancy\n");
        printf("6. Display Parking Spaces by Revenue\n");
        printf("0. Exit System\n");
        printf("===============================\n");    
        printf("Enter your choice: ");
        
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input! Please enter a number.\n");
            while (getchar() != '\n'); // Clear input buffer
            continue;
        }

        switch (choice) {
            case 1:
                park_vehicle();
                break;
            case 2:
                exit_vehicle();
                break;
            case 3:
                display_sorted_by_parkings();
                break;
            case 4:
                display_sorted_by_amount();
                break;
            case 5:
                display_sorted_by_occupancy();
                break;
            case 6:
                display_sorted_by_revenue();
                break;
            case 0:
                save_data();
                printf("\nCleaning up and saving data...\n");
                cleanup();
                printf("Thank you for using Smart Parking System!\n");
                break;
            default:
                printf("\nInvalid choice. Please try again.\n");
        }
    } while (choice != 0);

    return 0;
}