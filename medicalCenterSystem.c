#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char adminEmail[50], adminPass[50];

// Define color codes
#define RESET "\033[0m"
#define RED "\033[31m"
#define GREEN "\033[32m"
#define YELLOW "\033[33m"
#define BLUE "\033[34m"
#define CYAN "\033[36m"
#define BOLD "\033[1m"
#define UNDERLINE "\033[4m"

// Structure for storing patient information
typedef struct
{
    int id;
    char name[50];
    char email[50];
    char password[20];
} Patient;

// Structure for storing appointment information
typedef struct
{
    int appointmentId;
    int patientId;
    char doctorName[50];
    char date[15];
    char time[10];
} Appointment;

// File paths
const char *PATIENT_FILE = "patients.txt";
const char *APPOINTMENT_FILE = "appointments.txt";

// Function prototypes
void registerPatient();
int loginPatient();
void updatePatientInfo(int patientId);
void viewPatientDetails(int patientId);
void scheduleAppointment(int patientId);
void viewAppointments(int patientId);
void generateReceipt(int appointmentId);
void paymentGateway(int appointmentId);
void viewAllPatientRecords();
void viewAllAppointmentDetails();
void mainMenu(int);
void patientMenu(int patientId);
void printHeader(const char *title);

// Function to print the system header
void printHeader(const char *title)
{
    printf("\n" CYAN "===============================\n" RESET);
    printf(CYAN "%s\n", title);
    printf("===============================\n" RESET);
}

