# DJJ project

Η εργασία παραδίνεται από τους:
Παναγιώτης Κεχρινιώτης(sdi2000077), Αθηνά Χειλάκου(sdi2000213)

202000213 (sdi2000213@di.uoa.gr):
υλοποίηση heap, min heap, brute force, συνεισφορά στη main

202000077 (sdi2000077@di.uoa.gr)
υλοποίηση graph, data, testing, συνεισφορά στη main

Γενικά και τα δύο μέλη της ομάδας συνεισέφεραν σε όλα τα μέρη της εργασίας, δεν υπήρχε αυστηρός διαχωρισμός των τμημάτων της εργασίας.

K nearest neighbors descent implementation

Tρέχει με τα δεδομένα του διαγωνισμού στον φάκελο datasets/given και με
δεδομενα float τριών διαστάσεων

Το πρόγραμμα δέχεται ορίσματα γραμμής εντολών ως εξής:
./main <max_neighbors/k> <filename> <manh/eucl> <data_type_flag> <delta> <sampling_rate>
<max_neighbors>: πλήθος κοντινότερων γειτόνων που ψάχνουμε
<filename>: αρχείο δεδομένων
<manh/eucl>: μετρική απόστασης για τα δεδομένα, ένα εκ των manh (manhattan) ή eucl (euclidian)
<data_type_flag>: 0 για τα δεδομένα του διαγωνισμού και 1 για float τριών διαστάσεων
<delta>: παράμετρος για τον προόρο τερματισμό του αλγορίθμου, 0 < delta <= 1
<sampling_rate>: παράμετρος για την επιλογή ποσοστού των στοιχείων που θα ελεγθούν σε κάθε επανάληψη 0 < sampling_rate <= 1

TODO

add flags to avl, heap!!
fix the damn code
replace all_weights, all_flags
remove graph n\*n

avl to array flag fix

add flag size (true & false) to avl to reduce mapp to array space
