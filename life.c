//Name: Ryan Jaskulski
//Date: 10/30/23
//File: gol.c
// Conway's Game of Life
#include <curses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <locale.h>

//Calculate neighbor count for a specific cell
int count_neighbors(const int c, const int r, const int wy, const int wx, int a[wx][wy])
{
    int i, j, count = 0;
    int i2 = 0;
    int j2 = 0;
    for (i = r - 1; i <= r + 1; i++) {
        for (j = c - 1; j <= c + 1; j++) {
            i2 = i; //buffered i
            j2 = j; // buffered j
            if ((i == r && j == c)) { //skip over self
                continue;
            }
            if(i < 0) //begin wraparound logic
            {
                  i2 = wx-1;
            }
            if(j < 0)
            {
                  j2 = wy-1;
            }
            if(i >= wx)
            {
                  i2 = 0;
            }
            if(j >= wy)
            {
                  j2 = 0;
            } // end wraparound logic
            if (a[i2][j2] == 1) {
                count++;
            }
        }
    }
    return count;
}

void load_and_parse_cells(char* filename, int y, int x, int (*world)[y])
{
      /*File format (ex.)

      3 3
       x 
      xxx
       x

      */

      //Open the file
      FILE* cell = fopen(filename, "r");
      
      if(cell == NULL)
      {
            endwin();
            fprintf(stderr, "Unable to open file...\n");
            exit(1);
      }

      ssize_t sread;
      size_t len = 0;
      char* line = NULL;
      
      //Read in the line that contains inital canvas size
      if ((sread = getline(&line, &len, cell)) == -1)
      {
            endwin();
            fprintf(stderr, "Unable to read file...\n");
            exit(1);
      }

      //Get x val
      int cx = 0;
      cx = atoi(strtok(line, " "));

      //Get y val
      int cy = 0;
      cy = atoi(strtok(line, " "));


      int sx = (x/2)-(cx/2); //Where to start pasting the colony on the canvas
      int sy = (y/2)-(cy/2);

      //Read in the colony character representation
      while ((sread = getline(&line, &len, cell)) != -1)
      {
           
           sx = (x/2)-cx; //Where to start pasting the colony on the canvas
           int ln = strlen(line);
           for (int i = 0; i < ln; i++)
           {
                 //Paste individual character representations in cells
                 if(line[i] == 'x')
                 {
                       //Paste live cell
                       world[sx][sy] = 1;
                 }
                 else
                 {
                       //Paste dead cell
                       world[sx][sy] = 0;
                 }
                 sx++;
           }
           sy++;

      }

      fclose(cell);
}


void calc_generation(const int y, const int x, int world[x][y])
{
     int ne[x][y];
     
     //Calc all neighbors
     for (int i = 0; i < y; i++)
     {
           for (int j = 0; j < x; j++)
           {
                 ne[j][i] = count_neighbors(i, j, y, x, world); 
           }
     }

     //Calc living status
     for(int i = 0; i < y; i++)
     {
           for(int j = 0; j < x; j++)
           {
                 if(world[j][i] == 1 && (ne[j][i] == 2 || ne[j][i] == 3)) 
                 {
                       world[j][i] = 1;  
                 }
                 else if(world[j][i] == 0 && ne[j][i] == 3) 
                 {
                       world[j][i] = 1; 
                 }
                 else
                 {
                       world[j][i] = 0; 
                 }
           }
     }

}


int main(int argc, char** argv)
{
      if (argc < 2)
      {
            fprintf(stderr, "usage: life FILE\n");
            return 1;
      }
      int ch = 0;
      int pause_time = 250000;
      const float PAUSE_TIME_MAX = 500000;
      const float PAUSE_TIME_MIN = 10;

      setlocale(LC_ALL, "");
      WINDOW* stdscr = initscr();
      clear();
      cbreak();
      noecho();
      nonl();
      nodelay(stdscr, true);
      curs_set(0);
      int x, y;
      getmaxyx(stdscr, y, x);
      int world[x][y]; //Backing store
                       

     for(int i = 0; i < y; i++)
     {
           for(int j = 0; j < x; j++)
           {
                 world[j][i] = 0;
           }
     }


      //TODO: Load in config from file and setup canvas
      load_and_parse_cells(argv[1], y, x, world);

      first_draw:
            for(int i = 0; i < y; i++)
            {
                  for(int j = 0; j < x; j++)
                  {
                        if(world[j][i] == 1)
                        {
                              mvwprintw(stdscr, i, j, "x");
                        }
                        else
                        {
                              mvwprintw(stdscr, i, j, " ");
                        }
                        //mvwprintw(stdscr, i, j, "%d", world[j][i].neighbors);
                  }
            }
            refresh();

      char* introstring =  "Conroy's Game of Life. Press \'s\' to start...";
      mvwprintw(stdscr, y/2, (x/2)-strlen(introstring)/2, "%s", introstring);
      do
      {
            ch = getch();
            if(ch == 'q') goto end;
            else if (ch == 's'); //Do nothing; let loop break
            else if (ch == ERR); //Do nothing
            else { flash(); clear(); goto first_draw;}
      } while(ch != 's');
      clear();
      int g = 0;
      int alive = 0;
      do {
            alive = 0;
            ch = getch();

            switch(ch)
            {
                  case 'p':
                        mvwprintw(stdscr, y/2, (x/2)-3, "PAUSED");

                        //Trap the program in this loop (essentially pausing it)
                        int pauch;
                        while((pauch = getch()) != 'p')
                        {
                              //Quit
                              if(pauch == 'q') goto end; //Evil goto statement
                              if(pauch == 'r') break;
                        }
                        clear();
                        break;
                  case '+':
                        clear();
                        if(pause_time > PAUSE_TIME_MIN) 
                        {
                              pause_time /= 2.0f;
                        }
                        
                        break;
                  case '_':
                        clear();
                        if(pause_time < PAUSE_TIME_MAX)
                        {
                              pause_time *= 2.0f;
                        } 
                        break;
                  case 'q':
                        goto end; //evil goto statement
                        break;
                  case 'c':
                        clear();
                        break;
                  case ERR:
                        break;
                  default:
                        clear();
                        flash();
                        break;

            }
            calc_generation(y, x, world);


            //Draw all the cells to the screen
            for(int i = 0; i < y; i++)
            {
                  for(int j = 0; j < x; j++)
                  {
                        if(world[j][i] == 1)
                        {
                              wmove(stdscr, i, j);
                              waddch(stdscr, 'x');
                              alive++;
                        }
                        else
                        {
                              wmove(stdscr, i, j);
                              waddch(stdscr, ' ');
                        }
                  }
            }

            //Print other information to the screen
            mvwprintw(stdscr, 0, 0, "Generation: %d, Alive: %d", g, alive);
            wrefresh(stdscr);
            mvwprintw(stdscr, y-1, 0, "SPEED: %d ms delay", pause_time);
            g++;
            wrefresh(stdscr);
            usleep(pause_time);
      }
      while(1);
      //Evil goto tag
      end:
            //End the window object so that it exits cleanly 
            endwin();
            return 0;

}
