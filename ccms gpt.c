#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_EVENTS 100
#define MAX_REGISTRATIONS 100
#define MAX_USERS 100
#define MAX_ANNOUNCEMENTS 5
#define MAX_NAME_LENGTH 100
#define MAX_DATE_LENGTH 100
#define MAX_INFO_LENGTH 256
#define MAX_CONTACT_LENGTH 15
#define MAX_EMAIL_LENGTH 100


// Structures

typedef struct {
    char name[MAX_NAME_LENGTH];
    int registrations_open;
} EventInfo;
typedef struct {
    char event_name[MAX_NAME_LENGTH];
    char participant_name[MAX_NAME_LENGTH];
    char contact[MAX_CONTACT_LENGTH];
    char email[MAX_EMAIL_LENGTH];
} ParticipantRegistration;
typedef struct record {
    int enroll;
    char name[25];
    char batch[3];
    int contact;
    char mail[50];
} record;
typedef struct {
    char name[MAX_NAME_LENGTH];
    char date[MAX_DATE_LENGTH];
    char info[MAX_INFO_LENGTH];
} event;
typedef struct interview_registration {
    int enrollment_no;   // Enrollment number
    char batch[3];      // Batch (e.g., "CS", "EE", etc.)
    char name[25];      // Name of the registrant
    char expertise[100]; // Area of expertise
    char contact[15];   // Contact number
    char email[50];     // Email address
} interview_registration;
typedef struct {
    char enrollment_number[25];
    int hours_worked;
} User;


// Menus

int menu2() {
    int choice;
    printf(" 1. Non Member\n 2. Volunteer \n 3. Core Team\n 4. Management\n");
    printf("Enter your Role- ");
    scanf("%d", &choice);
    if (choice < 1 || choice > 5) {
        printf("Invalid Choice");
        exit(1);
    }
    return choice;
}
int menu() {
    int choice;
    printf("\t** COLLEGE CLUB MANAGEMENT SYSTEM **\t\n");
    printf("1. Aakriti\n2. Abhivyakti\n3. Rapid Programming Hub.....\n4. JODC\n5. Eloquence\n6. Vamunique\n7. Ffortismo\n8. GDSC\n9. Aura\n10. Prismatic\n11. Panache\n\n");
    printf("Choose a Club - ");
    scanf("%d", &choice);
    if (choice < 1 || choice > 11) {
        printf("Invalid choice\n");
        exit(1);
    }
    return choice;
}


EventInfo events[MAX_EVENTS];
int event_count = 0;
User  users[MAX_USERS];
int user_count = 0;

