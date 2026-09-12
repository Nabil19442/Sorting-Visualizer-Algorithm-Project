#include<bits/stdc++.h>
#include<windows.h>
#include<conio.h>
#include<iomanip>
using namespace std;

HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

int SPEED = 150;
bool paused = false;
string stepMsg = "";
string explanation = "";

// ---------------- BASIC ----------------

void gotoxy(int x,int y){
    COORD c{(SHORT)x,(SHORT)y};
    SetConsoleCursorPosition(hConsole,c);
}

void setcolor(int c){
    SetConsoleTextAttribute(hConsole,c);
}

void getConsoleSize(int &w,int &h){
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole,&csbi);
    w = csbi.srWindow.Right - csbi.srWindow.Left + 1;
    h = csbi.srWindow.Bottom - csbi.srWindow.Top + 1;
}

// ---------------- CONTROL ----------------

void handleControls(){
    if(_kbhit()){
        int ch=_getch();
        if(ch==32) paused=!paused;
    }

    while(paused){
        if(_kbhit()){
            int ch=_getch();
            if(ch==32) paused=false;
        }
    }
}

// ---------------- BOX ----------------

void drawBox(int x,int y,int w,int h){
    setcolor(14);

    for(int i=0;i<w;i++){
        gotoxy(x+i,y); cout<<char(205);
        gotoxy(x+i,y+h); cout<<char(205);
    }
    for(int i=0;i<h;i++){
        gotoxy(x,y+i); cout<<char(186);
        gotoxy(x+w,y+i); cout<<char(186);
    }

    gotoxy(x,y); cout<<char(201);
    gotoxy(x+w,y); cout<<char(187);
    gotoxy(x,y+h); cout<<char(200);
    gotoxy(x+w,y+h); cout<<char(188);

    setcolor(7);
}

// ---------------- LOADING ----------------

void loadingScreen(){
    system("cls");

    int sw,sh;
    getConsoleSize(sw,sh);

    gotoxy(sw/2-15,sh/2-2);
    setcolor(11);
    cout<<"LOADING SORTING VISUALIZER";

    gotoxy(sw/2-25,sh/2);
    for(int i=0;i<50;i++){
        setcolor(14);
        cout<<char(219);
        Sleep(15);
    }
}

// ---------------- ENDING ----------------

void endingScreen(){
    system("cls");

    int sw,sh;
    getConsoleSize(sw,sh);

    setcolor(10);
    gotoxy(sw/2-10,sh/2-2);
    cout<<"THANK YOU!";

    gotoxy(sw/2-18,sh/2);
    cout<<"Closing Visualizer...";

    gotoxy(sw/2-25,sh/2+2);
    for(int i=0;i<50;i++){
        setcolor(14);
        cout<<char(219);
        Sleep(15);
    }

    Sleep(500);
}

// ---------------- UI ----------------

int baseX, baseY;

void drawStaticUI(string title,string complexity){
    system("cls");

    int sw,sh;
    getConsoleSize(sw,sh);

    baseX=(sw-40)/2;
    baseY=(sh-18)/2;

    setcolor(11);
    gotoxy(sw/2-8,baseY-2);
    cout<<title;

    setcolor(10);
    gotoxy(baseX,baseY+15);
    cout<<"Time Complexity:";
    gotoxy(baseX,baseY+16);
    cout<<complexity;

    setcolor(8);
    gotoxy(baseX,baseY+18);
    cout<<"SPACE: Pause/Resume";

    // explanation panel
    setcolor(11);
    gotoxy(baseX+45,baseY);
    cout<<"EXPLANATION";
}

// ---------------- GRAPH ----------------

void drawGraph(vector<int>&arr,int a=-1,int b=-1,int sorted=-1){

    handleControls();

    int maxVal=*max_element(arr.begin(),arr.end());
    int height=12;

    for(int row=height;row>=1;row--){
        gotoxy(baseX, baseY+(height-row));

        for(int i=0;i<arr.size();i++){
            int bar=(arr[i]*height)/maxVal;

            if(bar>=row){

                if(i==a || i==b) setcolor(12);
                else if(i>=sorted && sorted!=-1) setcolor(10);
                else setcolor(7);

                cout<<setw(2)<<char(219);
            }
            else cout<<setw(2)<<" ";
        }
    }

    gotoxy(baseX, baseY+height+1);
    setcolor(7);
    for(int v:arr) cout<<setw(2)<<v;

    gotoxy(baseX, baseY+height+3);
    setcolor(11);
    cout<<"Step: "<<stepMsg<<"           ";

    // explanation text
    gotoxy(baseX+45,baseY+4);
    setcolor(7);
    cout<<explanation<<"            ";

    Sleep(SPEED);
}


// ---------------- SORTS ---------------- 

// Bubble
void bubbleSort(vector<int>&arr){
    drawStaticUI("BUBBLE SORT","Best: O(n)  Avg: O(n^2)  Worst: O(n^2)");

    int n=arr.size();

    for(int i=0;i<n;i++){
        for(int j=0;j<n-i-1;j++){

            explanation="Compare adjacent elements";
            stepMsg="Compare "+to_string(arr[j])+" & "+to_string(arr[j+1]);

            drawGraph(arr,j,j+1,n-i);

            if(arr[j]>arr[j+1]){
                explanation="Swapping elements";
                stepMsg="Swap "+to_string(arr[j])+" & "+to_string(arr[j+1]);

                swap(arr[j],arr[j+1]);
                Beep(800,30);

                drawGraph(arr,j,j+1,n-i);
            }
        }
    }
}

