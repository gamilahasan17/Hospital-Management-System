#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <queue>
using namespace std;

// ========== ENUMERATIONS ========== //
enum Department {
    CARDIOLOGY,
    NEUROLOGY,
    ORTHOPEDICS,
    PEDIATRICS,
    EMERGENCY,
    GENERAL
};

enum RoomType {
    GENERAL_WARD,
    ICU,
    PRIVATE_ROOM,
    SEMI_PRIVATE
};

// ========== EMERGENCY CASE CLASS ========== //
class EmergencyCase {
private:
    int patientId;
    int severity;

public:
    EmergencyCase(int pid, int s) {
        this->patientId = pid;
        this->severity = s;
    }

    int getPatientId() const {
        return patientId;
    }
    int getSeverity() const {
        return severity;
    }

    // Higher severity = higher priority
    bool operator<(const EmergencyCase& other) const {
        return severity < other.severity;
    }
};


// ========== PATIENT CLASS ========== //
class Patient {
private:
    int id;
    string name;
    int age;
    string contact;

    // Data Structures
    stack<string> medicalHistory;
    queue<string> testQueue;
    vector<string> prescriptions;

    bool isAdmitted;
    RoomType roomType;

    // Advanced Feature: Billing
    double bill;

public:
    // Constructor
    Patient(int pid, string n, int a, string c) {
        this->id = pid;
        this->name = n;
        this->age = a;
        this->contact = c;
        this->isAdmitted = false;
        this->bill = 0;
    }

    // ========== ORIGINAL FEATURES ========== //

    void admitPatient(RoomType type) {
        if (isAdmitted) {
            cout << "Patient is already admitted." << endl;
            return;
        }

        isAdmitted = true;
        roomType = type;
        addMedicalRecord("Patient admitted to hospital");

        switch (type) {
            case GENERAL_WARD:
                bill += 500;
                break;
            case ICU:
                bill += 3000;
                break;
            case PRIVATE_ROOM:
                bill += 1500;
                break;
            case SEMI_PRIVATE:
                bill += 1000;
                break;
        }
    }

    void dischargePatient() {
        if (!isAdmitted) {
            cout << "Patient is not currently admitted." << endl;
            return;
        }

        isAdmitted = false;
        addMedicalRecord("Patient discharged from hospital");
    }

    void addMedicalRecord(string record) {
        medicalHistory.push(record);
    }

    void requestTest(string testName) {
        testQueue.push(testName);
        addMedicalRecord("Test requested: " + testName);
        cout << "Test requested successfully." << endl;
    }

    string performTest() {
        if (testQueue.empty()) {
            return "No tests pending";
        }

        string test = testQueue.front();
        testQueue.pop();
        bill += 300;
        addMedicalRecord("Test performed: " + test);

        return test;
    }

    void displayHistory() {
        cout << "Medical History for " << name << " (ID: " << id << "):" << endl;

        stack<string> temp = medicalHistory;
        while (!temp.empty()) {
            cout << temp.top() << endl;
            temp.pop();
        }
    }

    int getId() {
        return id;
    }
    string getName() {
        return name;
    }

    bool getAdmissionStatus() {
        return isAdmitted;
    }


    // ========== NEW FEATURES ========== //

    // Medical Tests
    void displayPendingTests() {
        cout << "Pending Tests:" << endl;

        if (testQueue.empty()) {
            cout << "No tests pending." << endl;
            return;
        }

        queue<string> temp = testQueue;
        while (!temp.empty()) {
            cout << temp.front() << endl;
            temp.pop();
        }
    }

    // Prescriptions
    void addPrescription(string medicine) {
        prescriptions.push_back(medicine);
        bill += 100;
        addMedicalRecord("Prescription added: " + medicine);
        cout << "Medicine prescribed successfully." << endl;
    }

    void displayPrescriptions() {
        cout << "Prescriptions:" << endl;

        if (prescriptions.empty()) {
            cout << "No prescriptions." << endl;
            return;
        }

        for (string &med : prescriptions) {
            cout << med << endl;
        }
    }

