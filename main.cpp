#include <iostream>
#include <vector>
#include <array>
#include <chrono>
#include <random>

// Feedback:
// Konstanten in CAPSLOCK
// 32 Bit Integer (unsigned) verwenden bei Integer Deklarationen
// namespace std nutzen

// Zukünftig:
// mehrere String/Int Spalten (z.B. 10), von eine Spalte mit nur demselben Wert bis alle Werte sind distinct (Selektivität)
// Anzahl distinct Werte pro Spalte genau spezifizieren
// Vergleichswerte auch zufällig aus generierten Wertebereich holen, damit Compiler nicht optimiert
// Info: Debug Modus sorgt dafür, dass Compiler nicht optimiert, aber Messungen werden im Release Modus gemacht (hier wird vom Compiler optimiert)
// Release Modus -O3 Flag im Terminal
// immer 1000er Trennzeichen nutzen
// sinnvolle Bestandteile in einzelne Methoden kapseln

// To-Do zum nächsten Mal:
// Überprüfen, wo bei Ausgabe der Fehler liegt bei der Darstellung der Millisekunden (ms)
// Aufpassen bei 1024 / Größen
// Methoden kapseln
// Mehr Spalten
// Mehr mit Parametrisierung/Ausgabe beschäftigen
// Ergebnis/Messausgaben in CSV schreiben, nicht während Messungen in CSV schreiben, sondern erst am Ende gesammelt!
// Ergebnisse in einfachen Graphen plotten (z.B. Python - mathplotlib in Jupyter Notebook, plotting am besten über pandas in dataframe)
// Repository für die Gruppe machen und an Betreuer senden

// STD muss nicht vor jedem Befehl gesetzt werden
using namespace std;

// Initialisiere spätere Anzahl der Reihen
const int row_size = 1000000;

//Initialisiere spätere Größe des String Arrays eines Eintrages
const int string_size = 10;

//Initialisiere Vergleichseinträge für den jeweiligen Scan
const int scan_int = 8;
const array<char, string_size> scan_char = {'A','B','C','D','E','F','G','H','I','J'};


// Definiere Struktur der Tabelle
    struct Table{
        vector<int> column_1;
        vector<array<char, string_size>> column_2;
    };

int main() {

    // Seed + Generator von David übernommen
    // Intitialisiere Generator für Zufallswerte
    std::mt19937 generator(1337);  // any generator for random values with seed 1337
    std::uniform_int_distribution<uint32_t > c1_dist(0, UINT32_MAX);
    std::uniform_int_distribution<char> c2_dist(65, 126);

    // Initialisiere Tabelle
    Table table;

    // Befülle Tabellenspalte 1 mit Integer Werten
    for(int i=0;i<row_size; ++i){
        table.column_1.push_back(c1_dist(generator));
    }

    // Befülle Tabellenspalte 2 mit char Werten
    for(int i=0;i<row_size; ++i){


        array<char, string_size> array;

        // Fülle Strings mit Chars auf
        for(int j=0;j<string_size; ++j) {
            array[j] = c2_dist(generator);
        }
        table.column_2.push_back(array);
    }

    // Calculate size of entries for column 1
    int size_column1 = table.column_1.size();

    // Calculate size of entries for column 2
    int size_column2 = table.column_2.size();

    // Initialisiere Zähler Variablen für Columns
    int counter_1 = 0;
    int counter_2 = 0;


    cout << "Size of column 1 (int): " <<  size_column1 <<  "\n" << endl;
    cout << "Size of column 2 (String): " <<  size_column2 << "\n" << endl;


    // Scan für Integer Column 1
    std::chrono::time_point<std::chrono::system_clock> start_column1, end_column1;
    start_column1 = std::chrono::system_clock::now();
    for(int k = 0; k < size_column1; k++){
        if(table.column_1[k]==scan_int){
            counter_1 ++;
             }
        }
    end_column1 = std::chrono::system_clock::now();
    int duration_column1 = std::chrono::duration_cast<std::chrono::nanoseconds> (end_column1-start_column1).count();

    // Scan für Char Column 2
    std::chrono::time_point<std::chrono::system_clock> start_column2, end_column2;
    start_column2 = std::chrono::system_clock::now();
    for(int k = 0; k < size_column2; k++){
        if(table.column_2[k] == scan_char){
            counter_2 ++;
        }
    }
    end_column2 = std::chrono::system_clock::now();
    int duration_column2 = std::chrono::duration_cast<std::chrono::nanoseconds> (end_column2-start_column2).count();


    cout << "Duration Int: " << duration_column1 << endl;
    cout << "Duration Char: " << duration_column2 << endl;


    // Von David übernommen
    // Metriken für Column 1
    const auto byte_per_microsecond_column1 = (static_cast<double>(row_size * 4 / (float) duration_column1));
    const auto gb_per_microsecond_column1 = byte_per_microsecond_column1 / (float)std::pow(1024, 3);
    const auto gb_per_second_column1 = gb_per_microsecond_column1 * 1'000'000.0f;

    const auto rows_count = row_size;

    std::cout << "Rows: " << rows_count <<
              "\nSize: " << row_size * 4 / static_cast<double>(1024 * 1024) << " MB" <<
              //"\nSelectivity: " << selectivity <<
              "\nDuration: " << static_cast<double>(duration_column1) / 1'000 << " ms" <<
              "\nBandwith: " << gb_per_second_column1 << " GB/s\n";

    // Metriken Column 2

    const auto byte_per_microsecond_column2 = (static_cast<double>(row_size * 4 / (float) duration_column2));
    const auto gb_per_microsecond_column2 = byte_per_microsecond_column2 / (float)std::pow(1024, 3);
    const auto gb_per_second_column2 = gb_per_microsecond_column2 * 1'000'000.0f;

    const auto rows_count_column2 = row_size;

    std::cout << "Rows: " << rows_count <<
              "\nSize: " << row_size * 4 / static_cast<double>(1024 * 1024) << " MB" <<
              //"\nSelectivity: " << selectivity <<
              "\nDuration: " << static_cast<double>(duration_column2) / 1'000 << " ms" <<
              "\nBandwith: " << gb_per_second_column2 << " GB/s\n";

    cout << "Counter 1: " << counter_1 << endl;
    cout << "Counter 2: " << counter_2 << endl;

    return 0;

}
