/* A Sudoku Solving Machine */

#include <stdio.h>
#include <stdlib.h>


#define R 9
#define C 9
#define MEMORY 200000

char sudo_grid[9][9];
int combo[9][9];
int triplo[9][9][9];
int squares_filled;
int run_state = 1;	// our solving machine is working. run_state-> 0 machine stops.
int stack_pointer = 0;
int vm_step;
        //  USING  stack_pointer and vm_step to control the flow of program's loading and saving.


struct coordinates
{
	int r;
	int c;
} *target;


struct machine
{
	int registry[MEMORY][9][9][9];
}vm;


struct State
{
	int counter [9][9][9];
	char step_back[9][9];
	char saved_data[9][9];
	int combo_data[9][9];

	int selected_before[9];
	struct coordinates setted;
	struct State *previous;
} Database[200000];


void print_vmregistry()
{
	int i,j,k,l;

	for(i = 0; i <= vm_step; i++)
	{	for(j = 0; j < 9; j++)
		{
			for(k = 0; k<9;k++)
				for(l = 0;l<9;l++)
					printf("[%d][%d][%d][%d] = %d\n",i,j,k,l,vm.registry[i][j][k][l]);
		}
		printf("\n");
	}
}
char int_to_char(int src)
{
	switch(src)
	{
		case 1:		return '1';
		case 2:		return '2';
		case 3:		return '3';
		case 4:		return '4';
		case 5:		return '5';
		case 6:		return '6';
		case 7:		return '7';
		case 8:		return '8';
		case 9:		return '9';

		default:	fprintf(stderr,"Fatal1: shouldn't ever get that value here.Exit auto scheduled\n");
				exit(1);
	}
}


void save_state(struct State *sp)
{

	int i,j;

	for(i = 0; i < 9; i++)
		for(j = 0; j < 9; j++)
			sp->saved_data[i][j]=sudo_grid[i][j];


}


int char_to_int(char src)
{
	switch(src)
	{
		case '1':	return 1;
		case '2':	return 2;
		case '3':	return 3;
		case '4':	return 4;
		case '5':	return 5;
		case '6':	return 6;
		case '7':	return 7;
		case '8':	return 8;
		case '9':	return 9;

		default:	fprintf(stderr,"Fatal2: shouldn't ever get that value here.Exit auto scheduled\n");
				exit(1);
	}

}


struct coordinates * target_square()
{
	int i,j;
	struct coordinates *rv;

	rv->r = 0;
	rv->c = 0;

	for(i = 0; i<9; i++)
	{
		for(j = 0; j < 9; j++)
		{
			if(combo[i][j] > combo[rv->r][rv->c])
	 		{
				rv->r = i;
				rv->c = j;
			}
		}
	}
	return rv;

}

int is_valid(int row,int column,char value)		// 0-invalid 1-valid
{

	int i,j;
	int sm_row,sm_column;

	for(i = 0; i < 9; i++)
	{
		if( (sudo_grid[i][column] == value) || (sudo_grid[row][i] == value))
			return 0;

	}

	sm_row = (row/3)*3;
	sm_column = (column/3)*3;

	for(i = sm_row; i < sm_row+3; i++)
	{
		for(j = sm_column; j < sm_column+3; j++)
		{
			if(sudo_grid[i][j] == value)
				return 0;

		}
	}

	return 1;
}

void mysleep()
{
	int i,j;
	for(i  = 0; i < 200000; i++)
		for(j = 0; j <7000; j++)
			;
}


int find_combo(int row,int column)
{

	int i,j,ref,sm_row,sm_column,cnt = 0;

	int z,k,l;

	for(z=0;z<9;z++)
		for(k=0;k<9;k++)
			for(l=0;l<9;l++)
				triplo[z][k][l] = 0;

	for(i = 0; i < 9; i++)
	{
		if(i == column)
			continue;

		if(sudo_grid[row][i] != '0')
		{
			ref = char_to_int(sudo_grid[row][i]);
			triplo[row][column][ref-1] = 1;

		}

	}

	for(i = 0; i < 9; i++)
	{
		if(i == row)
			continue;

		if(sudo_grid[i][column] != '0')
		{
			ref = char_to_int(sudo_grid[i][column]);
			triplo[row][column][ref - 1] = 1;	// ref - 1. --> f.e triplo[k][x][0] = 1  no 1 allowed.
		}

	}

	sm_row = (row/3)*3;
	sm_column = (column/3)*3;

	for(i = sm_row; i <sm_row + 3; i++)
		for(j = sm_column; j < sm_column +3; j++)
		{
			if( (i == row) && (j == column) )
				continue;
			if(sudo_grid[i][j] != '0')
			{
				ref = char_to_int(sudo_grid[i][j]);
				triplo[row][column][ref - 1] = 1;
			}


		}
	// computing different alternatives for square[row,column]

	for( i = 0; i < 9; i++)
		if(triplo[row][column][i] == 1)
			cnt++;

	return cnt;
}