// Insertion
void insertionSort(vector<int>&arr){
    drawStaticUI("INSERTION SORT","Best: O(n)  Avg: O(n^2)  Worst: O(n^2)");

    for(int i=1;i<arr.size();i++){
        int key=arr[i];
        int j=i-1;

        while(j>=0 && arr[j]>key){
            explanation="Shifting element";
            stepMsg="Move "+to_string(arr[j]);

            arr[j+1]=arr[j];
            Beep(800,30);
            drawGraph(arr,j,j+1,i);
            j--;
        }
        arr[j+1]=key;
    }
}

// Selection
void selectionSort(vector<int>&arr){
    drawStaticUI("SELECTION SORT","Best: O(n^2)  Avg: O(n^2)  Worst: O(n^2)");

    for(int i=0;i<arr.size();i++){
        int minIdx=i;

        for(int j=i+1;j<arr.size();j++){
            explanation="Searching minimum";
            if(arr[j]<arr[minIdx]) minIdx=j;

            drawGraph(arr,minIdx,j,i);
        }

        explanation="Swapping min element";
        swap(arr[i],arr[minIdx]);
        Beep(900,30);
    }
}

// Quick
int partition(vector<int>&arr,int l,int h){
    int pivot=arr[h];
    int i=l-1;

    for(int j=l;j<h;j++){
        explanation="Compare with pivot";
        stepMsg="Pivot "+to_string(pivot);

        drawGraph(arr,h,j);

        if(arr[j]<pivot){
            i++;
            swap(arr[i],arr[j]);
            Beep(800,30);
        }
    }

    swap(arr[i+1],arr[h]);
    return i+1;
}

void quickSort(vector<int>&arr,int l,int h){
    if(l>=h) return;

    int pi=partition(arr,l,h);

    explanation="Pivot placed correctly";
    drawGraph(arr,pi,pi,pi);

    quickSort(arr,l,pi-1);
    quickSort(arr,pi+1,h);
}

// Merge
void merge(vector<int>&arr,int l,int m,int r){
    vector<int> t;
    int i=l,j=m+1;

    while(i<=m && j<=r){
        if(arr[i]<arr[j]) t.push_back(arr[i++]);
        else t.push_back(arr[j++]);
    }

    while(i<=m) t.push_back(arr[i++]);
    while(j<=r) t.push_back(arr[j++]);

    for(int k=0;k<t.size();k++){
        arr[l+k]=t[k];
        explanation="Merging arrays";
        drawGraph(arr,l);
    }
}

void mergeSort(vector<int>&arr,int l,int r){
    if(l>=r) return;
    int m=(l+r)/2;
    mergeSort(arr,l,m);
    mergeSort(arr,m+1,r);
    merge(arr,l,m,r);
}

// ---------------- MENU ----------------

int menu(){
    vector<string> items={
        "Bubble Sort",
        "Insertion Sort",
        "Selection Sort",
        "Quick Sort",
        "Merge Sort",
        "Exit"
    };

    int sel=0;

    while(true){
        system("cls");

        int sw,sh;
        getConsoleSize(sw,sh);

        int x=sw/2-20;
        int y=sh/2-8;

        drawBox(x,y,40,16);

        setcolor(11);
        gotoxy(sw/2-10,y+1);
        cout<<"SORTING VISUALIZER";

        for(int i=0;i<items.size();i++){
            gotoxy(x+3,y+3+i);

            if(i==sel){
                setcolor(10);
                cout<<"> "<<items[i];
            } else {
                setcolor(7);
                cout<<"  "<<items[i];
            }
        }

        setcolor(8);
        gotoxy(x+3,y+12);
        cout<<"UP/DOWN : Move";
        gotoxy(x+3,y+13);
        cout<<"ENTER   : Select";

        int ch=_getch();
        if(ch==224){
            ch=_getch();
            if(ch==72) sel=(sel-1+items.size())%items.size();
            else if(ch==80) sel=(sel+1)%items.size();
        }
        else if(ch==13) return sel;
    }
}

// ---------------- INPUT ----------------

void inputArray(vector<int>&arr){
    system("cls");

    int sw,sh;
    getConsoleSize(sw,sh);

    int x = sw/2 - 20;
    int y = sh/2 - 5;

    drawBox(x,y,40,10);

    setcolor(11);
    gotoxy(sw/2-10,y+1);
    cout<<"INPUT ARRAY";

    gotoxy(x+3,y+4);
    cout<<"Enter 10 numbers:";

    gotoxy(x+3,y+6);

    arr.clear();
    for(int i=0;i<10;i++){
        int v;
        cin>>v;
        arr.push_back(v);
    }
}

// ---------------- MAIN ----------------

int main(){
    loadingScreen();

    while(true){
        int choice=menu();
        if(choice==5) break;

        vector<int> arr;
        inputArray(arr);

        if(choice==0) bubbleSort(arr);
        else if(choice==1) insertionSort(arr);
        else if(choice==2) selectionSort(arr);
        else if(choice==3) quickSort(arr,0,9);
        else if(choice==4) mergeSort(arr,0,9);

        getch();
    }

    endingScreen();
    return 0;
}