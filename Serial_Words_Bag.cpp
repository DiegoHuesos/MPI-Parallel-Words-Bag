#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <chrono>

using namespace std;
using namespace std::chrono;

int main(int argc, char* argv[]) {
    // Número de palabras en el vocabulario
    const int num_books = 6;

    // Si no se ingresan los argumentos necesarios, mostrar mensaje de error
    if(argc < num_books+2) {
        cout << "Uso: " << argv[0] << " <archivo1> <archivo2> <archivo3> <archivo4> <archivo5> <archivo6> <archivo_vocabulario>" << endl;
        return 1;
    }

    // Leer nombres de archivos desde los argumentos
    string filenames[num_books];
    for (int i = 0; i < num_books; i++) {
        filenames[i] = argv[i+1];
    }
    // Leer nombre de archivo de vocabulario desde argumento
    string vocabulary_file_name = argv[num_books+1];

    // Leer palabras del archivo de vocabulario
    ifstream vocabulary_file_input_stream(vocabulary_file_name);
    // Leer número de palabras
    int number_of_words;
    vocabulary_file_input_stream >> number_of_words;
    // Crear arreglo de palabras
    map<string, int>* word_dictionaries = new map<string, int>[num_books];
    // Leer palabras del archivo de vocabulario y guardarlas en el los diccionarios de palabras
    string* vocabulary = new string[number_of_words]; // Crear arreglo de palabras
    string word_aux;
    for (int i = 0; i < number_of_words; i++) {
        vocabulary_file_input_stream >> word_aux;
        vocabulary[i] = word_aux;
        word_dictionaries[0].insert({word_aux, 0});
        word_dictionaries[1].insert({word_aux, 0});
        word_dictionaries[2].insert({word_aux, 0});
        word_dictionaries[3].insert({word_aux, 0});
        word_dictionaries[4].insert({word_aux, 0});
        word_dictionaries[5].insert({word_aux, 0});
    }
    vocabulary_file_input_stream.close();
    
    // Allocate memory for the bag_words_matrix
    int** bag_words_matrix = new int*[num_books]; // Allocate an array of pointers to int pointers
    for (int i = 0; i < num_books; i++) {
        bag_words_matrix[i] = new int[number_of_words]; // Allocate a row of ints for each pointer in the array
    }
    // Initialize the bag_words_matrix
    for (int i = 0; i < num_books; i++) {
        for (int j = 0; j < number_of_words; j++) {
        bag_words_matrix[i][j] = 0;
        }
    }

    // Medir tiempo de ejecución
    auto start_time = high_resolution_clock::now();

    // Contar palabras en archivos de texto
    string word;
    // Iterar sobre cada libro / archivo de texto para contar las palabras
    for (int i = 0; i < 6; i++) {
        string line; 
        ifstream book_file("./Books/" + filenames[i]);
        while (std::getline(book_file, line)) { // Read each line of the file
            std::vector<std::string> row; // Vector to hold the values in the current row
            std::string value;

            // Split the line into values using the comma delimiter
            for (char c : line) {
                if (c == ',') { // End of current value
                    word_dictionaries[i][value]++;
                    value.clear(); // Clear the value string for the next value
                } else {
                    value.push_back(c); // Add the current character to the current value
                }
            }
            word_dictionaries[i][word]++;
            value.clear(); // Clear the value string for the next value
        }
        book_file.close(); // Close the CSV file
    }

    


    // Guardar palabras en matriz
    for( int i = 0; i < 6; i++){
        for (  int j = 0; j < number_of_words; j++){
            bag_words_matrix[i][j] = word_dictionaries[i][vocabulary[j]];
        }
    }

    // Escribir matriz en archivo csv
    ofstream output("serial_output.csv");
    output << "Palabras,";
    for (int i = 0; i < 6; i++) {
        output << filenames[i] << ",";
    }
    output << endl;

    for (int j = 0; j < number_of_words; j++) {
        output << vocabulary[j] << ",";
        for (int i = 0; i < num_books; i++) {
            output << bag_words_matrix[i][j] << ",";
        }
        output << endl;
    }
    output.close();

    // Medir tiempo de ejecución
    auto end_time = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end_time - start_time);
    cout << "Tiempo de ejecucion: " << duration.count() << " ms" << endl;

    //Liberaro memoria
    delete[] vocabulary;

    for (int i = 0; i < num_books; i++) {
        delete[] bag_words_matrix[i]; // Deallocate each row
    }
    delete[] bag_words_matrix; // Deallocate the array of pointers

    delete[] word_dictionaries;


    // Terminar programa
    return 0;
}
