/*
Το προγραμμα μας προσομοιωνει τη διαχειριση βιβλιων μιας ηλεκτρονικης βιβλιοθηκης 
Η προσομοιωση του βιβλιου γινεται με ενα struct διπλας συνδεδεμενης λιστας και υπαρχουν διαφορες επιλογες που μπορει να βαλει ο χρηστης
και να διαχειριστει τα βιβλια με τον τροπο που επιθυμει
*/


// Koukos Paschalis
// AM: it2024041

#include "books.h" // περιλαμβανει κεφαλιδες κι declarations
#include <stdio.h> 
#include <stdlib.h>

#ifdef _WIN32 //προσαρμοσμενο σε καθε λειτουργικο συστημα για να καθαριζει την οθονη 
    #define CLEAR_SCREEN "cls" //windows
#else
    #define CLEAR_SCREEN "clear" //ubuntu linux 
#endif

int main(void){ // κύρια συνάρτηση του προγράμματος 
    node_book* head = NULL;  
    node_book* tail = NULL; 
    head = loadingfromfile(&head,&tail); // φορτώνουμε τα δεδομένα από το αρχείο

    int flag = TRUE; // σημαια για εξοδο απο το προγραμμα
    while (flag) 
    {
        system(CLEAR_SCREEN); 
        int value = print_first_screen_menu(); // εκτυπώνουμε το μενού
        
        switch (value) 
        {
            case 1: // προσθήκη βιβλίου
                head = add_book_entry(head, &tail); 
                break;
            case 2: // διαγραφή βιβλίου
                head = delete_book_entry(head, &tail); 
                break;
            case 3: // αλλαγή βιβλίου
                change_book_entry(head); 
                break;
            case 4: // ταξινόμηση βιβλίων
                head = sort_books(head, get_sort_choice());
                break;
            case 5: // αναζήτηση βιβλίου
                search_book_entry(head); 
                break;
            case 6: // εκτύπωση βιβλίων
                print_books(head); 
                break;
            case 7: // έξοδος από το πρόγραμμα
                flag = FALSE; 
                break;
        }
        
    }
    save_to_file(head); // αποθηκεύουμε τα δεδομένα σε αρχείο
    free_my_nodes(head); // απελευθερώνουμε τη μνήμη που δεσμεύτηκε για τη λίστα
    head=NULL; // αποφυγη αιωρουμενου δεικτη / ακομα κι τωρα στο τελος του προγραμματος
    tail=NULL;
    puts("\nExiting system...");  
    return EXIT_SUCCESS;
}