void combo_update(void)				// Updates the Combo Square that is Used to decide the next step each time.
{
	int i,j,rv;
	squares_filled = 0;

	for(i = 0; i < 9; i++)
		for(j = 0; j< 9; j++)
		{
			if(sudo_grid[i][j] !='0')
			{
				combo[i][j] = 0;
				squares_filled += 1;
				continue;
			}
			rv = find_combo(i,j);
			combo[i][j] = rv;
		}

}



void display_with_arg(char grid[9][9])
{
	int i,j;
	printf("			SUDOKU\n");
	printf("_________________________________________________________________\n");
	for(i = 0; i <R; i++)
	{	for(j = 0; j < C; j++)
		{
			if(grid[i][j] =='0')
				printf("_\t");
			else
				printf("%c\t",grid[i][j]);
		}
		if((i == 2) || (i == 5))
		{
			putchar('\n');
			printf("_________________________________________________________________\n");
		}
		printf("\n\n");
	}

	printf("_________________________________________________________________\n");
}



void display_puzzle()
{
	int i,j;
	printf("			SUDOKU\n");
	printf("_________________________________________________________________\n");
	for(i = 0; i <R; i++)
	{	for(j = 0; j < C; j++)
		{
			if(sudo_grid[i][j] =='0')
				printf("_\t");
			else
				printf("%c\t",sudo_grid[i][j]);
		}
		if((i == 2) || (i == 5))
		{
			putchar('\n');
			printf("_________________________________________________________________\n");
		}
		printf("\n\n");
	}

	printf("_________________________________________________________________\n");
}

void display_combo(void)
{
	int i,j;
	printf("			COMBO SQUARE\n");
	printf("_________________________________________________________________\n");
	for(i = 0; i <R; i++)
	{	for(j = 0; j < C; j++)
		{
			printf("%d\t",combo[i][j]);
		}

		printf("\n\n");
	}

	printf("_________________________________________________________________\n");
}

void load_state(struct State * sp)
{
	int i,j,k;
	int flag = 0;
	int preload_flag = 0;
	int pre_cnt = 0;
	int valid_values = 0;
	////printf("LOADING SECTION\n");

	for(i = 0; i < 9; i++)
			for(j = 0; j < 9; j++)
			{
				sudo_grid[i][j] = sp->saved_data[i][j];
			}
	////printf("Puzzle Loaded\n");

	combo_update();		//	IMPORTANT TO UPDATE COMBO STRAIGHT AWAY !!!!!!
	////display_puzzle();

	// if still in DEAD END we got to load from further back a step.

	////printf("VM_STEP: %d load1. \n",vm_step);
	for(i = 0; i < 9; i++)
	{
		if( is_valid ( sp->setted.r , sp->setted.c , int_to_char(i+1) ))
		{
			valid_values++;

			if(sp->selected_before[i] == 1)
				pre_cnt++;
		}
	}

	if(pre_cnt >= valid_values)
	{
        // Before we load we got to delete any saved data for the previously followed path,
		// to avoid abnormalities and faulty execution.

		int m1,m2,m3;
        for(m1 = 0; m1 < 9; m1++)
            for(m2 = 0; m2<9;m2++)
                for(m3 = 0;m3<9;m3++)
                    vm.registry[vm_step][m1][m2][m3] = 0;

        for(m1 = 0; m1 < 9; m1++)
            sp->selected_before[m1] = 0;

        vm_step--;
        stack_pointer--;
        ////printf("KANW ENA STEP PISW LOAD. i diefthisni pou fortonw %d   kai %d sizeof struct\n",&Database[vm_step],sizeof(Database[0]));

		load_state(&Database[vm_step]);
	}
}

