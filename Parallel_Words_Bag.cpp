#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>
#include <map>
#include <chrono>
#include <mpi.h>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {

    // Define global variables
    double start_time, execution_time, end_time;
    int num_processes, process_id;
    const int root = 0;
    const int num_books = 6;
    string* filenames;
    string filename;
    char** vocabulary;
    int** bag_words_matrix;
    const int str_len = 256; // length of each string
    int number_of_words;
    int* bag_words_array;
    map<string, int> word_dictionary;

    // Initialize MPI
    MPI_Init(&argc, &argv); // Initialize MPI
    MPI_Comm_size(MPI_COMM_WORLD, &num_processes); // Get the number of processes in the communicator group MPI_COMM_WORLD
    MPI_Comm_rank(MPI_COMM_WORLD, &process_id); // rank means processid

    // Start timer
    if(process_id == 0) start_time = MPI_Wtime();

    // Si no se ingresan los argumentos necesarios, mostrar mensaje de error y terminar el programa
    if(argc < num_books+2) {
        if( process_id == 0){
            cout << "Uso: " << argv[0] << " <archivo1> <archivo2> <archivo3> <archivo4> <archivo5> <archivo6> <archivo_vocabulario>" << endl;
        }
        MPI_Finalize();
        return 1;
    }


    // ROOT PROCESS
    if(process_id == 0){

        //--- FILENAMES---//
        // Leer nombres de archivos desde los argumentos y guardarlos en un arreglo de strings llamado filenames
        filenames = new string[num_books];
        for (int i = 0; i < num_books; i++) {
            filenames[i] = argv[i+1];
        }

        //--- VOCABULARY---//
        //  Crear un arreglo de strings llamado vocabulary de tamaño number_of_words para guardar las palabras del vocabulario
        const int arr_size = number_of_words; // number of strings in array 
        vocabulary = new char*[arr_size]; 
        // initialize each string in array
        string word_aux;
        for (int i = 0; i < arr_size; i++) {
            vocabulary[i] = new char[str_len+1];  // Cada string es de 256 chars incluyendo el caracter nulo
        }
        // Leer el nombre de archivo de vocabulario desde el último argumento y guardarlo en un string llamado vocabulary_file_name
        string vocabulary_file_name = argv[num_books+1];
        // Crear un variable de file stream a partir del nombre de archivo de vocabulario
        ifstream vocabulary_file_input_stream(vocabulary_file_name);
        // Leer la primera palabra del archivo de vocabulario que correpsonde al número de palabras en el vocabulario
        vocabulary_file_input_stream >> number_of_words;
        // Leer las palabras del archivo de vocabulario y guardarlas en el arreglo vocabulary
        for(int i = 0; i < number_of_words; i++){
            vocabulary_file_input_stream >> word_aux;
            strcpy(vocabulary[i], word_aux.c_str());
        }
        vocabulary_file_input_stream.close();

        //--- BAG OF WORDS MATRIX---//
        // Allocate memory for the bag_words_matrix
        bag_words_matrix = new int*[num_books]; // Allocate an array of pointers to int pointers
        for (int i = 0; i < num_books; i++) {
            bag_words_matrix[i] = new int[number_of_words]; // Allocate a row of ints for each pointer in the array
            for (int j = 0; j < number_of_words; j++) {
                bag_words_matrix[i][j] = 0; // Initialize the value of each element in the array to 0
            }
        }
        
        //--- SEND DATA TO OTHER PROCESSES---//
        // Send each filename to the corresponding processes
        filename = filenames[0]; // filename points to the first element of filenames para que no se bloqueen los procesos
        for(int i = 1; i < num_books; i++){
            MPI_Send(filenames[i].c_str(), str_len, MPI_CHAR, i, 0, MPI_COMM_WORLD);  //MPI_Send expects a C-style string.
        }
        // Send the corresponding bag_words_array from bag_words_matrix to all processes
        bag_words_array = bag_words_matrix[0]; // bag_words_array points to the first row of bag_words_matrix para que no se bloqueen los procesos
        for(int i = 1; i < num_books; i++){
            MPI_Send(bag_words_matrix[i], number_of_words, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
    }
     
    //-----------------ALL PROCESSES (except root)----------------------
    if(process_id != root){
        // Recibir de root: number_of_words, vocabulary, filename, su respectivo array de bag_words_matrix
        MPI_Recv(&filename, 1, MPI_CHAR, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(bag_words_array, number_of_words, MPI_INT, root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }

    //-----------------ALL PROCESSES (including root)----------------------
    // Mandar/Planchar a los todos los procesos: number_of_words
    MPI_Bcast( &number_of_words, 1, MPI_INT, root, MPI_COMM_WORLD); // Es bloqueante

    // Send all words in vocabulary to all processes
    const int arr_size = number_of_words; // number of strings in array
    for (int i = 0; i < number_of_words; ++i) {
        MPI_Bcast(vocabulary[i], arr_size, MPI_CHAR, root, MPI_COMM_WORLD);
    }

    // Cada proceso cuenta las palabras correspondiente en archivos de texto y guardar en word_dictionaries
    string word, line;
    ifstream book_file("./Books/" + filename); // Open the CSV file correspondiente al índice del proceso
    while (std::getline(book_file, line)) { 
        std::vector<std::string> row; // Vector to hold the values in the current row
        std::string value;
        for (char c : line) {
            if (c == ',') { // End of current value
                word_dictionary[value]++;
                value.clear(); // Clear the value string for the next value
            } else {
                value.push_back(c); // Add the current character to the current value
            }
        }
        word_dictionary[word]++;
        value.clear(); // Clear the value string for the next value
    }
    book_file.close(); // Close the CSV file

    // Cada proceso suma los valores de word_dictionaries en su respectivo bag_words_array
    for(int i = 0; i < number_of_words; i++){
        bag_words_array[i] += word_dictionary[vocabulary[i]]; // Si el valor no existe en el map, se crea con valor 0, entonces suma cero
    }

    // Enviar bag_words_array a root
    if (process_id != root)
    {
        MPI_Send(bag_words_array, number_of_words, MPI_INT, root, 0, MPI_COMM_WORLD);
    }else{
        bag_words_matrix[0] = bag_words_array;
    }
    
    // ROOT AGAIN
    if(process_id == 0){
        // Receive each bag_words_array of each process and asign it to each position of bag_words_matrix
        MPI_Status status;
        for(int i = 1; i < num_books; i++){
            MPI_Recv(bag_words_matrix[i], number_of_words, MPI_INT, i, 0, MPI_COMM_WORLD, &status);
        }

        // Escribir matriz en archivo csv
        ofstream output("parallel_output.csv");
        // Escribir encabezado
        output << "Palabras,";
        for (int i = 0; i < num_books; i++) {
            output << filenames[i] << ",";
        }
        output << endl;
        // Escribir contenido
        for (int j = 0; j < number_of_words; j++) {
            output << vocabulary[j] << ",";
            for (int i = 0; i < num_books; i++) {
                output << bag_words_matrix[i][j] << ",";
            }
            output << endl;
        }
        output.close();       
    }
    
    
    // Medir tiempo de ejecución
    if(process_id = 0){
        end_time = MPI_Wtime();
        execution_time = end_time - start_time;
        printf("Execution time: %f seconds\n", execution_time);
    } 

    //Liberar memoria
    delete[] vocabulary;
    delete[] filenames;
    for (int i = 0; i < num_books; i++) {
        delete[] bag_words_matrix[i]; // Deallocate each row
    }
    delete[] bag_words_matrix; // Deallocate the array of pointers
    
    // Finalize the MPI environment.
    MPI_Finalize();

    // Terminar programa
    return 0;
}
