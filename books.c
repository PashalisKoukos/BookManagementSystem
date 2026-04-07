#include "books.h"
#include <stdio.h> 
#include <stdlib.h>
#include <string.h>
#define BUFF_INT 32 // μεγεθος για το buffer του ακέραιου στον ελεγχο εγκυροτητας
#define DATA_SIZE_FROM_NODE_BOOK  sizeof(node_book) - (2 * sizeof(node_book*)) // μεγεθος_ολοκληρου_κομβου-2*(μεγεθος ενος δεικτη)
// χρησιμοποειται για το δυαδικο αρχειο , διοτι σε αυτο αποθηκευονται μονο τα δεδομενα,οχι οι ποιντερς
// Koukos Paschalis


// ΑΚΟΛΟΥΘΕΙ υλοποιηση συναρτησεων

int print_first_screen_menu(void){ // συνάρτηση για να εκτυπώσει το πρώτο μενού
    puts("| --------------------------- |");
    puts("| Book Management System Menu |"); // μενου επιλογών
    puts("| --------------------------- |");
    puts("\n1. Add a new book entry"); 
    puts("2. Delete a book entry"); 
    puts("3. Change a book entry"); 
    puts("4. Sort books"); 
    puts("5. Search for a book entry"); 
    puts("6. Print all books");
    puts("7. Exit");
    printf("-> Please select an option(1-7): ");
    int value; // μεταβλητή για να αποθηκεύσουμε την επιλογή του χρήστη
    check_input_menu(&value,1,7); // ελέγχουμε την είσοδο του χρήστη
    return value;
}

void check_input_menu(int* value,int min,int max){ // συναρτηση για να ελεγχει το input στα διαφορα μενου που διαθετει η εφαρμογη
    char choice[BUFF_INT]; // buffer για την επιλογή του χρήστη
    read_string_remove_newline(choice, BUFF_INT); // διαβάζουμε την είσοδο του χρήστη
    do
    {
        if (!is_integer(choice, value)) { // ελέγχουμε αν η είσοδος είναι έγκυρη
            printf("Invalid menu-input. Please enter an integer between %d and %d: ", min, max); // αν δεν είναι έγκυρη ζητάμε ξανά
            read_string_remove_newline(choice, BUFF_INT); 
        }else if(*value<min || *value>max){ // ελέγχουμε αν η είσοδος είναι έγκυρη
            printf("Invalid menu-input. Please choose between %d and %d: ", min, max); // αν δεν είναι έγκυρη ζητάμε ξανά
            read_string_remove_newline(choice, BUFF_INT);
        } else {
            break; 
        }
    } while (TRUE); // ελεγχος για το αν η εισοδος ειναι εγκυρη
    return; 
}

int is_integer(const char *input, int *value) { // συνάρτηση για να ελέγξει αν η είσοδος είναι ακέραιος
    char *end;
    if (input[0] == '\0') return FALSE; // κενή είσοδος
    *value = strtol(input, &end, 10); // μετατροπή της είσοδου σε ακέραιο
    if (input == end || *end != '\0') return FALSE; // ελεγχω την εισοδο αν είναι έγκυρη με τον εξης τροπο : 
    // αν η είσοδος είναι κενή ή αν δεν έχει μετατραπεί πλήρως σε ακέραιο
    return TRUE;
}

void read_string_remove_newline(char* string,int size){ // συνάρτηση για να διαβάσει μια συμβολοσειρά και να αφαιρέσει το newline
    fgets(string,size,stdin);
    do
    {
        if (string[0] == '\n') { // αν η είσοδος είναι κενή , δηλαδη εχει παρει μονο το \n
            printf("Empty input not allowed. Please enter a string of characters: "); // ζητάμε ξανά
            fgets(string,size,stdin); // διαβάζουμε την είσοδο του χρήστη
        } else {
            break; // αν η είσοδος είναι έγκυρη βγαίνουμε από το loop
        }

    } while (TRUE);  // συνεχιζω μεχρι να βρω αποδεκτη εισοδο
    string[strcspn(string, "\n")] = '\0'; // αφαιρούμε το \n από την είσοδο απο το fgets
    return;
}

