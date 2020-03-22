#include <iostream>
#include <fstream>
#include <algorithm>
#include <chrono>
#include <iomanip>
#define DIM_MAX 100000010
using namespace std;
using namespace std::chrono;

ifstream in("sortari.in");
ofstream out("sortari.out");


int arr_random[DIM_MAX], arr_testare[DIM_MAX];
int temp[2][DIM_MAX];

///Comparator pentru sortare descrescatoare
bool cmp(int i,int j){
    return i > j;
}

///functia care genereaza random elementele unui tablou de dimensiune d si cu cel mai mare element mx
void generare(int d, int mx){
    for(int i = 0;i<d;i++){
        arr_random[i] = (int)(1.0 * (rand()+1)/RAND_MAX * mx)%mx + 1;
        arr_testare[i] = arr_random[i];
      //  arr_sortat[i] = arr_random[i];
        //arr_invers[i] = arr_random[i];
    }
    sort(arr_testare, arr_testare + d);

}


void bubblesort(int arr[], int st, int dr){
    if(dr > 10000){
        return;
    }
    for(int i = st;i<dr - 1;i++)
        for(int j = i+1;j<dr;j++)
            if(arr[i] > arr[j])
                swap(arr[i], arr[j]);
    return;
}

void countsort(int arr[], int st, int dr){

    int mx = 0;
    for(int i = st;i<dr;i++){
        if(arr[i] > mx)
            mx = arr[i];
    }

    //cout<<'\n'<<mx<<'\n';
    for(int i = 0;i<=mx;i++){
        temp[0][i] = 0;
    }

    for(int i = st;i < dr;i++){
        temp[0][arr[i]]++;
    }
    int i = 0;
    int k = st;
    while(i<=mx){
        if(temp[0][i] != 0){
            arr[k++] = i;
            temp[0][i]--;
        }
        else i++;
    }
    return;
}



void countsort_for_radix(int st, int dr, int exp){
    int baza = 256;
    int cnt[baza + 10];
    //int temp[dr];
    for(int i = 0 ;i<baza; i++)
        cnt[i] = 0;

    for(int i = st;i < dr;i++){
        cnt[(temp[0][i] >> exp) & (baza - 1)]++;
        //temp_rad[!linie][i] = 0;
    }
    int i,last;
    for (i =last = 0; i < baza; i++) {
        last += cnt[i];
        cnt[i] = last - cnt[i];
    }

    for(i = st; i < dr; i++){
        temp[1][cnt[(temp[0][i] >> exp) & (baza - 1)]++] = temp[0][i];
        //cnt[(temp[0][i] >> exp) & (baza - 1)]++;
    }

    for(int i = st;i<dr;i++)
        temp[0][i] = temp[1][i];
}

void radixsort(int arr[], int st, int dr){

    for(int i = st;i<dr;i++){
        temp[0][i] = arr[i];
        temp[1][i] = 0;
    }

    int mx = arr[st];
    for(int i = st + 1;i<dr;i++){
        if(arr[i] > mx)
            mx = arr[i];
    }
    int exp;
    for( exp = 0; (mx >> exp) > 0; exp+=4){
        countsort_for_radix(st, dr, exp);
      // cout<<mx<<' '<<exp<<' '<<(mx >> exp)<<'\n';
    }
    //countsort_for_radix(st, dr, exp+8);
    for(int i = st;i<dr;i++)
        arr[i] = temp[0][i];

}

void interclasare(int arr[], int st, int mid, int dr){

    int i = st;
    int j = mid;
    int k = st;

    while(i<mid && j<dr){
        if(arr[i] <= arr[j]){
            temp[0][k++] = arr[i];
            i++;
        }
        else{
            temp[0][k++] = arr[j];
            j++;
        }
    }
    while(i<mid){
        temp[0][k++] = arr[i];
        i++;
    }
    while(j<dr){
        temp[0][k++] = arr[j];
        j++;
    }

    for(i = st;i<dr;i++)
        arr[i] = temp[0][i];

    return;

}

void mergesort(int arr[], int st, int dr){
    //if(dr >= 1000000){return;}
    if(dr - st > 1){

        int mid = (st + dr)/2;
        mergesort(arr, st, mid);
        mergesort(arr,mid, dr);
        interclasare(arr, st, mid, dr);

    }
}

void quicksort(int arr[], int st, int dr){
    if (st < dr - 1){
        int i = st, j = dr - 1;
        int piv = arr[(i + j)/2];
        while (i <= j){
           while (i <= dr && arr[i] < piv)
               i++;
           while (j >= st && arr[j] > piv)
               j--;
           if (i <= dr && j >= st && i <= j){
               swap(arr[i], arr[j]);
               i++;
               j--;
           }
        }
        quicksort(arr, st, j+1);
        quicksort(arr, i, dr);
    }
}

void testare(void (*currentsort)(int[], int, int), int d){


    auto start = high_resolution_clock::now();
    (*currentsort)(arr_random,0, d);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout<<setprecision(10)<<"\t\tTimp: "<<1.0*duration.count() /1000000 << " secunde"<<'\n';
    out<<setprecision(10)<<"\t\tTimp: "<<1.0*duration.count() /1000000 << " secunde"<<'\n';
    int i;
    for(i = 0;i<d;i++){
        if(arr_random[i] != arr_testare[i]){
            cout<<"\t\tNesortat!\n";
            out<<"\t\tNesortat!\n";
            break;
        }
    }
    if(i == d){
         cout<<"\t\tSortat!\n";
         out<<"\t\tSortat!\n";
    }

}



int main(){
    int d,mx,t;
    in>>t;
    while(t--){
        in>>d>>mx;
        generare(d,mx);
        cout<<"\nSortarea a "<<d<<" elemente cu nr mai mici ca "<<mx<<":\n\n";
        out<<"\nSortarea a "<<d<<" elemente cu nr mai mici ca "<<mx<<":\n\n";

        auto start = high_resolution_clock::now();
        sort(arr_random, arr_random + d);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<microseconds>(stop - start);

        cout<<"\tSTL Sort:\n";
        out<<"\tSTL Sort:\n";
        cout<<setprecision(10)<<"\t\tTimp: "<<1.0*duration.count() /1000000 << " secunde"<<'\n';
        out<<setprecision(10)<<"\t\tTimp: "<<1.0*duration.count() /1000000 << " secunde"<<'\n';

        generare(d,mx);
        if(d<=10000){
            cout<<"\tBubblesort:\n";
            out<<"\tBubblesort:\n";
            testare(bubblesort, d);
        }
        else{
            cout<<"\tBubblesort: Moare\n";
            out<<"\tBubblesort: Moare\n";
        }
        cout<<"\tCountsort:\n";
        out<<"\tCountsort:\n";
        generare(d,mx);
        testare(countsort, d);
        cout<<"\tRadixsort:\n";
        out<<"\tRadixsort:\n";
        generare(d,mx);
        testare(radixsort, d);
        cout<<"\tMergesort:\n";
        out<<"\tMergesort:\n";
        generare(d,mx);
        testare(mergesort, d);
        cout<<"\tQuicksort:\n";
        out<<"\tQuicksort:\n";
        generare(d,mx);
        testare(quicksort, d);
    }
    return 0;
}
