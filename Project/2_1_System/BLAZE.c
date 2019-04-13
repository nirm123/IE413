/******************************************/
/* SIGMA RUN DEFAULTS FOR TRANSLATED MODEL */
/*******************************************/
/*

         MODEL DEFAULTS
         --------------

Model Name:         BLAZE
Model Description:  
Output File:        BLAZE.out
Output Plot Style:  NOAUTO_FIT
Run Mode:           HI_SPEED
Trace Vars:         SERVICE[ID[7]],SYSTEM[ID[7]],RATIO[0]
Random Number Seed: 970
Initial Values:     
Ending Condition:   STOP_ON_TIME
Ending Time:        1000.000
Trace Events:       DELIVER
Hide Edges:         

*/
/*******************************************/


#include "sigmafns.h"
#include "sigmalib.h"

/* EVENT FUNCTIONS USED IN THE SIMULATION MODEL */
void   INIT(void);	/***  ***/
void   ARR(void);	/*** Arrival of customers into the system ***/
void   TOPPING(void);	/*** Begin process of creating pizza. ***/
void   COOK(void);	/*** Start baking the pizza ***/
void   READY(void);	/*** Pizza is out of the oven ***/
void   FINISH(void);	/*** Finishing touches on pizza (extra toppings and boxing) ***/
void   DELIVER(void);	/*** Deliver pizza to the customer ***/
void   STOP(void);	/***  ***/

/*   STATE VARIABLE DELARATIONS   */
double SYSTEM[1000];	/***  Time in system  ***/
double SERVICE[1000];	/***  Time being served (first)  ***/
long   QUEUE[2];	/***    ***/
long   SERVER[2];	/***    ***/
long   ID[10];	/***    ***/
double RATIO[3];	/***    ***/

/*   EVENT FUNCTIONS   */
enum
   {
   run_end_event,
   INIT_event,
   ARR_event,
   TOPPING_event,
   COOK_event,
   READY_event,
   FINISH_event,
   DELIVER_event,
   STOP_event,
   };

/*    MAIN PROGRAM     */
int main(int argc, char** argv)
{
  int  next_event;
  char keytoclose = 'p';

  if(!startup_check(0))
    return -1;

  /* Initialize csiglib and simulation */
  while (initialize(argc, (const char * *)argv)) {;

  /* Schedule beginning of simulation */
  event_time = current_time;
  event_type = INIT_event;
  schedule_event();

  /* Schedule end of simulation */
  event_time = stop_time;
  event_type = run_end_event;
  event_priority = 9999;
  schedule_event();

/*  EVENT EXECUTION CONTROL LOOP */
  while (!run_error && !done) {
    /* Pull next event from event list */
    next_event = c_timing();

    /* increment the event count for this event */
    event_count[next_event]++;

    /* Call appropriate event routine */
    switch ( next_event ) {
      case run_end_event:  run_end();
               break;

      case INIT_event:  INIT();
               break;

      case ARR_event:  ARR();
               break;

      case TOPPING_event:  TOPPING();
               break;

      case COOK_event:  COOK();
               break;

      case READY_event:  READY();
               break;

      case FINISH_event:  FINISH();
               break;

      case DELIVER_event:  DELIVER();
               event_trace("DELIVER",event_count[next_event]);
               break;

      case STOP_event:  STOP();
               break;

      }
    }
  }
// experiments terminated
printf("Experiments ended! If runs end early: \n\r1. check fields in *.exp file. \n\r2. check if output file was already open. \n\r");
return 0;
}

void
event_trace(const char * name_of_event,const long count)
{
  c_timest(SERVICE[ID[7]], 1, 0);
  c_sampst(SERVICE[ID[7]], 1, 0);
  c_timest(SYSTEM[ID[7]], 2, 0);
  c_sampst(SYSTEM[ID[7]], 2, 0);
  c_timest(RATIO[0], 3, 0);
  c_sampst(RATIO[0], 3, 0);
  if(trace_flag) fprintf(output_fp, "%9.3f\t %6s\t%6d ",current_time,name_of_event,count);
  if(trace_flag) fprintf(output_fp, "	%7.3g 	%7.3g 	%7.3g \n"
,(double)SERVICE[ID[7]], (double)SYSTEM[ID[7]], (double)RATIO[0]);
}



