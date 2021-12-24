# analizator-leksykalny
Proszę zaprojektować i wykonać program wypisujący drzewo wywołań funkcji na podstawie analizy programu w języku C.

Program ma być wywoływany z listą argumentów - nazw plików zawierających kod w jęz. C. Program ma czytać te pliki i na ich podstawie budować strukturę danych określającą z jakich funkcji składa się analizowany kod i jak te funkcje są powiązane wywołaniami. Na zakończenie program powinien wypisywać drzewo wywołań funkcji.

1. Program powinien umożliwiać:
 - przetwarzanie dowolnie wielu plików w jednym przebiegu
 - ignorowanie pewnych funkcji (zgodnie z lista domyślną, lub listą dostarczoną przez użytkownika)
 - wypisywanie numerów linii, gdzie rozpoczyna i kończy się funkcja oraz gdzie są wywoływane poszczególne funkcje
2. Program powinien poprawnie reagować na niezbilansowanie nawiasów klamrowych w plikach źródłowych.
3. Program powinien poprawnie obsługiwać komentarze
4. Program ma być zbudowany modularnie. Podział na pliki źródłowe ma być odzwierciedleniem funkcjonalnej struktury programu. Przykładowa lista modułów:
 - analizator leksykalny - zamiana ciągu znaków, jakim jest plik, na ciąg symboli leksykalnych; pomija symbole nieistotne z p. widzenia zadania
 - analizator składni - wykrywanie prototypów, definicji, wywołań funkcji
 - kontener danych - przechowywania w pamięci drzewa wywołań
 - lista funkcji pomijanych - wykorzystywana przez analizator składni do określenia, czy zapisywać dane o funkcji, czy nie
 - formater wydruku - wypisuje zawartość kontenera w odpowiedniej formie
 - sterowanie - funkcja main: analizuje argv, inicjuje kontener, wywołuje analizator składni
 
  Opcje:
    - obsługiwanie zagnieżdżonych plików włączanych przez include
    - ignorowania makrodefinicji: Jeżeli w pliku źródłowym występuje np. makrodefinicja