void free_my_nodes(node_book* head){ // συναρτηση για να ελευθερωνει την δεσμευμενη μνημη στο τελος του προγραμματος
    node_book* temphead = head;
    node_book* temp=NULL;
    while (temphead!=NULL)
    {
        temp=temphead;
        temphead=temphead->next;
        free(temp);
    }
    return; 
}
    
void press_enter_to_continue(void){ 
    // συναρτηση που χρησιμοποιω ετσι ωστε να μην σβηνεται απευθειας το 
    //αποτελεσμα με την system(CLEAR_SCREEN). Με αυτο το τροπο ο χρηστης μπορει να δει το αποτελεσμα 
    // κι να πατησει εντερ για να συνεχισει μολις επιθυμησει
    printf("Press enter to go back to the book management menu...");
    while (getchar() != '\n'); // περιμένουμε να πατήσει enter
    return;
    
}

option_for_sort get_sort_choice(void) { // συνάρτηση για να πάρει την επιλογή ταξινόμησης από τον χρήστη
    // εμφανιζω μενου ταξινομησης 
    int value;
    printf("\n---Select sorting option:\n");
    printf("1. Sort by title\n");
    printf("2. Sort by author\n");
    printf("3. Sort by publisher\n");
    printf("4. Sort by publication year\n");
    printf("5. Sort by ISBN\n");
    printf("Enter your choice (1-5): ");
    check_input_menu(&value,1,5); // ελέγχουμε την είσοδο του χρήστη με αποδεκτο πεδιο τιμων [1,5]
    return (option_for_sort)value; // επιστρέφουμε την επιλογή ταξινόμησης με cast
}

node_book* sort_books(node_book* head, option_for_sort choice) { // συναρτηση για την ταξινομηση-κυριως βοηθητικη
    if (head== NULL || head->next== NULL) { // αν η λίστα είναι κενή ή έχει μόνο ένα βιβλίο
        puts("\n\nSorting option not available: The list is either empty or contains only 1 book.");
        press_enter_to_continue();
        return head;
    }

    head = mergesort(head, choice); // ξεκιναμε την ταξινομηση
    puts("\n\nBooks sorted successfully.");
    press_enter_to_continue();
    return head;
}

node_book* split(node_book *head) { // συναρτηση που χωριζει τη λιστα σε δυο κομματια για να επιτευχθει η μεθοδος ταξινομησης με συγχωνευση
    // source: geeksforgeeks

    node_book *fast = head;
    node_book *slow = head;

    // Move fast pointer two steps and slow pointer
    // one step until fast reaches the end
    while (fast != NULL && fast->next != NULL 
           && fast->next->next != NULL) {
        fast = fast->next->next;
        slow = slow->next;
    }

    // Split the list into two halves
    node_book *temp = slow->next;
    slow->next = NULL;
    if (temp != NULL) {
        temp->previous = NULL;
    }
    return temp;
}