    // Billing
    void addBill(double amount) {
        bill += amount;
    }

    double getBill() {
        return bill;
    }

    void displayBill() {
        cout << "========== PATIENT BILL ==========" << endl;
        cout << "Patient ID: " << id << endl;
        cout << "Patient Name: " << name << endl;
        cout << "Total Bill: $" << bill << endl;
        cout << "==================================" << endl;
    }

    // Additional Getters
    int getAge() {
        return age;
    }
    string getContact() {
        return contact;
    }
    RoomType getRoomType() {
        return roomType;
    }
};


// ========== DOCTOR CLASS ========== //
class Doctor {
private:
    int id;
    string name;
    Department department;

    // Queue of patients waiting for doctor
    queue<int> appointmentQueue;

public:
    // Constructor
    Doctor(int did, string n, Department d) {
        this->id = did;
        this->name = n;
        this->department = d;
    }

    // ========== ORIGINAL FEATURES ========== //

    void addAppointment(int patientId) {
        appointmentQueue.push(patientId);
    }

    int seePatient() {
        if (appointmentQueue.empty()) {
            return -1;
        }
        else {
            int patientId = appointmentQueue.front();
            appointmentQueue.pop();
            return patientId;
        }
    }

    int getId() {
        return id;
    }
    string getName() {
        return name;
    }

    string getDepartment() {
        switch (department) {
            case CARDIOLOGY:
                return "Cardiology";
            case NEUROLOGY:
                return "Neurology";
            case ORTHOPEDICS:
                return "Orthopedics";
            case PEDIATRICS:
                return "Pediatrics";
            case EMERGENCY:
                return "Emergency";
            case GENERAL:
                return "General";
            default:
                return "General";
        }
    }


    // ========== NEW FEATURES ========== //

    // Display waiting patients
    void displayAppointments() {
        if (appointmentQueue.empty()) {
            cout << "No appointments." << endl;
            return;
        }

        queue<int> temp = appointmentQueue;
        cout << "Appointment Queue:" << endl;

        while (!temp.empty()) {
            cout << "- Patient ID: " << temp.front() << endl;
            temp.pop();
        }
    }

    // Cancel appointment
    void cancelAppointment(int patientId) {
        if (appointmentQueue.empty()) {
            cout << "No appointments available." << endl;
            return;
        }

        queue<int> temp;
        bool found = false;

        while (!appointmentQueue.empty()) {
            int currentPatientId = appointmentQueue.front();
            appointmentQueue.pop();

            if (!found && currentPatientId == patientId) {
                found = true;
            } else {
                temp.push(currentPatientId);
            }
        }

        appointmentQueue = temp;

        if (found) {
            cout << "Appointment cancelled successfully." << endl;
        } else {
            cout << "Appointment not found." << endl;
        }
    }

    // Number of waiting patients
    int getAppointmentCount() {
        return appointmentQueue.size();
    }
};


// ========== HOSPITAL CLASS ========== //
class Hospital {
private:
    // Main collections
    vector<Patient> patients;
    vector<Doctor> doctors;

    // Original emergency queue
    queue<int> emergencyQueue;

    // Advanced emergency queue
    priority_queue<EmergencyCase> priorityEmergencyQueue;

    // Counters
    int patientCounter;
    int doctorCounter;

    // ========== ROOM MANAGEMENT ========== //
    int generalRooms;
    int icuRooms;
    int privateRooms;
    int semiPrivateRooms;

public:
    // Constructor
    Hospital() {
        patientCounter = 1;
        doctorCounter = 1;

        generalRooms = 20;
        icuRooms = 5;
        privateRooms = 10;
        semiPrivateRooms = 10;
    }

    int registerPatient(string name, int age, string contact) {
        Patient newPatient(patientCounter, name, age, contact);
        patients.push_back(newPatient);
        return patientCounter++;
    }

