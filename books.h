// Koukos Paschalis : ΑΜ it2024041
// αρχείο βιβλιοθήκης για την διαχείριση βιβλίων του συστήματος που θελουμε να φτιάξουμε
// περιλαμβανω ολες τις κεφαλίδες που χρειάζονται για την διαχείριση των βιβλίων
// και τις συναρτήσεις που θα χρησιμοποιηθούν στη main.c και στο books.c (υλοποίηση)
// καθως και τη δομη για την διαχειριση της διπλα συνδεδ. λιστας και 2 βοηθητικα enums

#ifndef BOOKS_H // αποφυγη λανθασμενης συμπεριληψης
#define BOOKS_H 

// μακροεντολες
#define TRUE 1 
#define FALSE 0 
#define STND_SIZE 256 // μεγεθος για τον τιτλο, συγγραφέα και εκδότη-standard size
#define ISBN_SIZE 20 // μεγεθος για το ISBN


/* δομη διπλας συνδεδεμενης λιστας που θα χρησιμοποιηθει */

typedef struct node_book{ // Δομή για τον κόμβο βιβλίου της διπλα συνδεδεμένης λίστας   
    char title[STND_SIZE]; 
    char author[STND_SIZE];
    char publisher[STND_SIZE]; 
    int publication_year; 
    char isbn[ISBN_SIZE]; 


    struct node_book* next; // δείχνει στον επόμενο κόμβο
    struct node_book* previous; // δείχνει στον προηγούμενο κόμβο
} node_book; 

//χρηση ειδικων κωδικων για ευκολια στην αναγνωση με enumeration

typedef enum { 
    // εδω για την search_isbn θα χρησιμοποιησω κωδικους-enum
    CHANGE,
    DELETE,
} option_for_search_book;

typedef enum {
    // για την ταξινομηση
    TITLE=1, // για να ειναι εγκυρο με την επιλογη χρηστη απο το μενου που θα του θεσω το βαζω =1
    AUTHOR, // =2 κλπ
    PUBLISHER,
    PYEAR,
    ISBN
} option_for_sort;


/*declarations συναρτησεων*/

void change_book_details(node_book*, int); // συναρτηση που βοηθα την υλοποιηση της change_book_entry
node_book* find_book_by_isbn(node_book* ,const char*);//συναρτηση για να βρισκω το βιβλιο με βαση το isbn
node_book* add_book_entry(node_book*, node_book**); // συνάρτηση για να προσθέσει ένα νέο βιβλίο
node_book* delete_book_entry(node_book*,node_book**); // συναρτηση για να διαγράψει ένα βιβλίο
void print_books(const node_book*); // συναρτηση για να εκτυπώσει όλα τα βιβλία
void search_book_entry(node_book*); // συνάρτηση για να αναζητήσει ένα βιβλίο
node_book* add_book_details(void); // συναρτηση για να βαζει τα στοιχεια της νεας εγγραφης ο χρηστης
void change_book_entry(node_book*); // συνάρτηση για να αλλάξει τα στοιχεία ενός βιβλίου
node_book* sort_books(node_book* ,option_for_sort);// συνάρτηση για να ταξινομήσει τα βιβλία
node_book* search_isbn(node_book*,option_for_search_book); // βοηθητικη συναρτηση για να επιστρεφει το βιβλιο αν βρεθηκε 
node_book* loadingfromfile(node_book**,node_book**); // συνάρτηση για να φορτώσει τα δεδομένα από το αρχείο
void save_to_file(node_book*); // συνάρτηση για να αποθηκεύσει τα δεδομένα σε αρχείο
void read_string_remove_newline(char*,int); // συνάρτηση για να διαβάσει μια συμβολοσειρά και να αφαιρέσει το newline
void press_enter_to_continue(void); // συνάρτηση για να περιμένει ο χρήστης να πατήσει enter
node_book* split(node_book*);// συνάρτηση για να χωρίσει τη λίστα σε δύο μέρη
node_book* mergesort(node_book*,option_for_sort);// συνάρτηση για να ταξινομήσει τη λίστα
node_book* merge(node_book*, node_book*, option_for_sort);// συνάρτηση για να συγχωνεύσει δύο λίστες
option_for_sort get_sort_choice(void); // συνάρτηση για να πάρει την επιλογή ταξινόμησης από τον χρήστη
int is_integer(const char*,int*); // συνάρτηση για να ελέγξει αν η είσοδος είναι ακέραιος
int print_first_screen_menu(void); // συνάρτηση για να εκτυπώσει το πρώτο μενού
void check_input_menu(int*,int,int); // συνάρτηση για να ελέγξει την είσοδο του μενού
void free_my_nodes(node_book*); // συνάρτηση για να απελευθερώσει τη μνήμη που δεσμεύτηκε για τη λίστα

#endif 