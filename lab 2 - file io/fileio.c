/*
Course: Comp Systems & Networking
Assignment: Lab 2 Part 2 -File IO 
By: Jacob Waxman and Olivia Nye 
Due Date: 01/29/2020

*/
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) 
{
  //opens input file provided as command line arg
  FILE* input = fopen(argv[1],"r");
  //creates array of idle values
  int values[50]; 

  //number of days idle
  int days;
  fscanf(input,"%d",&days);

//reads the values from the input file
  for(int i=0; i<days; i++)
  {
    fscanf(input,"%d",&values[i]);
  } 
  fclose(input);


int total=0;
for (int i=0; i<days; i++)
{
  //sums up the values from each day
total = total + values[i];
}

double rate = total/10080.0; //divides by minutes in a week

double average = (double)total/days; //divides by minutes per day

int min = values[0]; //initialize min to val 0
for (int i=0; i<days; i++)
{
  if (values[i]<values[min]) //if other value is less, new min
  {
    min = i+1;
  }
}

int max = values[0]; //initialize max to val 0
for (int i=0; i<days; i++)
{
  if (values[i]>values[min]) //if other value is more, new max
  {
    max = i+1;
  }
}
//opens output file to print to, provided as command line arg
FILE* output = fopen(argv[2],"w");
fprintf(output,"Total idle time: %d  \n",total); //print total
fprintf(output,"Performance rate of this system during the week: %f\n",rate); // print rate
fprintf(output,"Average daily idle time: %f\n", average);// print average
fprintf(output,"Day with min idle time: %d\n", min); //print min
fprintf(output,"Day with max idle time: %d", max); //print max
fclose(output);

return 0;

}