// Function to register a new patient
void registerPatient()
{
    system("cls");
    FILE *file = fopen(PATIENT_FILE, "a");
    if (!file)
    {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    Patient patient;
    printHeader("=== Register New Patient ===");

    printf(YELLOW "Enter ID: " RESET);
    scanf("%d", &patient.id);
    printf(YELLOW "Enter Name: " RESET);
    scanf(" %[^\n]", patient.name);
    printf(YELLOW "Enter Email: " RESET);
    scanf(" %[^\n]", patient.email);
    printf(YELLOW "Enter Password: " RESET);
    scanf(" %[^\n]", patient.password);

    fwrite(&patient, sizeof(Patient), 1, file);
    fclose(file);

    printf(GREEN "Registration successful! Welcome, %s.\n" RESET, patient.name);
}

// Function to log in a patient
int loginPatient()
{
    system("cls");
    FILE *file = fopen(PATIENT_FILE, "r");
    if (!file)
    {
        printf(RED "Error opening file! Make sure to register first.\n" RESET);
        return 0;
    }

    char email[50], password[20];
    printHeader("=== Login ===");
    printf(YELLOW "Enter Email: " RESET);
    scanf(" %[^\n]", email);
    printf(YELLOW "Enter Password: " RESET);
    scanf(" %[^\n]", password);

    Patient patient;
    while (fread(&patient, sizeof(Patient), 1, file))
    {
        if (strcmp(patient.email, email) == 0 && strcmp(patient.password, password) == 0)
        {
            printf(GREEN "Login successful! Welcome, %s\n" RESET, patient.name);
            fclose(file);
            return patient.id; // Return the patient ID for further operations
        }
    }

    printf(RED "Invalid email or password.\n" RESET);
    fclose(file);
    return 0;
}

int adminLogin()
{
    system("cls");
    
    char email[50], password[20];
    printHeader("=== Admin Login ===");
    printf(YELLOW "Enter Email: " RESET);
    scanf(" %[^\n]", email);
    printf(YELLOW "Enter Password: " RESET);
    scanf(" %[^\n]", password);

    if(strcmp(adminEmail, email) == 0 && strcmp(adminPass, password) == 0) return 1;
    
    return 0;
}

// Function to update patient information
void updatePatientInfo(int patientId)
{
    FILE *file = fopen(PATIENT_FILE, "r+");
    if (!file)
    {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    Patient patient;
    int found = 0;
    long pos;

    while ((pos = ftell(file)) >= 0 && fread(&patient, sizeof(Patient), 1, file))
    {
        if (patient.id == patientId)
        {
            found = 1;
            printHeader("=== Update Patient Info ===");
            printf(YELLOW "Current Name: %s\n" RESET, patient.name);
            printf(YELLOW "Enter New Name: " RESET);
            scanf(" %[^\n]", patient.name);

            printf(YELLOW "Current Email: %s\n" RESET, patient.email);
            printf(YELLOW "Enter New Email: " RESET);
            scanf(" %[^\n]", patient.email);

            fseek(file, pos, SEEK_SET);
            fwrite(&patient, sizeof(Patient), 1, file);
            break;
        }
    }

    if (!found)
    {
        printf(RED "Patient record not found!\n" RESET);
    }
    else
    {
        printf(GREEN "Information updated successfully!\n" RESET);
    }

    fclose(file);
}

// Function to view patient details
void viewPatientDetails(int patientId)
{
    FILE *file = fopen(PATIENT_FILE, "r");
    if (!file)
    {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    Patient patient;
    int found = 0;

    printHeader("=== Patient Details ===");
    while (fread(&patient, sizeof(Patient), 1, file))
    {
        if (patient.id == patientId)
        {
            found = 1;
            printf(YELLOW "ID: %d\n" RESET, patient.id);
            printf(YELLOW "Name: %s\n" RESET, patient.name);
            printf(YELLOW "Email: %s\n" RESET, patient.email);
            break;
        }
    }

    if (!found)
    {
        printf(RED "Patient record not found!\n" RESET);
    }

    fclose(file);
}

// Function to schedule an appointment
void scheduleAppointment(int patientId)
{
    const char *doctors[] = {
        "Dr. Mahbubur Rahman", "Dr. Masud", "Dr. Farjana Begum", "Dr. Tanvir Rahman",
        "Dr. Toha Islam", "Dr. Tonu Chowdhury", "Dr. Amatullah Amin", "Dr. Yalahi Rahman",
        "Dr. Ahil Chowdhury", "Dr. S.M Saiful Haque", "Dr. Marzia Sharmin", "Dr. Nargis Akter"};
    int numDoctors = sizeof(doctors) / sizeof(doctors[0]);

    FILE *file = fopen(APPOINTMENT_FILE, "a");
    if (!file)
    {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    Appointment appointment;
    appointment.patientId = patientId;

    printHeader("=== Schedule Appointment ===");
    printf(YELLOW "Enter Appointment ID: " RESET);
    scanf("%d", &appointment.appointmentId);

    printf(YELLOW "\nSelect a Doctor by number:\n" RESET);
    for (int i = 0; i < numDoctors; i++)
    {
        printf("%d. %s\n", i + 1, doctors[i]);
    }

    int doctorChoice;
    printf(YELLOW "Enter your choice: " RESET);
    scanf("%d", &doctorChoice);

    if (doctorChoice < 1 || doctorChoice > numDoctors)
    {
        printf(RED "Invalid doctor choice.\n" RESET);
        fclose(file);
        return;
    }

    strcpy(appointment.doctorName, doctors[doctorChoice - 1]);

    printf(YELLOW "Enter Date (DD/MM/YYYY): " RESET);
    scanf(" %[^\n]", appointment.date);
    printf(YELLOW "Enter Time (HH:MM): " RESET);
    scanf(" %[^\n]", appointment.time);

    fwrite(&appointment, sizeof(Appointment), 1, file);
    fclose(file);

    printf(GREEN "Appointment scheduled successfully with Dr. %s!\n" RESET, appointment.doctorName);
}

// Function to view appointments
void viewAppointments(int patientId)
{
    FILE *file = fopen(APPOINTMENT_FILE, "r");
    if (!file)
    {
        printf(RED "Error opening file!\n" RESET);
        return;
    }

    Appointment appointment;
    int found = 0;

    printHeader("=== Your Appointments ===");
    while (fread(&appointment, sizeof(Appointment), 1, file))
    {
        if (appointment.patientId == patientId)
        {
            found = 1;
            printf(CYAN "Appointment ID: %d\n" RESET, appointment.appointmentId);
            printf(CYAN "Doctor's Name: %s\n" RESET, appointment.doctorName);
            printf(CYAN "Date: %s\n" RESET, appointment.date);
            printf(CYAN "Time: %s\n\n" RESET, appointment.time);
        }
    }

    if (!found)
    {
        printf(RED "No appointments found.\n" RESET);
    }

    fclose(file);
}

// Function to generate a payment receipt
void generateReceipt(int appointmentId)
{
    printHeader("=== Payment Receipt ===");
    printf(YELLOW "Appointment ID: %d\n" RESET, appointmentId);
    printf(YELLOW "Amount to be paid: 500 BDT\n" RESET); // Example static amount
}

// Function to handle payment gateway
void paymentGateway(int appointmentId)
{
    int choice;
    float amount = 500.0; // Amount from generateReceipt

    while (1)
    {
        printHeader("=== Payment Gateway ===");
        printf("Choose an option for payment\n");
        printf("1. Visa\n");
        printf("2. Mastercard\n");
        printf("3. DBBL\n");
        printf("4. Mobile Banking\n");
        printf("5. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 5)
        {
            return;
        }

        printf(CYAN "\nConsultation Fee for the Doctor: %.2f BDT\n" RESET, amount);
        int action;
        printf("1. Pay\n");
        printf("2. Back\n");
        printf("Enter your choice: ");
        scanf("%d", &action);

        if (action == 1)
        {
            printf(GREEN "\nPayment Successful! Thank you for using our service.\n" RESET);
            break;
        }
        else if (action == 2)
        {
            break;
        }
        else
        {
            printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}
void viewAllPatientRecords() {
    FILE *file = fopen(PATIENT_FILE, "r");
    if (!file) {
        printf(RED "Error opening patient file!\n" RESET);
        return;
    }

    printHeader("=== All Patient Records ===");
    Patient patient;
    while (fread(&patient, sizeof(Patient), 1, file)) {
        printf(YELLOW "ID: %d\nName: %s\nEmail: %s\n\n" RESET, patient.id, patient.name, patient.email);
    }

    fclose(file);
}

void viewAllAppointmentDetails() {
    FILE *file = fopen(APPOINTMENT_FILE, "r");
    if (!file) {
        printf(RED "Error opening appointment file!\n" RESET);
        return;
    }

    printHeader("=== All Appointment Details ===");
    Appointment appointment;
    while (fread(&appointment, sizeof(Appointment), 1, file)) {
        printf(YELLOW "Appointment ID: %d\nPatient ID: %d\nDoctor's Name: %s\nDate: %s\nTime: %s\n\n" RESET,
               appointment.appointmentId, appointment.patientId, appointment.doctorName, appointment.date, appointment.time);
    }

    fclose(file);
}

// Patient menu
void patientMenu(int patientId)
{
    int choice;
    while (1)
    {
        printHeader("=== Patient Menu ===");
        printf("1. Update Information\n");
        printf("2. View Details\n");
        printf("3. Schedule Appointment\n");
        printf("4. View Appointments\n");
        printf("5. Generate Payment Receipt\n");
        printf("6. Payment Gateway\n");
        printf("7. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            updatePatientInfo(patientId);
            break;
        case 2:
            viewPatientDetails(patientId);
            break;
        case 3:
            scheduleAppointment(patientId);
            break;
        case 4:
            viewAppointments(patientId);
            break;
        case 5:
            generateReceipt(patientId); // You can modify the appointmentId logic if necessary
            break;
        case 6:
            paymentGateway(patientId);
            break;
        case 7:
            printf(GREEN "Logged out successfully!\n" RESET);
            return;
        default:
            printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}


void adminMenu() {
    system("cls");
    int choice;
    while (1) {
        printHeader("=== Admin Menu ===");
        printf("1. View All Patient Records\n");
        printf("2. View All Appointment Details\n");
        printf("3. Logout\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                viewAllPatientRecords();
                break;
            case 2:
                viewAllAppointmentDetails();
                break;
            case 3:
                printf(GREEN "Logged out successfully!\n" RESET);
                return;
            default:
                printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}

// Main menu
void mainMenu(int adminSignal)
{
    system("cls");
    if(adminSignal == 1) printf(RED "Invalid admin credentials. Please try again.\n" RESET);
    int choice;
    while (1)
    {
        printHeader("=== Main Menu ===");
        printf("1. Register as Patient\n");
        printf("2. Patient Login\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            registerPatient();
            break;
        case 2:
        {
            int patientId = loginPatient();
            if (patientId > 0)
            {
                patientMenu(patientId);
            }
        }
        break;
        case 3:
        {
            int check = adminLogin();
            if(check > 0) adminMenu();
            else {
                mainMenu(1);
            }
            break;
        }
        case 4:
            printf(GREEN "Exiting the system. Goodbye!\n" RESET);
            exit(0);
        default:
            printf(RED "Invalid choice. Please try again.\n" RESET);
        }
    }
}

int main()
{
    strcpy(adminEmail, "admin@gmail.com");
    strcpy(adminPass, "1234");
    // Start the application by showing the main menu
    mainMenu(0);
    return 0;
}