node_book* merge(node_book* first, node_book* second, option_for_sort choice) { // συναρτηση για ταξινομηση των βιβλιων με βαση κριτηριο
    if (!first) return second; // αν η πρώτη λίστα είναι κενή επιστρέφουμε τη δεύτερη
    if (!second) return first; // αν η δεύτερη λίστα είναι κενή επιστρέφουμε την πρώτη

    int ch=0;
    switch (choice) { // επιλογή ταξινόμησης
        case TITLE: // τιτλος
            ch = strcmp(first->title, second->title);
            break;
        case AUTHOR: //συγγραφεας
            ch= strcmp(first->author, second->author);
            break;
        case PUBLISHER: //εκδοτης
            ch= strcmp(first->publisher, second->publisher);
            break;
        case PYEAR: //χρονια εκδοσης
            ch= first->publication_year - second->publication_year;  
            break;
        case ISBN: // isbn
            ch= strcmp(first->isbn, second->isbn);
            break;
    }

    // εδω εχω προσαρμοσει τον κωδικα στις δικες μας απαιτησεις 
    // αλλαζω τη σειρα των κομβων διχως να πειραξω τους ποιντερ
    if (ch<=0) { // αν ο πρώτος κόμβος είναι μικρότερος ή ίσος με τον δεύτερο
        first->next = merge(first->next, second, choice); // συγχωνεύουμε τις επόμενες λίστες
        if (first->next) first->next->previous = first; // ενημερώνουμε το previous pointer του επόμενου κόμβου
        first->previous = NULL; // ορίζουμε το previous pointer του πρώτου κόμβου σε NULL
        return first;
    } else { // αν ο δεύτερος κόμβος είναι μικρότερος από τον πρώτο
        second->next = merge(first, second->next, choice); 
        if (second->next) second->next->previous = second; 
        second->previous = NULL; 
        return second;
    }
}

node_book* loadingfromfile(node_book **head, node_book **tail) { // συναρτηση για να φορτωνει τα βιβλια απο το books.dat
    FILE* file = fopen("books.dat", "rb"); // άνοιγμα αρχείου
    if (file == NULL) {
        perror("Error opening file with name: books.dat"); 
        return NULL;
    }

    node_book temp; // προσωρινη μεταβλητη για την αναγνωση
    while (fread(&temp, DATA_SIZE_FROM_NODE_BOOK, 1, file) == 1) { //διαβαζω απο το αρχειο 1 κομβο τη φορα(δηλαδη ενα βιβλιο τη φορα). Αν !=1 φτασαμε στο τελος
        node_book* new_entrance_node_book = (node_book*)malloc(sizeof(node_book)); // δεσμευω χωρο για το νεο βιβλιο-κομβο
        if (new_entrance_node_book == NULL) {
            perror("Loading data failed.\n");
            fclose(file);
            free_my_nodes(*head); // σε περιπτωση ενδοιαμεσου σφαλματος ελευθερωνω οτι δεσμευσα
            *head = NULL; // αποφυγη αιωρουμενου δεικτη
            *tail = NULL;
            return NULL;
        }

        // Αντιγραφή δεδομένων από προσωρινή μεταβλητή στον νέο κόμβο
        *new_entrance_node_book = temp;

        // ακολουθειται η λογικη της add book entry
        new_entrance_node_book->next = NULL;
        new_entrance_node_book->previous = NULL;

        if ((*head) == NULL) {
            (*head) = new_entrance_node_book;
            (*tail) = new_entrance_node_book;
        } else {
            (*tail)->next = new_entrance_node_book;
            new_entrance_node_book->previous = (*tail);
            (*tail) = new_entrance_node_book;
        }
       
    }
    if(ferror(file)){ //αν συνεβη οποιοδηποτε σφαλμα
        perror("Unexpected error while reading the file");
        fclose(file);
        free_my_nodes(*head);
        *head = NULL;
        *tail = NULL;
        return NULL;
    }
    fclose(file);
    return *head;
}

void save_to_file(node_book* head){ // συναρτηση για να αποθηκευω τα δεδομενα στο αρχειο μας 
    
    FILE* file = NULL;
    file = fopen("books.dat", "wb"); // ανοίγουμε το αρχείο για προσθήκη
    if(file == NULL) { // αν δεν μπορέσαμε να ανοίξουμε το αρχείο
        perror("Error opening file with name: books.dat\n");
        return; 
    }

    node_book* current_book=head; // αρχικοποιούμε το current_book να δείχνει στο head της λίστας
    int i=0;
    while (current_book!=NULL && fwrite(current_book,DATA_SIZE_FROM_NODE_BOOK,1,file)==1){i++;current_book=current_book->next;} // αποθηκευση βιβλιων
    // ουσιαστικα η Fwrite γραφει 1 κομβο τη φορα μεγεθους data_size_from_node_book
    if (ferror(file))
    {
        perror("Error during saving the data into the file");
        printf("\nNumber of books saved before the error:%d",i);
    }else{
        printf("\nBooks data saved to books.dat");
    }
    fclose(file);
    return;
}

