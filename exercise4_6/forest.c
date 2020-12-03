#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>

#include "neillsimplescreen.h"

#define G 250
#define L 2500
#define WIDTH 80
#define HEIGHT 30
#define GENS 1000

/*Mark three states and assign values*/
typedef enum status {empty=' ',tree='@',fire='*'} status;
/*Instead of Boolean*/
typedef enum logic {f,t} logic;

void test(void);
void init(status a[HEIGHT][WIDTH]);
void exchange(status a[HEIGHT][WIDTH],status b[HEIGHT][WIDTH]);
void process(status a[HEIGHT][WIDTH]);
void boundary(status a[HEIGHT][WIDTH],status b[HEIGHT+2][WIDTH+2]);
logic onfire(status a[HEIGHT][WIDTH],int x, int y);
void display(status a[HEIGHT][WIDTH]);

int main(void) {
        int i;
        status grid[HEIGHT][WIDTH];
        /*test();*/
        init(grid);
        neillclrscrn();/*Clear screen*/
        for (i = 0; i < GENS; i++) {
                neillcursorhome();/*Move cursor home every generations*/
                process(grid);
                display(grid);
                neillreset();/*Reset text/colour styles*/
                printf("This has developed %d generations.",i+1);
                neillbusywait(0.1);/*Time interval*/
                if (i==GENS-1) {
                        printf("\n");/*Just for beauty*/
                }
        }
        return 0;
}
void test(void) {
        int i,j;
        int row,cow;
        status a[HEIGHT][WIDTH],b[HEIGHT][WIDTH];

        init(a);
        row=17;
        cow=20;
        assert(a[row][cow]==empty);

        exchange(a,b);
        row=22;
        cow=65;
        assert(b[row][cow]==a[row][cow]);

        row=14;
        cow=29;
        for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                        if (i==row&&j==cow) {
                                a[i][j]=tree;
                        }
                        else if (i==row+1&&j==cow-1) {
                                a[i][j]=fire;
                        }
                        else if (i==0&&j==0) {
                                a[i][j]=tree;
                        }
                        else if (i==1&&j==1) {
                                a[i][j]=fire;
                        }
                        else if (i==0&&j==cow) {
                                a[i][j]=tree;
                        }
                        else if (i==1&&j==cow-1) {
                                a[i][j]=fire;
                        }
                }
        }
        assert(onfire(a,row,cow));
        assert(onfire(a,0,0));
        assert(onfire(a,0,cow));

        process(a);
        assert(a[row][cow]==fire);
        assert(a[0][0]==fire);
        assert(a[0][cow]==fire);
        assert(a[row+1][cow-1]==empty);
        assert(a[1][1]==empty);
        assert(a[1][cow-1]==empty);
}
/*Set initial value for empty space*/
void init(status a[HEIGHT][WIDTH]) {
        int i,j;
        for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                        a[i][j]=empty;
                }
        }
}
/*To exchange the value of two arrays*/
void exchange(status a[HEIGHT][WIDTH],status b[HEIGHT][WIDTH]){
        int i,j;
        for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                        b[i][j]=a[i][j];
                }
        }
}
/*Forest tree status update*/
void process(status a[HEIGHT][WIDTH]) {
        status b[HEIGHT][WIDTH];
        int i,j;
        srand(time(0));
        exchange(a,b);
        for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                        if (a[i][j]==empty) {
                                if (rand()%G+1==G) {
                                        b[i][j]=tree;/*Grow*/
                                }
                        }
                        else if (a[i][j]==fire) {
                                b[i][j]=empty;
                        }
                        else if (a[i][j]==tree) {
                                if (onfire(a,i,j)) {
                                        b[i][j]=fire;
                                }
                                else if (rand()%L+1==L) {
                                        b[i][j]=fire;/*Lightning*/
                                }
                        }
                }
        }
        exchange(b,a);
}
/*To prevent boundary value overflow*/
void boundary(status a[HEIGHT][WIDTH],status b[HEIGHT+2][WIDTH+2]){
        int i,j;
        for (i = 0; i < HEIGHT+2; i++) {
                for (j = 0; j < WIDTH+2; j++) {
                        if (i==0||j==0||i==HEIGHT+1||j==WIDTH+1) {
                                b[i][j]=empty;
                        }
                        else{
                                b[i][j]=a[i-1][j-1];
                        }
                }
        }
}
/*Determine if neighboring trees are on fire*/
logic onfire(status a[HEIGHT][WIDTH], int x, int y) {
        int i,j;
        logic k=f;
        status b[HEIGHT+2][WIDTH+2];
        boundary(a,b);
        for (i = x; i <= x+2; i++) {
                for (j = y; j <= y+2; j++) {
                        if (b[i][j]==fire) {
                                k=t;
                        }
                }
        }
        return k;
}
/*Display array and set color*/
void display(status a[HEIGHT][WIDTH]) {
        int i,j;
        for (i = 0; i < HEIGHT; i++) {
                for (j = 0; j < WIDTH; j++) {
                        switch (a[i][j]) {
                        case tree: neillfgcol(green);
                                break;
                        case fire: neillfgcol(red);
                                break;
                        default: neillbgcol(black);
                        }
                        printf("%c",a[i][j]);
                }
                printf("\n");
        }
}