    int addDoctor(string name, Department dept) {
        Doctor newDoctor(doctorCounter, name, dept);
        doctors.push_back(newDoctor);
        return doctorCounter++;
    }

    void admitPatient(int patientId, RoomType type) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            if (patient->getAdmissionStatus()) {
                cout << "Patient is already admitted." << endl;
                return;
            }
            if (!isRoomAvailable(type)) {
                cout << "No room available for this room type." << endl;
                return;
            }
            patient->admitPatient(type);
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void bookAppointment(int doctorId, int patientId) {
        Doctor* doctor = findDoctor(doctorId);
        Patient* patient = findPatient(patientId);

        if (doctor == nullptr && patient == nullptr) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
            cout << "Patient with ID " << patientId << " not found." << endl;
            return;
        }
        if (doctor == nullptr) {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
            return;
        }
        if (patient == nullptr) {
            cout << "Patient with ID " << patientId << " not found." << endl;
            return;
        }

        doctor->addAppointment(patientId);
        cout << "Appointment booked for patient " << patientId << " with doctor " << doctorId << endl;
    }

    void cancelAppointment(int doctorId, int patientId) {
        Doctor* doctor = findDoctor(doctorId);
        if (doctor != nullptr) {
            doctor->cancelAppointment(patientId);
        } else {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
        }
    }

    void doctorSeePatient(int doctorId) {
        Doctor* doctor = findDoctor(doctorId);
        if (doctor != nullptr) {
            int patientId = doctor->seePatient();
            if (patientId == -1) {
                cout << "No patients waiting." << endl;
            } else {
                cout << "Dr. " << doctor->getName() << " is now seeing patient " << patientId << endl;
            }
        } else {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
        }
    }

    void displayDoctorAppointments(int doctorId) {
        Doctor* doctor = findDoctor(doctorId);
        if (doctor != nullptr) {
            cout << "Appointments for " << doctor->getName() << ":" << endl;
            doctor->displayAppointments();
        } else {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
        }
    }

    void addEmergency(int patientId) {
        emergencyQueue.push(patientId);
    }

    int handleEmergency() {
        if (emergencyQueue.empty()) {
            cout << "No emergencies in queue." << endl;
            return -1;
        }
        int pid = emergencyQueue.front();
        emergencyQueue.pop();
        cout << "Handled emergency for patient: " << pid << endl;
        return pid;
    }

    void addPriorityEmergency(int patientId, int severity) {
        Patient* patient = findPatient(patientId);
        if (patient == nullptr) {
            cout << "Patient with ID " << patientId << " not found." << endl;
            return;
        }
        if (severity < 1 || severity > 5) {
            cout << "Invalid severity level." << endl;
            return;
        }
        priorityEmergencyQueue.push(EmergencyCase(patientId, severity));
        cout << "Emergency added with severity " << severity << endl;
    }

    int handlePriorityEmergency() {
        if (priorityEmergencyQueue.empty()) {
            cout << "No priority emergencies." << endl;
            return -1;
        }
        EmergencyCase topCase = priorityEmergencyQueue.top();
        priorityEmergencyQueue.pop();
        cout << "Handling patient " << topCase.getPatientId()
             << " with severity " << topCase.getSeverity() << endl;
        return topCase.getPatientId();
    }

    Patient* findPatient(int patientId) {
        for (auto& patient : patients) {
            if (patient.getId() == patientId) {
                return &patient;
            }
        }
        return nullptr;
    }

    Doctor* findDoctor(int doctorId) {
        for (auto& doctor : doctors) {
            if (doctor.getId() == doctorId) {
                return &doctor;
            }
        }
        return nullptr;
    }

    void displayPatientInfo(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            cout << "Patient Information:" << endl;
            cout << "ID: " << patient->getId() << endl;
            cout << "Name: " << patient->getName() << endl;
            cout << "Admission Status: " << (patient->getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
            patient->displayHistory();
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayDoctorInfo(int doctorId) {
        Doctor* doctor = findDoctor(doctorId);
        if (doctor != nullptr) {
            cout << "Doctor Information:" << endl;
            cout << "ID: " << doctor->getId() << endl;
            cout << "Name: " << doctor->getName() << endl;
            cout << "Department: " << doctor->getDepartment() << endl;
        } else {
            cout << "Doctor with ID " << doctorId << " not found." << endl;
        }
    }

    void searchPatientByName(string name) {
        bool found = false;
        for (auto& patient : patients) {
            if (patient.getName() == name) {
                cout << "Patient Found:" << endl;
                cout << "ID: " << patient.getId() << endl;
                cout << "Name: " << patient.getName() << endl;
                cout << "Age: " << patient.getAge() << endl;
                cout << "Contact: " << patient.getContact() << endl;
                found = true;
            }
        }
        if (!found) {
            cout << "Patient not found." << endl;
        }
    }

    void displayAllPatients() {
        cout << "========== ALL PATIENTS ==========" << endl;
        for (auto& patient : patients) {
            cout << "ID: " << patient.getId()
                 << " | Name: " << patient.getName()
                 << " | Age: " << patient.getAge()
                 << " | Status: " << (patient.getAdmissionStatus() ? "Admitted" : "Not Admitted") << endl;
        }
    }

    void displayAllDoctors() {
        cout << "========== ALL DOCTORS ==========" << endl;
        for (auto& doctor : doctors) {
            cout << "ID: " << doctor.getId()
                 << " | Name: " << doctor.getName()
                 << " | Department: " << doctor.getDepartment()
                 << " | Appointments: " << doctor.getAppointmentCount() << endl;
        }
    }

    void displayStatistics() {
        int admittedCount = 0;
        double totalBilled = 0;

        for (auto& patient : patients) {
            if (patient.getAdmissionStatus()) {
                admittedCount++;
            }
            totalBilled += patient.getBill();
        }

        cout << "========== HOSPITAL STATISTICS ==========" << endl;
        cout << "Total Patients: " << patients.size() << endl;
        cout << "Total Doctors: " << doctors.size() << endl;
        cout << "Admitted Patients: " << admittedCount << endl;
        cout << "Waiting Emergencies: " << emergencyQueue.size() << endl;
        cout << "Priority Emergencies: " << priorityEmergencyQueue.size() << endl;
        cout << "Total Generated Bills: $" << totalBilled << endl;
        cout << "=========================================" << endl;

        cout << "Final Patient Bills:" << endl;
        for (auto& patient : patients) {
            patient.displayBill();
        }
    }

    void dischargePatient(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            patient->dischargePatient();
            cout << "Patient discharged successfully." << endl;
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void requestPatientTest(int patientId, string testName) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            patient->requestTest(testName);
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void performPatientTest(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            string result = patient->performTest();
            cout << "Test result/action: " << result << endl;
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayPatientTests(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            patient->displayPendingTests();
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void prescribeMedicine(int patientId, string medicine) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            patient->addPrescription(medicine);
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayPrescriptions(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            patient->displayPrescriptions();
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    void displayPatientBill(int patientId) {
        Patient* patient = findPatient(patientId);
        if (patient != nullptr) {
            patient->displayBill();
        } else {
            cout << "Patient with ID " << patientId << " not found." << endl;
        }
    }

    bool isRoomAvailable(RoomType type) {
        switch (type) {
            case GENERAL_WARD:
                return generalRooms > 0;
            case ICU:
                return icuRooms > 0;
            case PRIVATE_ROOM:
                return privateRooms > 0;
            case SEMI_PRIVATE:
                return semiPrivateRooms > 0;
            default:
                return false;
        }
    }

    void displayRoomStatus() {
        cout << "========== ROOM STATUS ==========" << endl;
        cout << "General Ward: " << generalRooms << endl;
        cout << "ICU: " << icuRooms << endl;
        cout << "Private Rooms: " << privateRooms << endl;
        cout << "Semi Private Rooms: " << semiPrivateRooms << endl;
    }
};


// ========== MAIN PROGRAM ========== //
int main() {

    Hospital hospital;

    // TEST CASE 1: Registering patients
    int p1 = hospital.registerPatient("John Doe", 35, "555-1234");
    int p2 = hospital.registerPatient("Jane Smith", 28, "555-5678");
    int p3 = hospital.registerPatient("Mike Johnson", 45, "555-9012");

    // TEST CASE 2: Adding doctors
    int d1 = hospital.addDoctor("Dr. Smith", CARDIOLOGY);
    int d2 = hospital.addDoctor("Dr. Brown", NEUROLOGY);
    int d3 = hospital.addDoctor("Dr. Lee", PEDIATRICS);

    // TEST CASE 3: Admitting patients
    hospital.admitPatient(p1, PRIVATE_ROOM);
    hospital.admitPatient(p2, ICU);
    hospital.admitPatient(p1, SEMI_PRIVATE);

    // TEST CASE 4: Booking appointments
    hospital.bookAppointment(d1, p1);
    hospital.bookAppointment(d1, p2);
    hospital.bookAppointment(d2, p3);
    hospital.bookAppointment(999, p1);
    hospital.bookAppointment(d1, 999);

    // TEST CASE 5: Handling medical tests
    hospital.addEmergency(p3);
    hospital.addEmergency(p1);
    hospital.handleEmergency();
    hospital.handleEmergency();
    hospital.handleEmergency();

    // TEST CASE 6: Information Display & Edge Cases
    hospital.displayPatientInfo(p1);
    hospital.displayPatientInfo(p2);
    hospital.displayPatientInfo(999);

    hospital.displayDoctorInfo(d1);
    hospital.displayDoctorInfo(d2);
    hospital.displayDoctorInfo(999);

    hospital.dischargePatient(999);
    hospital.cancelAppointment(999, p1);
    hospital.handleEmergency();

    // TEST CASE 7: Search, Tests & Prescriptions
    cout << "========== NEW FEATURES ==========" << endl;
    hospital.searchPatientByName("John Doe");

    hospital.requestPatientTest(p1, "Blood Test");
    hospital.requestPatientTest(p1, "X-Ray");
    hospital.requestPatientTest(p1, "MRI");

    cout << "Pending tests:" << endl;
    hospital.displayPatientTests(p1);

    cout << "Performing first test:" << endl;
    hospital.performPatientTest(p1);

    cout << "Remaining tests:" << endl;
    hospital.displayPatientTests(p1);

    hospital.prescribeMedicine(p1, "Paracetamol");
    hospital.prescribeMedicine(p1, "Antibiotic");
    hospital.displayPrescriptions(p1);

    // TEST CASE 8: Billing, Appointment Management & Priority Emergencies
    hospital.displayPatientBill(p1);

    hospital.displayDoctorAppointments(d1);
    hospital.displayDoctorAppointments(d2);

    hospital.doctorSeePatient(d1);
    hospital.displayDoctorAppointments(d1);

    hospital.cancelAppointment(d1, p2);
    hospital.displayDoctorAppointments(d1);

    hospital.addPriorityEmergency(p1, 2);
    hospital.addPriorityEmergency(p2, 5);
    hospital.addPriorityEmergency(p3, 3);
    hospital.addPriorityEmergency(p1, 4);

    cout << "Handling priority emergencies:" << endl;
    hospital.handlePriorityEmergency();
    hospital.handlePriorityEmergency();
    hospital.handlePriorityEmergency();
    hospital.handlePriorityEmergency();

    // TEST CASE 9: Room Status, Listings, Discharge & Final Statistics
    hospital.displayRoomStatus();
    hospital.displayAllPatients();
    hospital.displayAllDoctors();

    hospital.dischargePatient(p1);
    hospital.displayPatientInfo(p1);

    hospital.displayStatistics();

    return 0;
}