node_book* add_book_details(void){ // συναρτηση βοηθητικη για την add_book_entry

    node_book* new_entrance_node_book = (node_book*)malloc(sizeof(node_book)); // δεσμεύουμε μνήμη για το νέο κόμβο
    if (new_entrance_node_book == NULL) { // ελεγχουμε αν η δέσμευση μνήμης ήταν επιτυχής
        perror("Not able to add a new book"); // αν δεν ήταν επιτυχής εκτυπώνουμε μήνυμα
        press_enter_to_continue(); 
        return NULL;
    }

    puts("\n---Enter book details:"); // εισοδος στοιχείων βιβλίου
    printf("Title: ");
    read_string_remove_newline(new_entrance_node_book->title,STND_SIZE);

    printf("Author: ");
    read_string_remove_newline(new_entrance_node_book->author,STND_SIZE);

    printf("Publisher: ");
    read_string_remove_newline(new_entrance_node_book->publisher,STND_SIZE);

    printf("Publication Year: "); // ελεγχω για σωστη εισοδο ακεραιου επαναληπτικα
    char p_y[BUFF_INT];
    do
    {
        fgets(p_y,BUFF_INT,stdin); // διαβάζουμε την είσοδο του χρήστη
        p_y[strcspn(p_y, "\n")] = '\0'; // αφαιρούμε το \n από την είσοδο
        if (is_integer(p_y,&new_entrance_node_book->publication_year) && new_entrance_node_book->publication_year>=0) // ελέγχουμε αν η είσοδος είναι έγκυρη
            break; // αν είναι έγκυρη βγαίνουμε από το loop
        printf("Invalid publication year. Please enter a valid publication year: "); // αν δεν είναι έγκυρη ζητάμε ξανά
    } while (TRUE); // ελεγχος για το αν η εισοδος ειναι εγκυρη
    
    printf("ISBN: ");
    read_string_remove_newline(new_entrance_node_book->isbn,ISBN_SIZE); 
    return new_entrance_node_book; // επιστροφη καταχωρησης βιβλιου-εγγραφης
}

node_book* add_book_entry(node_book* head, node_book** tail) { // συνάρτηση για να προσθέσει ένα νέο βιβλίο στη λίστα

    
    node_book* new_entrance_node_book = add_book_details(); // παιρνω το βιβλιο που δεσμευτηλε χωρος
    if (new_entrance_node_book==NULL)return head; // αν δεν πετυχε η δεσμευση
    
    // ελεγχος αν το βιβλιο υπάρχει ήδη στη λιστα
    
    if (find_book_by_isbn(head,new_entrance_node_book->isbn)!=NULL)
    {
        printf("Book with ISBN: %s already exists.\n", new_entrance_node_book->isbn);
        free(new_entrance_node_book); // απελευθερώνουμε τη μνήμη του νέου κόμβου
        press_enter_to_continue();
        return head;
    }
    
    // αν το βιβλίο δεν υπάρχει ήδη στη λίστα, προχωράμε στην προσθήκη του 

    new_entrance_node_book->next = NULL; // αρχικοποιούμε το next pointer του νέου κόμβου σε NULL
    new_entrance_node_book->previous = NULL; // αρχικοποιούμε το previous pointer του νέου κόμβου σε NULL

    if(head==NULL && (*tail)==NULL){ // αν η λίστα είναι κενή
        head=new_entrance_node_book; // ορίζουμε το head να δείχνει στον νέο κόμβο
        (*tail)=new_entrance_node_book; // ορίζουμε το tail να δείχνει στον νέο κόμβο
    }else{ // δεν ειναι κενη
        (*tail)->next=new_entrance_node_book; // θέτουμε το next pointer του τρέχοντος tail να δείχνει στον νέο κόμβο
        new_entrance_node_book->previous=(*tail); // θέτουμε το previous pointer του νέου κόμβου να δείχνει στον τρέχοντα tail
        (*tail)=new_entrance_node_book; //  ενημερώνουμε το tail να δείχνει στον νέο κόμβο
    }
    puts("\nBook added successfully."); 
    press_enter_to_continue();
    return head;

}

