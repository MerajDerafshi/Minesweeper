#include <bits/stdc++.h>
using namespace std;

bool exsist(string source, char element) { 
    for(int i = 0; i < source.length(); i++) { 
        if(source[i] == element) { 
            return true; 
        } 
    } 
    return false; 
} 
string write(string text) { 
    string space = ""; 
    for(int i = 0; i <= 1; i++) { 
        space += text; 
    }
    return space; 
}
int Segment(string phrase, int segmentstartindex, int step) { 
    string segment[12]; 
    segment[0] = "ABCDEFGIMNOPQRTZ12356789"; 
    segment[1] = "ABCDEFGIKMOPQRSTZ2356789"; 
    segment[2] = "ACEFGHKLMNOPQRUVWXY567894"; 
    segment[3] = "BDIKMNSTX14"; 
    segment[4] = "ABDHJMNOPQRUVWYZ23789"; 
    segment[5] = "AEFHKPQRXY245689"; 
    segment[6] = "ABFHKPQRVXYZ23456789"; 
    segment[7] = "ACEFGHJKLMNOPQRUVW2689"; 
    segment[8] = "BDINQRSTVWXYZ147"; 
    segment[9] = "ABDGHJKMNOUWX35689"; 
    segment[10] = "BCDEGIJLOSUVWZ1235689"; 
    segment[11] = "BCDEGIJLNOQRUWZ1235689";
    if(step == 1) { 
        for(int i = 0; i < phrase.length(); i++) { 
            cout << " "; 
            if(exsist(segment[segmentstartindex], phrase[i])) { 
                cout << write("━"); 
            } 
            else { 
                cout << write(" "); 
            } 
            cout << " "; 
            if(exsist(segment[segmentstartindex + 1], phrase[i])) { 
                cout << write("━"); 
            } 
            else { 
                cout << write(" "); 
            } 
            cout << "  "; 
        } 
        cout << "\n"; 
    } 
    else { 
        for(int k = 0; k < 1; k++) { 
            for(int i = 0; i < phrase.length(); i++) { 
                if(exsist(segment[segmentstartindex], phrase[i])) { 
                    cout << "┃"; 
                } 
                else { 
                    cout << " "; 
                } 
                cout << write(" ");
                if(exsist(segment[segmentstartindex + 1], phrase[i])) { 
                    cout << "┃"; 
                } 
                else { 
                    cout << " "; 
                }
                cout << write(" "); 
                if(exsist(segment[segmentstartindex + 2], phrase[i])) { 
                    cout << "┃"; 
                } 
                else { 
                    cout << " "; 
                } 
                cout << " "; 
            }
            cout << "\n"; 
        } 
    } 
    return segmentstartindex; 
} 
 
void print() { 
    Segment("MINESWEEPER", 0, 1); 
    Segment("MINESWEEPER", 2, 2); 
    Segment("MINESWEEPER", 5, 1); 
    Segment("MINESWEEPER", 7, 2); 
    Segment("MINESWEEPER", 10, 1); 
    
}