#include <iostream>
#include <bitstream>

void ltoao (long num, char** s) {
  bool is_negative = false;
  // 8 Byte (64 Binärbits) erlauben das Schreiben von maximal 22 Oktalbits
  // Für das Zeilenendezeichen (Terminalzeichen) wird ein Zeichen von +1 benötigt: '0', sein ganzzahliger Code ist 0
  (*s) = new char[24];
  (*s)[0] = ' ';
  (*s)[23] = '\0';

  if (num < 0) {
    is_negative = true;
    (*s)[0] = '-';
    std::bitset<64> bs1(num);
    std::cout<<bs1.to_string()<<"\n";

    num = ~num + 1;
    std::bitset<64> bs2(num);
    std::cout << bs2.to_string() << "\n";
  }

  // 64 Binärbits ermöglichen es Ihnen, eine Oktalzahl aus 21 Ziffern mit einem vollen Bereich und einen unvollständigen Bereich für 22 Oktalbits darzustellen
  // Wir werden aus den unteren Klassen einfügen
  for (int i = 22; i >= 1; i--) {
    // Um eine oktale Ziffer darzustellen, benötigen Sie drei Binärziffern
    // current_digit ist das Ergebnis des Anwendens einer Maske, die die drei niederwertigsten Bits aus der binären Darstellung von num übernimmt 
    unsigned char current_digit = num & 0b00000111;
    // Wir haben eine ganzzahlige Darstellung einer Zahl von 0 bis 7, aber in der char-Darstellung (ASCII) haben die Zeichen ihre eigenen Codes, daher müssen wir den ASCII-Code für das Zeichen nehmen, das der symbolischen Darstellung der Ziffer entspricht, current_digit
    current_digit = '0' + current_digit;
    (*s)[i] = current_digit;
    // Verschieben Sie die Zahl um drei Bits nach rechts, um die drei Binärbits zu entfernen, die wir gerade verarbeitet haben
    // Vor dem SVDIG wird auf vorzeichenlos gegossen, damit während der Schicht die Einheit nicht aus dem Vorzeichen kopiert wird
    num = (unsigned long) num >> 3;
    // Machen Sie dann dasselbe, bis wir 21 Gruppen mit drei Kategorien und einer weiteren Kategorie verarbeiten
  }
}