node_book* search_isbn(node_book* head,option_for_search_book choice){ // συναρτηση για να βρει το βιβλιο αν υπαρχει 
    if (choice==DELETE) // για διαγραφη εμφανιση μηνυματος
    {
        puts("\n---Delete a book entry below:");
        printf("Enter the ISBN of the book to delete: ");
    }else{ // για αλλαγη στοιχειων
        puts("\n---Change a book entry below:");
        printf("Enter the ISBN of the book you want to change: ");
    }
    
    char isbn[ISBN_SIZE];
    read_string_remove_newline(isbn,ISBN_SIZE); // διαβαζω το isbn του χρηστη

    return find_book_by_isbn(head,isbn); // επιστρεφω το βιβλιο αν βρεθηκε χρησιμοποιωντας την findbook
}

node_book* find_book_by_isbn(node_book* head,const char* isbn){ // συναρτηση για να βρισκω ενα βιβλιο με βαση το isbn
    node_book* temp = head;
    node_book* found = NULL;
    while (temp!=NULL)
    {
        if (strcmp(temp->isbn,isbn)==0) //βρεθηκε το βιβλιοc
        {
            found=temp; // το αποθηκευω
            break;
        }
        temp=temp->next;
    }
    return found; // επιστροφη αποτελεσματος/ NULL αν δε βρεθηκε το βιβλιο
}

node_book* delete_book_entry(node_book* head, node_book** tail) { // συνάρτηση για να διαγράψει ένα βιβλίο από τη λίστα
    if (head == NULL) {
        puts("The list of books is empty.Delete option not available."); // αν η λίστα είναι κενή
        press_enter_to_continue();
        return head;
    }

    node_book* found = search_isbn(head,DELETE); // ψαχνω το βιβλιο με βαση το isbn 
    if (found==NULL)
    {
        // αν δεν βρεθεί το βιβλίο με το συγκεκριμένο ISBN
        puts("Book with this ISBN not found.");
        press_enter_to_continue();
        return head;
    }

    //βρεθηκε-> προχωρω σε διαγραφη
    if (found->previous==NULL) // ειναι ο πρωτος κομβος 
    {
        if (found->next==NULL) // αν ειναι μοναδικος κομβος -> αδειαζω λιστα
        {
            head=NULL;
            *tail=NULL; // λιστα αδεια
            free(found);
        }else{ // αν ειναι απλα ο πρωτος κομβος οριζω το head τον επομενο κομβο κι τον οριζω ως πρωτο κομβο
            head=found->next;
            head->previous=NULL;
            free(found);
        }
        
    } else if(found->next==NULL){ // τελευταιος κομβος 
        (*tail) = found->previous; // ενημερώνουμε το tail να δείχνει στον προηγούμενο κόμβο
        (*tail)->next = NULL; // ενημερώνουμε το next pointer του νέου tail σε NULL
        free(found); 
    } else{ //ενδιαμεσος κομβος 
        found->previous->next=found->next; // το προηγουμενο να δειχνει το επομενο
        found->next->previous=found->previous; // το επομενο να δειχνει στο προηγουμενο παρακαμπτωντας τον κομβο που βρεθηκε για διαγραφη
        free(found);
    }
    puts("Book deleted successfully!");
    press_enter_to_continue();
    return head;

}