int
initialize(int argc, const char** argv)
{
static int first_time = 1;     /* First time in initialize? */
static FILE *input_fp;     /* For reading from the input file */
char *exp_file_name;       /* For constructing input file name */
char y_n = 'p';            /* yes/no for file overwrite*/

       char dir[256];
       char fname[256];
       char ext[256];
       char simulation[1024];
       char experient_name[1024];
        _splitpath( argv[0], NULL, dir, fname, ext );
       strcpy(simulation, fname);
       strcat(simulation, ext);
       strcpy(experient_name, fname);
       strcat(experient_name, ".exp");
     printf("Running the simulation: %s\n", simulation);
    if(strlen(dir) !=0)
       printf("In Path: %s\n",dir);
  if (first_time) {
    exp_file_name = _strdup(argv[0]);
    exp_file_name[strlen(exp_file_name)-1] = 'p';
    printf("\nLooking for experiment file: %s\n",experient_name);
    }

  if ((first_time && (input_fp=fopen(exp_file_name,"r"))!=NULL)
                                           || input_fp!=NULL) {
  if (first_time) {
     first_time = 0; /* Reset for next time into initialize */
     printf("Found. Use [Control]-C to abort replications.\n");
     }

  /* We have run control file of type *.exp          */
  /* Read next set of data from run control file.    */
  if (fscanf(input_fp,"%s %1s %ld %lf %d", output_file_name, &y_n, &rndsd, &stop_time, &trace_flag)<4
) {
     /* End of run control file */
     fclose(input_fp);
     return 0;
     }

  if (y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N') { 
  fprintf(stderr,"INPUT ERROR: Invalid append file flag in *.exp file: (y=append, n=overwrite old file)\n"); 
  return 0; 
  }

  if (y_n == 'y' || y_n == 'Y') {
     if ((output_fp = fopen(output_file_name,"a"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
  return 0; 
  }
  }
if (y_n == 'n' || y_n == 'N') {
     if ((output_fp = fopen(output_file_name,"w"))==NULL) { 
     /* Output file can't be opened. */
     fprintf(stderr,"\nINPUT ERROR: Cannot open output file %s in *.exp file\n",argv[1]);
     return 0;
     }
     }

  if (rndsd < 1 || rndsd > 65534) {
     fprintf(stderr,"\nINPUT ERROR: Random seed %ld is not between 0 and 65534\n",rndsd);
     return 0;
     }

  if (stop_time <= 0.0) {
     fprintf(stderr,"\nINPUT ERROR: Stopping time %lf is negative!\n",stop_time);
     return 0;
     }

  if (trace_flag != 0 && trace_flag != 1) {
     fprintf(stderr,"\nINPUT ERROR: Invalid trace_flag=%d: (1=full trace, 0=summary only)\n",trace_flag);
     return 0;
     }

  done = 0;
     }

 else if (first_time) { /* And open failed, implies data from stdin */
    first_time = 0; /* Reset for next time into initialize */
    printf("Not found, input data at the keyboard.\n");
     /* Give output file name */
     while(y_n != 'y' && y_n != 'Y' && y_n != 'n' && y_n != 'N')
         {
         printf("\nOUTPUT FILE (Enter File Name with Path):\n");
         scanf("%s", output_file_name);
         fflush(stdin);
         sprintf(filename,"%.20s", output_file_name);
         printf("WARNING:File %.20s must not be open!!\n If file does not exist it will be created.\n",filename);
         printf("Do you want the new output appended to this file? (yes/[no])\n");
         scanf("%1s",&y_n);
         fflush(stdin);
         }
     if(y_n == 'y' || y_n == 'Y') output_fp = fopen(filename,"a");
     if(y_n == 'n' || y_n == 'N') output_fp = fopen(filename,"w");

     /* Read in random number seed */
     printf("\n\nRANDOM NUMBER SEED (Enter Integer Between 0 and 65534):\n");
     scanf("%ld", &rndsd);
     fflush(stdin);

     /* Read in run stopping time */
     printf("\nSTOPPING TIME (Enter number of time units until run termination):\n");
     scanf("%lf", &stop_time);
     fflush(stdin);

     /* Read in trace_flag */
     printf("\n\nTRACE (1 = Event Trace, 0 = Summary Only):\n");
     scanf("%d", &trace_flag);
     fflush(stdin);

     /* Parameters for the initial event */;
   }

 else {
    /* this is not the first time and there is no .exp file */
    return 0;
    }
  
   /* PLACE CUSTOMIZED INITIALIZATIONS HERE */

if (trace_flag)
   {
   fprintf(output_fp,"    Time\t  Event\t Count");
   fprintf(output_fp,"	  SERVICE[ID[7]]");
   fprintf(output_fp,"	   SYSTEM[ID[7]]");
   fprintf(output_fp,"	          RATIO[0] ");
   fprintf(output_fp,"\n");
   }
  /* Initialize CSIGLIB variables and files */
  c_initlk(rndsd);
  c_initfiles();

  return(1);
}



void
run_end()
{
  printf("\r\nNormal completion after %f time units\n",current_time);
  printf("The Next Seed In the Random Input Stream is %ld\n",rndsd);

  ///  Summary statistics ///
  fprintf(output_fp,"SUMMARY STATISTICS\n");
  printf("SUMMARY STATISTICS\n");
  c_timest(SERVICE[ID[7]], 1, 1);
   fprintf(output_fp, "SERVICE[ID[7]]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SERVICE[ID[7]]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SERVICE[ID[7]], 1, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(SYSTEM[ID[7]], 2, 1);
   fprintf(output_fp, "SYSTEM[ID[7]]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("SYSTEM[ID[7]]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(SYSTEM[ID[7]], 2, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  c_timest(RATIO[0], 3, 1);
   fprintf(output_fp, "RATIO[0]:\n Time Ave. = \t%7.4g Time Sample Var. =\t%7.4g\n", transfer[4], transfer[7]);
   printf("RATIO[0]:\n Time Ave. = 	%7.4g Time Sample Var. = 	%7.4g\n", transfer[4], transfer[7]);
  c_sampst(RATIO[0], 3, 1);
   fprintf(output_fp, " Event Ave. =\t%7.4g Event Sample Var. =\t%7.4g\n", transfer[4], transfer[8]);
   fprintf(output_fp, " Minimum =\t%7.4g\n", transfer[7]);
   fprintf(output_fp, " Maximum =\t%7.4g\n", transfer[6]);
   printf(" Event Ave.  = 	%7.4g Event Sample Var.= 	%7.4g\n", transfer[4], transfer[8]);
   printf(" Minimum  = 	%7.4g\n", transfer[7]);
   printf(" Maximum  = 	%7.4g\n", transfer[6]);
  printf("Output written to, %s\n",output_file_name);
  fclose(output_fp);
  c_closedisk();
  done = 1;
}



/****************************/
/*     EVENT FUNCTIONS      */
/****************************/

/***  ***/
void
INIT()
{
int  _edge_condition[2];


  /* state changes */
  SERVER[0]=2;
  SERVER[1]=1;
  QUEUE[0]=0;
  QUEUE[1]=0;
  ID[0]=-1;
  ID[1]=0;
  ID[2]=0;
  ID[3]=0;
  ID[4]=0;
  ID[5]=0;
  ID[6]=0;
  ID[7]=0;
  RATIO[0]=0;
  RATIO[1]=0;
  RATIO[2]=0;
  ID[9]=0;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );
  _edge_condition[1] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Initialization event  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + -2.4966*LN(1-RND);
    event_type = ARR_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 180;
    event_type = STOP_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Arrival of customers into the system ***/
void
ARR()
{
int  _edge_condition[2];


  /* state changes */
  ID[0]=ID[0]+1;
  QUEUE[0]=QUEUE[0]+1;
  SYSTEM[ID[0]]=CLK;
  RATIO[1]=RATIO[1]+CLK*(QUEUE[0]==8&&RATIO[2]==0);
  RATIO[2]=(QUEUE[0]>7);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( SERVER[0]>0&&QUEUE[0]>0 );
  _edge_condition[1] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Start creating pizza if server is available  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[0];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = TOPPING_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Arrival event  ***/
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + -2.4966*LN(1-RND)*(CLK<50||(CLK>80&&CLK<110)||CLK>140)+-0.7444*LN(1-RND)*((CLK>=50&&CLK<=80)||(CLK>=110&&CLK<=140));
    event_type = ARR_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Begin process of creating pizza. ***/
void
TOPPING()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  ID[1] = (long) transfer[3];

  /* state changes */
  QUEUE[0]=QUEUE[0]-1;
  SERVER[0]=SERVER[0]-1;
  ID[2]=ID[2]+1;
  SERVICE[ID[1]]=CLK;
  RATIO[0]=RATIO[0]+(CLK-RATIO[1])*(QUEUE[0]==7);
  RATIO[1]=RATIO[1]*(QUEUE[0]!=7);
  RATIO[2]=(RATIO[1]!=0);

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Schedule end of creation  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[1];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0.3666+2.6334*RND;
    event_type = COOK_event;
    event_priority = 6;
    schedule_event();
    }

}


/*** Start baking the pizza ***/
void
COOK()
{
int  _edge_condition[3];

  /* Attribute Value(s) Passed to this Event */
  ID[3] = (long) transfer[3];

  /* state changes */
  SERVER[0]=SERVER[0]+1;
  SERVICE[ID[3]]=CLK-SERVICE[ID[3]];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( QUEUE[0]>0 );
  _edge_condition[1] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Schedule another pizza creation if queue is not empty.  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[2];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = TOPPING_event;
    event_priority = 5;
    schedule_event();
    }

  if (_edge_condition[1])
    /***  Baking the pizza  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[3];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 2.9333+3*RND;
    event_type = READY_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Pizza is out of the oven ***/
void
READY()
{
int  _edge_condition[1];

  /* Attribute Value(s) Passed to this Event */
  ID[4] = (long) transfer[3];

  /* state changes */
  QUEUE[1]=QUEUE[1]+1;
  ID[9]=(ID[4]!=ID[6])*(ID[4]*(QUEUE[1]==1));

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( SERVER[1]>0&&QUEUE[1]>0 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  If server is available, finish pizza for delivery  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[4];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = FINISH_event;
    event_priority = 5;
    schedule_event();
    }

}


/*** Finishing touches on pizza (extra toppings and boxing) ***/
void
FINISH()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  ID[5] = (long) transfer[3];

  /* state changes */
  QUEUE[1]=QUEUE[1]-1;
  SERVER[1]=SERVER[1]-1;
  ID[6]=ID[6]+1;
  ID[9]=0;

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Complete finishing touches  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[5];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + -0.9494*LN(1-RND);
    event_type = DELIVER_event;
    event_priority = 6;
    schedule_event();
    }

}


/*** Deliver pizza to the customer ***/
void
DELIVER()
{
int  _edge_condition[2];

  /* Attribute Value(s) Passed to this Event */
  ID[7] = (long) transfer[3];

  /* state changes */
  SERVER[1]=SERVER[1]+1;
  SYSTEM[ID[7]]=CLK-SYSTEM[ID[7]];

  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( QUEUE[1]>0 );

  /* schedule future events */
  if (_edge_condition[0])
    /***  Start finishing next pizza if pizza is waiting.  ***/
    {
    /*** attribute value(s) to be transferred to event ***/
    transfer[3] = ID[6]*(ID[9]==0)+(ID[9]!=0)*ID[9];
    for ( t_index=4; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    event_time = current_time + 0;
    event_type = FINISH_event;
    event_priority = 5;
    schedule_event();
    }

}


/***  ***/
void
STOP()
{
int  _edge_condition[1];


  /* Evaluate edge conditions now so that they will*/
  /* not be changed by preemptive event execution  */
  _edge_condition[0] = ( 1==1 );

  /* schedule future events */
  if (_edge_condition[0])
    {
    for ( t_index=3; t_index<maxatr; t_index++) transfer[t_index] = 0.0;
    /*** cancel event ***/
    c_cancel ( ARR_event );
    }

}