void solve(void)
{
    static int stage = 0;
    int i,flag = 0;
    printf(" \b"); // i just can't run it without printing somethin'.(!?)
    //printf("%d\n",++stage);
  	//printf(" \n");  // if this is deleted program causes segmentation fault..(?)---> WHY!!?
    target = target_square();

    ////printf("stack_pointer: %d\n",stack_pointer);
	////printf("Targeted square %d %d\n",target->r,target->c);

	for(i = 1; i <= 9; i++)
	{
        //printf("%d  %d selected_before\n",i,Database[stack_pointer-1].selected_before[i-1]);

		if(vm.registry[vm_step][target->r][target->c][i-1] == 1)
			continue;

		if(is_valid(target->r,target->c,int_to_char(i)))
		{
			////printf("Einai valid na parei: (%d)\n",i);
			//saving old state.

			int jim,jon;

			for(jim = 0; jim < 9; jim++)

				for(jon = 0;jon<9;jon++)

					Database[stack_pointer].step_back[jim][jon] = sudo_grid[jim][jon];

			//writing on sudo_grid the new number
			sudo_grid[target->r][target->c] =int_to_char(i);

			//save for the combo_data array
			Database[stack_pointer].previous = &Database[stack_pointer -1] ;
			Database[stack_pointer-1].setted.r = target->r;
			Database[stack_pointer-1].setted.c = target->c;

			combo_update();

			int pr,pc;

			for(pr = 0;pr<9;pr++)
				for(pc = 0; pc<9;pc++)
					Database[stack_pointer ].combo_data[pr][pc] = combo[pr][pc];

			vm.registry[vm_step][target->r][target->c][i-1] = 1;
			////printf("setted to vm[%d][%d][%d][%d] sto %d\n",vm_step,target->r,target->c,i-1,vm.registry[vm_step][target->r][target->c][i-1]);
			Database[stack_pointer-1].counter[target->r][target->c][i-1] =1;

            save_state(&Database[stack_pointer]);

            ////printf("EGINE SAVE STI DIEFTHINSI %d \n",&Database[vm_step]);
			vm_step+=1;
			Database[stack_pointer-1].selected_before[i - 1] = 1;
		////	printf("setted Database tou %d -1 selected before tou %d -1 sto 1\n",stack_pointer,i);
			stack_pointer++;
			flag = 1;
			break;
		}
	}

	////printf("To flag einai %d\n",flag);

	if(flag == 0)
	{
		////printf("\nCannot set (%d,%d) \nDEAD END POINT OCCURANCE.\n",target->r,target->c);
		////printf("Path abortion.\n");
		////printf("Stack_pointer is %d vm_step %d\n",stack_pointer,vm_step);
		////printf("Loading previous data\n");
		--stack_pointer;
		vm_step--;
 		load_state(&Database[stack_pointer  ]);
	}



	combo_update();
	////printf("STAGE %d  stack_pointer: %d init_sq %d  sq_filled:%d\n",++stage,stack_pointer,28,squares_filled);
	////printf("stack_pointer: %d\n",stack_pointer);
    stage++;
	int a;
	////printf("Oti na nai ekane targeted: %d %d\n",target->r,target->c);

	////printf("VM_STEP values gia vm_%d \n",vm_step);
	////for(a= 0; a <9;a++)
		////printf("%d %d\n",a+1,vm.registry[vm_step-1][target->r][target->c][a]);
//	//display_puzzle();
    char ch;

//	scanf("%c\n",&ch);  // mia scanf an theloume na to treksoume vhma vhma to programma kai me ta display_puzzles na vlepoume ti kanei to programma svisimo k anathesi timwn
//                                  Gia xarhn taxuteras ola displays kai print outs einai san sxolia.
	if(ch == 'q')
		exit(1);

    if(squares_filled == 81)
	{
		printf("\nSolution complete %c.\nCompleted after %d stages.\n",7,stage); // and a sound here notifies user.
		//mysleep();

		//printf("\nPRINT OUT:\n");
		/*int t,j;
		for(t = 0; t <= 10; t++)
        {
            for(i  = 0; i < 200000; i++)
                for(j = 0; j <850; j++)
                    ;
            printf("******");
        }
		printf("\n");

        for(i  = 0; i < 200000; i++)
                for(j = 0; j <850; j++)
                    ;
		*/run_state = 0;
	}
}
int main(int argc,char *argv[])
{
	char data;
	FILE *fp;
	int i,j;

	vm_step =0;

    for(i = 0 ; i < 200000;i++)

		for(j = 0; j < 9; j++)
			Database[i].selected_before[j] = 0;

	i = j = 0;
  fp = fopen("zero.txt","r");
    //fp = fopen("sudoku_values1.txt","r");
//  fp = fopen("sudoku_values2.txt","r");
//fp= fopen("easier_one.txt","r");
    //fp = fopen("data1.txt","r");
    //fp = fopen("internet_puzzle2.txt","r");
	//fp = fopen("internet_puzzle1.txt","r");
	if(fp == NULL)
	{
		printf("Fatal: error loading the sudoku data\n");
		exit(1);
	}

	while(!feof(fp))
	{
		fscanf(fp,"%c",&data);
		if(data == '\n')
		{
			j = 0;	//	needed here i don't know why,but works.
			continue;
		}
		sudo_grid[i][j] = data;
		j++;
		if( (j%9 ) == 0)
		{
			j = 0;
			i++;
		}
	}
	fclose(fp);

	printf("Initial data loaded on sudo_grid\n");
	printf("displaying initial state\n");

	display_puzzle();

	combo_update();
    display_combo();
	save_state(&Database[0]);	// initially saving data.(As given puzzle).
	stack_pointer++;

	while(run_state)
	{
		combo_update();
		solve();
	}
    printf("Squares_filled now %d\n",squares_filled);
    display_puzzle();
    system("pause");
	return 0;
}