void print_books(const node_book* head){ // συναρτηση για να τυπωνει τη λιστα βιβλιων
    if (head==NULL) { // αν η λίστα είναι κενή
        printf("The list of books is empty.Printing choice not available\n"); // εκτύπωση μηνύματος
        press_enter_to_continue(); // περιμένουμε να πατήσει enter ο χρήστης
        return; 
    }

    printf("\n---LIST OF BOOKS:\n");
    unsigned int i=0;
    const node_book* temp = head; // αρχικοποιούμε το temp να δείχνει στο head της λίστας/const γιατι δε θα κανω αλλαγες στη λιστα.ΜΟΝΟ εκτυπωση
    while (temp!=NULL) { // όσο η λίστα δεν είναι κενή
        printf("\n===Book number:%hu\n", ++i); // εκτύπωση αριθμού βιβλίου
        printf("Title: %s\n", temp->title); // εκτύπωση τίτλου βιβλίου
        printf("Author(s): %s\n", temp->author); // εκτύπωση συγγραφέα βιβλίου
        printf("Publisher: %s\n", temp->publisher); // εκτύπωση εκδότη βιβλίου
        printf("Publication Year: %d\n", temp->publication_year); // εκτύπωση έτους έκδοσης βιβλίου
        printf("ISBN: %s\n", temp->isbn); // εκτύπωση ISBN βιβλίου
        temp = temp->next; // προχωράμε στον επόμενο κόμβο
    }
    press_enter_to_continue();
    return; 
}

void search_book_entry(node_book* head){ // συνάρτηση για να αναζητήσει ένα βιβλίο στη λίστα
    if (head==NULL) // αν ειναι αδεια
    {
        puts("The list of books is empty.Searching option not available"); 
        press_enter_to_continue();
        return;
    }

    puts("\n---Search for a book entry"); 
    printf("Enter the ISBN of the book to search: ");
    char isbn[ISBN_SIZE]; 
    read_string_remove_newline(isbn,ISBN_SIZE);

    node_book* found_book = find_book_by_isbn(head,isbn); //βρισκω το βιβλιο με βαση το isbn
    if (found_book!=NULL) //αν το βιβλιο βρεθηκε
    {
        printf("===Book Details:\n");
        printf("Title: %s\n", found_book->title); // εκτύπωση τίτλου βιβλίου
        printf("Author(s): %s\n", found_book->author); // εκτύπωση συγγραφέα βιβλίου
        printf("Publisher: %s\n", found_book->publisher); // εκτύπωση εκδότη βιβλίου
        printf("Publication Year: %d\n", found_book->publication_year); // εκτύπωση έτους έκδοσης βιβλίου
        printf("ISBN: %s\n", found_book->isbn); // εκτύπωση ISBN βιβλίου
    }else{
        printf("Book with ISBN: %s not found.\n", isbn); // αν δεν βρεθεί το βιβλίο με το συγκεκριμένο ISBN
    }
    
    press_enter_to_continue();
    return; 
}

