 #include <cstdio>
   #include <cstdlib>
   #include <cmath>
   #include <cstring>
   #include <set>
   #include <vector>
   #include "omp.h"
   
   using namespace std;
   
   double r = 1.6;
   int t_num = 5000;
   int t_len = 5;
   
   struct Clust
   {
     int size;
     int count;
     int width;
     multiset <char> clus;
     Clust(){
         size = 0;
         count = 0;
         width = 0;}
   };
   
   void AddTransaction(Clust &C, char* t)
   {
     C.size = C.size + t_len;
     C.count = C.count + 1;
   
     for (int i = 0; i < t_len; i++){
       if (C.clus.count(t[i]) == 0) {
         C.width = C.width + 1;
       }
       C.clus.insert(t[i]);
     }
   }
   
   void RemoveTransaction(Clust &C, char* t)
   {
     C.size = C.size - t_len;
     C.count = C.count - 1;
   
     for (int i = 0; i < t_len; i++){
       if (C.clus.count(t[i]) == 1) {
         C.width = C.width - 1;
       }
       C.clus.erase(C.clus.find(t[i]));
     }
   }
   
   double AddCost(Clust C, char* t)
   {
     int S = C.size;
     int N = C.count;
     int W = C.width;
     int Snew = S + t_len;
     int Nnew = N + 1;
     int Wnew = W;
     set <int> tmp;
   
     for (int i = 0; i < t_len; i++){
       if (C.clus.count(t[i]) == 0 && tmp.count(t[i]) == 0) {
         Wnew = Wnew + 1;
       }
       tmp.insert(t[i]);
     }
     return (Snew * Nnew) / (pow(Wnew, r)) - ((S == 0) ? 0 : (S * N) / (pow(W, r)));
   }
   
   double RemoveCost(Clust C, char* t)
   {
     int S = C.size;
     int N = C.count;
     int W = C.width;
     int Snew = S - t_len;
     int Nnew = N - 1;
     int Wnew = W;
   
     for (int i = 0; i < t_len; i++){
       if (C.clus.count(t[i]) == 1) {
         Wnew = Wnew - 1;
       }
     }
     return ((Wnew == 0) ? 0 : (Snew * Nnew) / (pow(Wnew, r))) - (S * N) / (pow(W, r));
   }
   
   int main(int argc, char **argv)
   {
     int ClusCnt = 0;
     double maxCost;
     int bestChoice;
     double remCost;
       int i, j, k;
       int tr = omp_get_max_threads();
       bool moved = false;
   
   t_num = atoi(argv[1]);
   t_len = atoi(argv[2]);
   r = atof(argv[3]);
   
     int* Distr = (int*)malloc(t_num * sizeof(int));
   
     char** T = (char **)malloc(t_num * sizeof(char*));
     T[0] = (char *)malloc(t_num * t_len * sizeof(char));
     for (i=1; i < t_num; i++){
       T[i] = T[i-1] + t_len;
     }
   
       vector<Clust> C;
       C.push_back(Clust());
   
     FILE* input = fopen("mushrooms.txt", "r");
         for (i = 0; i < t_num; i++) {
           for (j = 0; j < t_len; j++) {
             fscanf(input, "%c ", &T[i][j]);
           }
         }
   
   //Инициализация
   
     double tm = omp_get_wtime();
     AddTransaction(C[0], T[0]);
     ClusCnt++;
     C.push_back(Clust());
   
       Distr[0] = 0;
   
     for (i = 1; i < t_num; i++){
       maxCost = 0;
       double add[ClusCnt + 1];
   
       #pragma omp parallel for private(j) shared(ClusCnt, add, C, T)
       for (j = 0; j <= ClusCnt; j++){
         add[j] = AddCost(C[j], T[i]);
       }
   
         for (j = 0; j <= ClusCnt; j++){
         if (add[j] > maxCost){
           maxCost = add[j];
           bestChoice = j;
         }
       }
   
   
       if (bestChoice == ClusCnt){
         ClusCnt++;
         C.push_back(Clust());
       }
       AddTransaction(C[bestChoice], T[i]);
       Distr[i] = bestChoice;
     }
   
   //Итерации
     for (k = 0; k < 10; k++){ // k - число итераций
         moved = false;
         double add[ClusCnt + 1];
   
         for (i = 0; i < t_num; i++){
          maxCost = 0;
          remCost = RemoveCost(C[Distr[i]],T[i]);
   
          #pragma omp parallel for private(j) shared(ClusCnt, add, C, T, Distr, remCost)
          for (j = 0; j <= ClusCnt; j++){
           if (j != Distr[i]) {
             add[j] = AddCost(C[j], T[i]) + remCost;
             }
          }
           for (j = 0; j <= ClusCnt; j++){
             if (j != Distr[i]){
             if (add[j] > maxCost) {
               maxCost = add[j];
               bestChoice = j;
               }
             }
           }
   
           if (maxCost > 0){
             if (bestChoice == ClusCnt){
               ClusCnt++;
               C.push_back(Clust());
             }
             RemoveTransaction(C[Distr[i]], T[i]);
             AddTransaction(C[bestChoice], T[i]);
             Distr[i] = bestChoice;
             moved = true;
           }
         }
     }
   
   tm = omp_get_wtime() - tm;
   
   printf("size:%d\twidth:%d\tcl_cnt:%d\threads:%d\ttime:%lf seconds\tr:%lf",t_num,t_len,ClusCnt - 1,tr,tm,r);
   
   
   FILE* output = fopen("out.txt","w");
   for (int j = 0; j <= ClusCnt; j++){
       fprintf(output,"Cluster %d:\n",j);
       for (multiset<char>::iterator it = C[j].clus.begin(); it != C[j].clus.end(); it++){
               fprintf(output,"%c",*it);
       }
       fprintf(output,"\n");
   }
   
   fclose(input);
   fclose(output);
   free(T[0]);
   free(T);
   free(Distr);
   
   return 0;
   }