void load_announcements_from_file(event vision[], int *count,char i[]) {
    FILE *file = fopen(i, "r");
    if (file == NULL) {
        printf("No existing announcements found. Starting fresh.\n");
        return;
    }

    while (*count < MAX_ANNOUNCEMENTS && fscanf(file, "Announcement Name: %[^\n]\n", vision[*count].name) == 1) {
        fscanf(file, "Announcement Date: %[^\n]\n", vision[*count].date);
        fscanf(file, "Announcement Info: %[^\n]\n\n", vision[*count].info);
        (*count)++;
    }

    fclose(file);
}
void save_all_announcements_to_file(event vision[], int count,char i[]) {
    FILE *file = fopen(i, "w"); // Open file in write mode
    if (file == NULL) {
        printf("Error opening file for writing!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(file, "Announcement Name: %s\n", vision[i].name);
        fprintf(file, "Announcement Date: %s\n", vision[i].date);
        fprintf(file, "Announcement Info: %s\n\n", vision[i].info);
    }

    fclose(file);
}
void Add_Anouncement(event vision[], int *count, char i[]) {
    while (1) {
        // Add the new announcement
        printf("Announcement Name - ");
        scanf("%s", vision[*count].name);
        getchar(); // Consume the newline character left by scanf
        printf("Announcement Date - ");
        scanf("%s", vision[*count].date);
        getchar(); // Consume the newline character left by scanf
        printf("Announcement Info - ");
        fgets(vision[*count].info, sizeof(vision[*count].info), stdin);

        (*count)++; // Increment the count after adding the announcement

        // Save all announcements to file
        save_all_announcements_to_file(vision, *count,i);

        char more;
        printf("Do you want to add another announcement? (y/n): ");
        scanf(" %c", &more);
        if (more != 'y') {
            break;
        }
    }
}
void ViewAnnouncements(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\nCurrent Announcements:\n");
    event vision;
    int count = 0;

    while (fgets(vision.name, sizeof(vision.name), file) != NULL) {
        // Remove newline character from name
        vision.name[strcspn(vision.name, "\n")] = 0;

        // Read date
        if (fgets(vision.date, sizeof(vision.date), file) == NULL) break;
        vision.date[strcspn(vision.date, "\n")] = 0;

        // Read info
        if (fgets(vision.info, sizeof(vision.info), file) == NULL) break;
        vision.info[strcspn(vision.info, "\n")] = 0;

        // Print the announcement
        count++;
        printf("Announcement %d:\n", count);
        printf("Announcement Name: %s\n", vision.name);
        printf("Announcement Date: %s\n", vision.date);
        printf("Announcement Info: %s\n\n", vision.info);
    }

    if (count == 0) {
        printf("No Announcements\n");
    }

    fclose(file);
}
void RegisterForInterview() {
    interview_registration reg;
    FILE *file = fopen("interview_registrations.csv", "a"); // Open CSV file in append mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    printf("Enter your Enrollment Number: ");
    scanf("%d", &reg.enrollment_no);
    printf("Enter your Batch: ");
    scanf("%s", reg.batch);
    printf("Enter your Name: ");
    scanf("%s", reg.name);
    printf("Enter your Expertise: ");
    scanf("%s", reg.expertise);
    printf("Enter your Contact Number: ");
    scanf("%s", reg.contact);
    printf("Enter your Email: ");
    scanf("%s", reg.email);

    // Write details to the CSV file
    fprintf(file, "%d,%s,%s,%s,%s,%s\n", reg.enrollment_no, reg.batch, reg.name, reg.expertise, reg.contact, reg.email);

    fclose(file);
    printf("Registration successful!\n");
}
void ViewRegisteredInterviews() {
    FILE *file = fopen("interview_registrations.csv", "r"); // Open CSV file in read mode
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char line[256];
    printf("\nRegistered Interviews:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line); // Print each line from the CSV file
    }

    fclose(file);
}
void CreateEvent() {
    if (event_count >= MAX_EVENTS) {
        printf("Maximum number of events reached.\n");
        return;
    }

    while (1) {
        printf("Enter the Event Name: ");
        fgets(events[event_count].name, sizeof(events[event_count].name), stdin);

        // Remove newline character if present
        size_t len = strlen(events[event_count].name);
        if (len > 0 && events[event_count].name[len - 1] == '\n') {
            events[event_count].name[len - 1] = '\0';
        }

        // Check if the event name is empty
        if (strlen(events[event_count].name) > 0) {
            break; // Valid input, exit the loop
        } else {
            printf("Event name cannot be empty. Please enter a valid event name.\n");
        }
    }

    events[event_count].registrations_open = 1; // Open registrations by default
    event_count++;

    printf("Event '%s' created with registrations open.\n", events[event_count - 1].name);
}
void ListOpenEvents() {
    printf("Open Events:\n");
    int found_open_event = 0;

    for (int i = 0; i < event_count; i++) {
        if (events[i].registrations_open) {
            printf("%d. %s\n", i + 1, events[i].name);
            found_open_event = 1;
        }
    }

    if (!found_open_event) {
        printf("No open events available.\n");
    }
}
void RegisterForEvent() {
    ListOpenEvents();

    int event_choice;
    printf("Select an event by number: ");
    scanf("%d", &event_choice);

    if (event_choice < 1 || event_choice > event_count || !events[event_choice - 1].registrations_open) {
        printf("Invalid choice or registrations are closed.\n");
        return;
    }

    ParticipantRegistration reg;
    strcpy(reg.event_name, events[event_choice - 1].name);

    printf("Enter your Name: ");
    scanf(" %[^\n]", reg.participant_name);
    printf("Enter your Contact Number: ");
    scanf("%s", reg.contact);
    printf("Enter your Email: ");
    scanf("%s", reg.email);

    // Save registration to CSV
    FILE *file = fopen("event_registrations.csv", "a");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }
    fprintf(file, "%s,%s,%s,%s\n", reg.event_name, reg.participant_name, reg.contact, reg.email);
    fclose(file);

    printf("Registration for event '%s' successful!\n", reg.event_name);
}
void ViewRegistrations() {
    char event_name[MAX_NAME_LENGTH];
    printf("Enter the Event Name to view registrations: ");
    scanf(" %[^\n]", event_name);

    FILE *file = fopen("event_registrations.csv", "r");
    if (file == NULL) {
        printf("Error opening file!\n");
        return;
    }

    char line[256];
    int found = 0;
    printf("Registered participants for event '%s':\n", event_name);
    while (fgets(line, sizeof(line), file)) {
        if (strstr(line, event_name) != NULL) {
            printf("%s", line); // Print each line that matches the event name
            found = 1;
        }
    }

    if (!found) {
        printf("No registrations found for event '%s'.\n", event_name);
    }

    fclose(file);
}
void LoadWorkingHours() {
    FILE *file = fopen("working_hours.csv", "r");
    if (file == NULL) {
        printf("Could not open file for reading.\n");
        return;
    }

    while (fscanf(file, "%[^,],%d\n", users[user_count].enrollment_number, &users[user_count].hours_worked) != EOF) {
        user_count++;
    }

    fclose(file);
}
void SaveWorkingHours() {
    FILE *file = fopen("working_hours.csv", "w");
    if (file == NULL) {
        printf("Could not open file for writing.\n");
        return;
    }

    for (int i = 0; i < user_count; i++) {
        fprintf(file, "%s,%d\n", users[i].enrollment_number, users[i].hours_worked);
    }

    fclose(file);
}
void trim_whitespace(char *str) {
    char *end;

    // Trim leading space
    while (isspace((unsigned char)*str)) str++;

    // Trim trailing space
    end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;

    // Null terminate after the last non-space character
    *(end + 1) = '\0';
}
void AddWorkingHours() {
    char enrollment_number[25];
    int hours;

    printf("Enter your Enrollment Number: ");
    scanf(" %[^\n]", enrollment_number);
    trim_whitespace(enrollment_number); // Trim whitespace
    printf("Enter hours worked: ");
    scanf("%d", &hours);

    // Variable to track if the user is found
    int user_found = 0;

    // Check if user already exists
    for (int i = 0; i < user_count; i++) {
        if (strcmp(users[i].enrollment_number, enrollment_number) == 0) {
            // User found, increment hours to their total
            users[i].hours_worked += hours; // Incrementing the existing hours
            printf("Added %d hours to enrollment number %s. Total hours: %d.\n",
                   hours, enrollment_number, users[i].hours_worked);
            user_found = 1; // Mark user as found
            break; // Exit the loop
        }
    }

    // If user was not found, add a new entry
    if (!user_found) {
        if (user_count < MAX_USERS) {
            strcpy(users[user_count].enrollment_number, enrollment_number);
            users[user_count].hours_worked = hours; // Set initial hours for the new user
            user_count++;
            printf("Added enrollment number %s with %d hours worked.\n",
                   enrollment_number, hours);
        } else {
            printf("Maximum number of users reached.\n");
        }
    }

    // Save updated data to CSV
    SaveWorkingHours();
}
int NonMember(event vision[], int count, char i[]) {
    int choice;
    printf("1. View Announcement\n2. Register for an Interview\n3. Register for Event\nEnter your choice - ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            ViewAnnouncements(i);
            return 0;
        case 2:
           RegisterForInterview();
            return 0;
        case 3:
            RegisterForEvent();
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}
int Volunteer(event vision[], int count, char i[]) {
    int choice;
    printf("1. View Announcement\n2. Register for an Interview\n3. Register for Event\n4. View Event Registrations \n5. Add Working Hours \nEnter your choice - ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            ViewAnnouncements(i);
            return 0;
        case 2:
           RegisterForInterview();
            return 0;
        case 3:
            RegisterForEvent();
            return 0;
        case 4:
            ViewRegistrations();
            return 0;
        case 5:
            AddWorkingHours();
        default:
            printf("Invalid choice\n");
            return 0;
    }

}
int CoreTeam(event vision[], int count,char i[]){
    int choice;
    printf("1. View Event Registrations \n2. Add Working Hours\n3. View Interview Registrations\n4. Add Announcements \nEnter your choice - ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            ViewRegistrations();
            return 0;
        case 2:
            AddWorkingHours();
            return 0;
        case 3:
            ViewRegisteredInterviews();
            return 0;
        case 4:
            Add_Anouncement(vision, &count,i);
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}
int Management(event vision[], int count, char i[]){
    int choice;
    printf("1. View Event Registrations \n2. View Interview Registrations\n3. Add Announcements \n4.Create Event\nEnter your choice - ");
    scanf("%d", &choice);

    switch (choice) {
        case 1:
            ViewRegistrations();
            return 0;
        case 2:
            ViewRegisteredInterviews();
            return 0;
        case 3:
            Add_Anouncement(vision, &count, i);
            return 0;
        case 4:
            CreateEvent();
            return 0;
        case 5:
            AddWorkingHours();
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int Aakriti() {

    event aakriti[5];
    char i[25]="announcements_aakriti.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(aakriti, &count,i);

    switch (choice) {
        case 1:
            NonMember(aakriti, count,i);
            return 0;
        case 2:
            Volunteer(aakriti, count,i);
            return 0;
        case 3:
            CoreTeam(aakriti,count,i);
            return 0;
        case 4:
            Management(aakriti,count,i);
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int Abhivyakti(){

    event abhivyakti[5];
    char i[25]="announcements_abhvyakti.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(abhivyakti, &count,i);

    switch (choice) {
        case 1:
            NonMember(abhivyakti, count,i);
            return 0;
        case 2:
            Volunteer(abhivyakti, count,i);
            return 0;
        case 3:
            CoreTeam(abhivyakti,count,i);
            return 0;
        case 4:
            Management(abhivyakti,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int RPH(){
    event RPH[5];
    char i[25]="announcements_RPH.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(RPH, &count,i);

    switch (choice) {
        case 1:
            NonMember(RPH, count,i);
            return 0;
        case 2:
            Volunteer(RPH, count,i);
            return 0;
        case 3:
            CoreTeam(RPH,count,i);
            return 0;
        case 4:
            Management(RPH,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int JODC(){
     event JODC[5];
    char i[25]="announcements_JODC.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(JODC, &count,i);

    switch (choice) {
        case 1:
            NonMember(JODC, count,i);
            return 0;
        case 2:
            Volunteer(JODC, count,i);
            return 0;
        case 3:
            CoreTeam(JODC,count,i);
            return 0;
        case 4:
            Management(JODC,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int Eloquence(){
 event Eloquence[5];
    char i[25]="announcements_Eloquence.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(Eloquence, &count,i);

    switch (choice) {
        case 1:
            NonMember(Eloquence, count,i);
            return 0;
        case 2:
            Volunteer(Eloquence, count,i);
            return 0;
        case 3:
            CoreTeam(Eloquence,count,i);
            return 0;
        case 4:
            Management(Eloquence,count,i);
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }
}

int Vamunique(){
 event Vamunique[5];
    char i[25]="announcements_Vamunique.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(Vamunique, &count,i);

    switch (choice) {
        case 1:
            NonMember(Vamunique, count,i);
            return 0;
        case 2:
            Volunteer(Vamunique, count,i);
            return 0;
        case 3:
            CoreTeam(Vamunique,count,i);
            return 0;
        case 4:
            Management(Vamunique,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }}

int Ffortissimo(){
 event Ffortissimo[5];
    char i[25]="announcements_Ffortissimo.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(Ffortissimo, &count,i);

    switch (choice) {
        case 1:
            NonMember(Ffortissimo, count,i);
            return 0;
        case 2:
            Volunteer(Ffortissimo, count,i);
            return 0;
        case 3:
            CoreTeam(Ffortissimo,count,i);
            return 0;
        case 4:
            Management(Ffortissimo,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }}

int GDSC(){
 event GDSC[5];
    char i[25]="announcements_GDSC.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(GDSC, &count,i);

    switch (choice) {
        case 1:
            NonMember(GDSC, count,i);
            return 0;
        case 2:
            Volunteer(GDSC, count,i);
            return 0;
        case 3:
            CoreTeam(GDSC,count,i);
            return 0;
        case 4:
            Management(GDSC,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }}

int Aura(){
 event Aura[5];
    char i[25]="announcements_Aura.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(Aura, &count,i);

    switch (choice) {
        case 1:
            NonMember(Aura, count,i);
            return 0;
        case 2:
            Volunteer(Aura, count,i);
            return 0;
        case 3:
            CoreTeam(Aura,count,i);
            return 0;
        case 4:
            Management(Aura,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }}

int Prismatic(){
 event Prismatic[5];
    char i[25]="announcements_Prismatic.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(Prismatic, &count,i);

    switch (choice) {
        case 1:
            NonMember(Prismatic, count,i);
            return 0;
        case 2:
            Volunteer(Prismatic, count,i);
            return 0;
        case 3:
            CoreTeam(Prismatic,count,i);
            return 0;
        case 4:
            Management(Prismatic,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }}

int Panche(){
 event Panche[5];
    char i[25]="announcements_Panche.txt";
    int count = 0; // Track the number of announcements
    int choice = menu2();
    load_announcements_from_file(Panche, &count,i);

    switch (choice) {
        case 1:
            NonMember(Panche, count,i);
            return 0;
        case 2:
            Volunteer(Panche, count,i);
            return 0;
        case 3:
            CoreTeam(Panche,count,i);
            return 0;
        case 4:
            Management(Panche,count,i
                       );
            return 0;
        default:
            printf("Invalid choice\n");
            return 0;
    }}

int choice1_func(int choice) {
    switch (choice) {
        case 1:
            return Aakriti();
        case 2:
            return Abhivyakti();
        case 3:
            return RPH();
        case 4:
            return JODC();
        case 5:
            return Eloquence();
        case 6:
            return Vamunique();
        case 7:
            return Ffortissimo();
        case 8:
            return GDSC();
        case 9:
            return Aura();
        case 10:
            return Prismatic();
        case 11:
            return Panche();
        default:
            printf("Invalid choice\n");
            break;
    }
    return 0; // Added return statement
}


int main() {
        char c='1';
    do{

        printf("To Exit - 0 \n Else press any other key");
        scanf("%c",&c);
        int choice = menu();
        choice1_func(choice);
    }while(c!=0);
    printf("\n\nThankyou");

    return 0;
}