void change_book_details(node_book* found_book, int value){ // συναρτηση για την αλλαγη στοιχειων μιας εισοδου - βιβλιου
       
    switch (value) { // επιλογή χρήστη

        case 1:// αλλαγη τιτλου βιβλιου
            printf("Previous book title:'%s'.\nEnter the new book title: ",found_book->title);
            read_string_remove_newline(found_book->title, STND_SIZE);
            printf("Book title successfully changed to: %s\n", found_book->title);
            break;

        case 2:// αλλαγη συγγραφεα βιβλιου
            printf("Previous book author:'%s'.\nEnter the new book author: ",found_book->author);
            read_string_remove_newline(found_book->author, STND_SIZE);
            printf("Book author successfully changed to: %s\n", found_book->author);
            break;

        case 3:// αλλαγη εκδοτη βιβλιου
            printf("Previous publisher is:'%s'.\nEnter the new book publisher: ",found_book->publisher);
            read_string_remove_newline(found_book->publisher, STND_SIZE);
            printf("Book publisher successfully changed to: %s\n", found_book->publisher);
            break;

        case 4:// αλλαγη ετους εκδοσης βιβλιου με ελεγχο
            printf("Previous publication year is:'%d'.\nEnter the new book publication year: ",found_book->publication_year);
            char p_y[BUFF_INT];
            do
            {
                fgets(p_y, BUFF_INT, stdin); // διαβάζουμε την είσοδο του χρήστη
                p_y[strcspn(p_y, "\n")] = '\0'; // αφαιρούμε το \n από την είσοδο
                if (is_integer(p_y, &found_book->publication_year) && found_book->publication_year >= 0) // ελέγχουμε αν η είσοδος είναι έγκυρη
                    break; // αν είναι έγκυρη βγαίνουμε από το loop
                printf("Invalid publication year. Please enter a valid publication year: "); // αν δεν είναι έγκυρη ζητάμε ξανά
            } while (TRUE); // ελεγχος για το αν η εισοδος ειναι εγκυρη
            printf("Book publication year successfully changed to: %d\n", found_book->publication_year);
            break;

        case 5:// αλλαγη ISBN βιβλιου
            printf("Previous ISBN:'%s' . Enter the new book ISBN: ",found_book->isbn);
            read_string_remove_newline(found_book->isbn, ISBN_SIZE);
            printf("Book ISBN successfully changed to: %s\n", found_book->isbn);
            break;

        case 6:// ακυρωση
            puts("Change option cancelled."); 
            press_enter_to_continue();
            return;
    }
}

void change_book_entry(node_book* head) { // συνάρτηση για να αλλάξει τα στοιχεία ενός βιβλίου
    if (head == NULL) {
        puts("The list of books is empty.Changing option not available"); // αν η λίστα είναι κενή
        press_enter_to_continue(); 
        return;
    }

    node_book* found_book=search_isbn(head,CHANGE); // βρισκω το βιβλιο με βαση το isbn αν υπαρχει με κωδικο CHANGE για 
    //εμφανιση καταλληλου μηνυματος
    if (found_book != NULL) { // αν βρέθηκε το βιβλίο
        printf("\nChange book with title: '%s'.\n---Details menu\n\n", found_book->title);
        printf("1. Change book title\n2. Change book author\n3. Change book publisher\n4. Change book publication year\n5. Change book ISBN\n6. Cancel\n");
        printf("Enter your choice (1-6): ");

        int value;
        check_input_menu(&value,1,6); // ελέγχουμε την είσοδο του χρήστη
        change_book_details(found_book,value); // αλλαζω τα στοιχεια με βαση το value 
    } else {
        puts("Book with this ISBN not found"); // αν δεν βρεθεί το βιβλίο με το συγκεκριμένο ISBN
    }
    press_enter_to_continue();
    return;
}

node_book* mergesort(node_book* head,option_for_sort choice) {
    // συνάρτηση για να ταξινομήσει τη λίστα βιβλίων με τη μέθοδο merge sort
    // καθως διαβασα πως ειναι η καταλληλοτερη για να ταξινομησει μια διπλα συνδεδεμενη λιστα (για μεγαλες λιστες κυριως βεβαια)
    // συνδεσμος που χρησιμοποιηθηκε:https://www.geeksforgeeks.org/merge-sort-for-doubly-linked-list/ για να κατανοησω την merge sort
    // και να την προσαρμόσω για την δική μου περίπτωση
    // την πηρα και την προσαρμοσα στις απαιτησεις της δικης μου λιστας
    if (head==NULL || head->next==NULL)return head; // αν η λίστα είναι κενή ή έχει μόνο ένα βιβλίο επιστρέφουμε το head

    node_book* second = split(head); // χωρίζουμε τη λίστα σε δύο μέρη
    // καλούμε αναδρομικά τη mergeSort για να ταξινομήσουμε τα δύο μέρη
    head = mergesort(head, choice); // ταξινόμηση του πρώτου μέρους
    second = mergesort(second, choice); // ταξινόμηση του δεύτερου μέρους
    return merge(head, second, choice);// συγχώνευση των δύο ταξινομημένων μερών
}

